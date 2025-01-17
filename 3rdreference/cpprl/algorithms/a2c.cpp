#include "algorithms/a2c.h"
#include "algorithms/algorithm.h"
#include "model/mlp_base.h"
#include "model/policy.h"
#include "storage.h"
#include "spaces.h"

#include <torch/types.h>
#include <doctest/doctest.h>
#include <chrono>
#include <memory>

namespace cpprl {
    A2C::A2C(Policy &policy,
             float actor_loss_coef,
             float value_loss_coef,
             float entropy_coef,
             float learning_rate,
             float epsilon,
             float alpha,
             float max_grad_norm)
            : policy(policy),
              actor_loss_coef(actor_loss_coef),
              value_loss_coef(value_loss_coef),
              entropy_coef(entropy_coef),
              max_grad_norm(max_grad_norm),
              original_learning_rate(learning_rate),
              optimizer(std::make_unique<torch::optim::RMSprop>(
                      policy->parameters(),
                      torch::optim::RMSpropOptions(learning_rate)
                              .eps(epsilon)
                              .alpha(alpha))) {}

    std::vector<UpdateDatum> A2C::update(RolloutStorage &rollouts, float decay_level) {
        // Decay learning rate
        optimizer->defaults().set_lr(original_learning_rate * decay_level);
        // Prep work
        auto full_obs_shape = rollouts.get_observations().sizes();
        std::vector<int64_t> obs_shape(full_obs_shape.begin() + 2,
                                       full_obs_shape.end());
        obs_shape.insert(obs_shape.begin(), -1);
        auto action_shape = rollouts.get_actions().size(-1);
        auto rewards_shape = rollouts.get_rewards().sizes();
        int num_steps = rewards_shape[0];
        int num_processes = rewards_shape[1];

        // Update observation normalizer
        if (policy->using_observation_normalizer()) {
            policy->update_observation_normalizer(rollouts.get_observations());
        }

        // Run evaluation on rollouts
        auto evaluate_result = policy->evaluate_actions(
                rollouts.get_observations().slice(0, 0, -1).view(obs_shape),
                rollouts.get_hidden_states()[0].view({-1, policy->get_hidden_size()}),
                rollouts.get_masks().slice(0, 0, -1).view({-1, 1}),
                rollouts.get_actions().view({-1, action_shape}));
        auto values = evaluate_result[0].view({num_steps, num_processes, 1});
        auto action_log_probs = evaluate_result[1].view(
                {num_steps, num_processes, 1});

        // Calculate advantages
        // Advantages aren't normalized (they are in PPO)
        auto advantages = rollouts.get_returns().slice(0, 0, -1) - values;

        // Value loss
        auto value_loss = advantages.pow(2).mean();

        // Action loss
        auto action_loss = -(advantages.detach() * action_log_probs).mean();

        // Total loss
        auto loss = (value_loss * value_loss_coef +
                     action_loss -
                     evaluate_result[2] * entropy_coef);

        // Step optimizer
        optimizer->zero_grad();
        loss.backward();
        optimizer->step();

        return {{"Value loss",  value_loss.item().toFloat()},
                {"Action loss", action_loss.item().toFloat()},
                {"Entropy",     evaluate_result[2].item().toFloat()}};
    }

    static void learn_pattern(Policy &policy, RolloutStorage &storage, A2C &a2c) {
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

            a2c.update(storage);
            storage.after_update();
        }
    }

    static void learn_game(Policy &policy, RolloutStorage &storage, A2C &a2c) {
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

            a2c.update(storage);
            storage.after_update();
        }
    }

    TEST_CASE("A2C") {
        SUBCASE("update() learns basic pattern") {
            torch::manual_seed(0);
            auto base = std::make_shared<MlpBase>(1, false, 5);
            ActionSpace space{"Discrete", {2}};
            Policy policy(space, base);
            RolloutStorage storage(5, 2, {1}, space, 5, torch::kCPU);
            A2C a2c(policy, 1, 0.5, 1e-3, 0.001);

            // The reward is the action
            auto pre_game_probs = policy->get_probs(
                    torch::ones({2, 1}),
                    torch::zeros({2, 5}),
                    torch::ones({2, 1}));

            learn_pattern(policy, storage, a2c
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
            SUBCASE("Without normalized observations") {
                torch::manual_seed(0);
                auto base = std::make_shared<MlpBase>(1, false, 5);
                ActionSpace space{"Discrete", {2}};
                Policy policy(space, base);
                RolloutStorage storage(5, 2, {1}, space, 5, torch::kCPU);
                A2C a2c(policy, 1, 0.5, 1e-7, 0.0001);

                auto pre_game_probs = policy->get_probs(
                        torch::ones({2, 1}),
                        torch::zeros({2, 5}),
                        torch::ones({2, 1}));

                learn_game(policy, storage, a2c
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

            SUBCASE("With normalized observations") {
                torch::manual_seed(0);
                auto base = std::make_shared<MlpBase>(1, false, 5);
                ActionSpace space{"Discrete", {2}};
                Policy policy(space, base, true);
                RolloutStorage storage(5, 2, {1}, space, 5, torch::kCPU);
                A2C a2c(policy, 1, 0.5, 1e-7, 0.0001);

                auto pre_game_probs = policy->get_probs(
                        torch::ones({2, 1}),
                        torch::zeros({2, 5}),
                        torch::ones({2, 1}));

                learn_game(policy, storage, a2c
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