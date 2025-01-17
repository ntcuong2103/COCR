#include "algorithms/ppo.h"
#include "algorithms/algorithm.h"
#include "generators/generator.h"
#include "model/mlp_base.h"
#include "model/policy.h"
#include "storage.h"
#include "spaces.h"

#include <torch/types.h>
#include <doctest/doctest.h>
#include <chrono>
#include <memory>

namespace cpprl {

    PPO::PPO(Policy &policy,
             float clip_param,
             int num_epoch,
             int num_mini_batch,
             float actor_loss_coef,
             float value_loss_coef,
             float entropy_coef,
             float learning_rate,
             float epsilon,
             float max_grad_norm,
             float kl_target)
            : policy(policy),
              actor_loss_coef(actor_loss_coef),
              value_loss_coef(value_loss_coef),
              entropy_coef(entropy_coef),
              max_grad_norm(max_grad_norm),
              original_learning_rate(learning_rate),
              original_clip_param(clip_param),
              kl_target(kl_target),
              num_epoch(num_epoch),
              num_mini_batch(num_mini_batch),
              optimizer(std::make_unique<torch::optim::Adam>(
                      policy->parameters(),
                      torch::optim::AdamOptions(learning_rate)
                              .eps(epsilon))) {}

    std::vector<UpdateDatum> PPO::update(RolloutStorage &rollouts, float decay_level) {
        // Decay lr and clip parameter
        float clip_param = original_clip_param * decay_level;
        optimizer->defaults().set_lr(original_learning_rate * decay_level);

        // Calculate advantages
        auto returns = rollouts.get_returns();
        auto value_preds = rollouts.get_value_predictions();
        auto advantages = (returns.narrow(0, 0, returns.size(0) - 1) -
                           value_preds.narrow(0, 0, value_preds.size(0) - 1));

        // Normalize advantages
        advantages = (advantages - advantages.mean()) / (advantages.std() + 1e-5);

        float total_value_loss = 0;
        float total_action_loss = 0;
        float total_entropy = 0;
        float kl_divergence = 0;
        float kl_early_stopped = -1;
        float clip_fraction = 0;
        int num_updates = 0;

        // Epoch loop
        for (int epoch = 0; epoch < num_epoch; ++epoch) {
            // Shuffle rollouts
            std::unique_ptr<Generator> data_generator;
            if (policy->is_recurrent()) {
                data_generator = rollouts.recurrent_generator(advantages,
                                                              num_mini_batch);
            } else {
                data_generator = rollouts.feed_forward_generator(advantages,
                                                                 num_mini_batch);
            }

            // Loop through shuffled rollout
            while (!data_generator->done()) {
                MiniBatch mini_batch = data_generator->next();

                // Run evaluation on minibatch
                auto evaluate_result = policy->evaluate_actions(
                        mini_batch.observations,
                        mini_batch.hidden_states,
                        mini_batch.masks,
                        mini_batch.actions);

                // Calculate approximate KL divergence for info and early stopping
                kl_divergence = (mini_batch.action_log_probs - evaluate_result[1])
                        .mean()
                        .item()
                        .toFloat();
                if (kl_divergence > kl_target * 1.5) {
                    kl_early_stopped = num_updates;
                    goto finish_update;
                }

                // Calculate difference ratio between old and new action probabilites
                auto ratio = torch::exp(evaluate_result[1] -
                                        mini_batch.action_log_probs);

                // PPO loss formula
                auto surr_1 = ratio * mini_batch.advantages;
                auto surr_2 = (torch::clamp(ratio,
                                            1.0 - clip_param,
                                            1.0 + clip_param) *
                               mini_batch.advantages);
                clip_fraction += (ratio - 1.0)
                        .abs()
                        .gt(clip_param)
                        .to(torch::kFloat)
                        .mean()
                        .item()
                        .toFloat();
                auto action_loss = -torch::min(surr_1, surr_2).mean();

                // Value loss
                auto value_loss = 0.5 * (mini_batch.returns - evaluate_result[0])
                        .pow(2)
                        .mean();
                // TODO: Implement clipped value loss

                // Total loss
                auto loss = (value_loss * value_loss_coef +
                             action_loss * actor_loss_coef -
                             evaluate_result[2] * entropy_coef);

                // Step optimizer
                optimizer->zero_grad();
                loss.backward();
                // TODO: Implement gradient norm clipping
                optimizer->step();
                num_updates++;

                total_value_loss += value_loss.item().toFloat();
                total_action_loss += action_loss.item().toFloat();
                total_entropy += evaluate_result[2].item().toFloat();
            }
        }

        finish_update:
        // Update observation normalizer
        if (policy->using_observation_normalizer()) {
            policy->update_observation_normalizer(rollouts.get_observations());
        }

        total_value_loss /= num_updates;
        total_action_loss /= num_updates;
        total_entropy /= num_updates;
        clip_fraction /= num_updates;

        if (kl_early_stopped > -1) {
            return {{"Value loss",                      total_value_loss},
                    {"Action loss",                     total_action_loss},
                    {"Clip fraction",                   clip_fraction},
                    {"Entropy",                         total_entropy},
                    {"KL divergence",                   kl_divergence},
                    {"KL divergence early stop update", kl_early_stopped}};
        } else {
            return {{"Value loss",    total_value_loss},
                    {"Action loss",   total_action_loss},
                    {"Clip fraction", clip_fraction},
                    {"Entropy",       total_entropy},
                    {"KL divergence", kl_divergence}};
        }
    }

    static void learn_pattern(Policy &policy, RolloutStorage &storage, PPO &ppo) {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 5; ++j) {
                auto observation = torch::randint(0, 2, {2, 1});

                std::vector<torch::Tensor> act_result;
                {
                    torch::NoGradGuard no_grad;
                    act_result = policy->act(observation,
                                             torch::Tensor(),
                                             torch::ones({2, 1}));
                }
                auto actions = act_result[1];

                auto rewards = actions;
                storage.insert(observation,
                               torch::zeros({2, 5}),
                               actions,
                               act_result[2],
                               act_result[0],
                               rewards,
                               torch::ones({2, 1}));
            }

            torch::Tensor next_value;
            {
                torch::NoGradGuard no_grad;
                next_value = policy->get_values(
                                storage.get_observations()[-1],
                                storage.get_hidden_states()[-1],
                                storage.get_masks()[-1])
                        .detach();
            }
            storage.compute_returns(next_value, false, 0., 0.9);

            ppo.update(storage);
            storage.after_update();
        }
    }

    static void learn_game(Policy &policy, RolloutStorage &storage, PPO &ppo) {
        // The game is: If the action matches the input, give a reward of 1, otherwise -1
        auto observation = torch::randint(0, 2, {2, 1});
        storage.set_first_observation(observation);

        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 5; ++j) {
                std::vector<torch::Tensor> act_result;
                {
                    torch::NoGradGuard no_grad;
                    act_result = policy->act(observation,
                                             torch::Tensor(),
                                             torch::ones({2, 1}));
                }
                auto actions = act_result[1];

                auto rewards = ((actions == observation.to(torch::kLong)).to(torch::kFloat) * 2) - 1;
                observation = torch::randint(0, 2, {2, 1});
                storage.insert(observation,
                               torch::zeros({2, 5}),
                               actions,
                               act_result[2],
                               act_result[0],
                               rewards,
                               torch::ones({2, 1}));
            }

            torch::Tensor next_value;
            {
                torch::NoGradGuard no_grad;
                next_value = policy->get_values(
                                storage.get_observations()[-1],
                                storage.get_hidden_states()[-1],
                                storage.get_masks()[-1])
                        .detach();
            }
            storage.compute_returns(next_value, false, 0.1, 0.9);

            ppo.update(storage);
            storage.after_update();
        }
    }

    TEST_CASE("PPO") {
        torch::manual_seed(0);
        SUBCASE("update() learns basic pattern") {
            auto base = std::make_shared<MlpBase>(1, false, 5);
            ActionSpace space{"Discrete", {2}};
            Policy policy(space, base, false);
            RolloutStorage storage(20, 2, {1}, space, 5, torch::kCPU);
            PPO ppo(policy, 0.2, 3, 5, 1, 0.5, 1e-3, 0.001);

            // The reward is the action
            auto pre_game_probs = policy->get_probs(
                    torch::ones({2, 1}),
                    torch::zeros({2, 5}),
                    torch::ones({2, 1}));

            learn_pattern(policy, storage, ppo
            );

            auto post_game_probs = policy->get_probs(
                    torch::ones({2, 1}),
                    torch::zeros({2, 5}),
                    torch::ones({2, 1}));

            INFO("Pre-training probabilities: \n"
                         << pre_game_probs << "\n");
            INFO("Post-training probabilities: \n"
                         << post_game_probs << "\n");
            DOCTEST_CHECK(post_game_probs[0][0].item().toDouble() <
                          pre_game_probs[0][0].item().toDouble());
            DOCTEST_CHECK(post_game_probs[0][1].item().toDouble() >
                          pre_game_probs[0][1].item().toDouble());
        }

        SUBCASE("update() learns basic game") {
            SUBCASE("Without observation normalization") {
                auto base = std::make_shared<MlpBase>(1, false, 5);
                ActionSpace space{"Discrete", {2}};
                Policy policy(space, base, false);
                RolloutStorage storage(20, 2, {1}, space, 5, torch::kCPU);
                PPO ppo(policy, 0.2, 3, 5, 1, 0.5, 1e-3, 0.001);

// The game is: If the action matches the input, give a reward of 1, otherwise -1
                auto pre_game_probs = policy->get_probs(
                        torch::ones({2, 1}),
                        torch::zeros({2, 5}),
                        torch::ones({2, 1}));

                learn_game(policy, storage, ppo
                );

                auto post_game_probs = policy->get_probs(
                        torch::ones({2, 1}),
                        torch::zeros({2, 5}),
                        torch::ones({2, 1}));

                INFO("Pre-training probabilities: \n"
                             << pre_game_probs << "\n");
                INFO("Post-training probabilities: \n"
                             << post_game_probs << "\n");
                DOCTEST_CHECK(post_game_probs[0][0].item().toDouble() <
                              pre_game_probs[0][0].item().toDouble());
                DOCTEST_CHECK(post_game_probs[0][1].item().toDouble() >
                              pre_game_probs[0][1].item().toDouble());
            }

            SUBCASE("With observation normalization") {
                auto base = std::make_shared<MlpBase>(1, false, 5);
                ActionSpace space{"Discrete", {2}};
                Policy policy(space, base, true);
                RolloutStorage storage(20, 2, {1}, space, 5, torch::kCPU);
                PPO ppo(policy, 0.2, 3, 5, 1, 0.5, 1e-3, 0.001);

// The game is: If the action matches the input, give a reward of 1, otherwise -1
                auto pre_game_probs = policy->get_probs(
                        torch::ones({2, 1}),
                        torch::zeros({2, 5}),
                        torch::ones({2, 1}));

                learn_game(policy, storage, ppo
                );

                auto post_game_probs = policy->get_probs(
                        torch::ones({2, 1}),
                        torch::zeros({2, 5}),
                        torch::ones({2, 1}));

                INFO("Pre-training probabilities: \n"
                             << pre_game_probs << "\n");
                INFO("Post-training probabilities: \n"
                             << post_game_probs << "\n");
                DOCTEST_CHECK(post_game_probs[0][0].item().toDouble() <
                              pre_game_probs[0][0].item().toDouble());
                DOCTEST_CHECK(post_game_probs[0][1].item().toDouble() >
                              pre_game_probs[0][1].item().toDouble());
            }
        }
    }
}