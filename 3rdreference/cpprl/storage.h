#pragma once

#include "generators/generator.h"
#include "spaces.h"

#include <torch/types.h>
#include <memory>

namespace cpprl {
    class RolloutStorage {
    private:
        torch::Tensor observations, hidden_states, rewards, value_predictions,
                returns, action_log_probs, actions, masks;
        torch::Device device;
        int64_t num_steps;
        int64_t step;

    public:
        RolloutStorage(int64_t num_steps,
                       int64_t num_processes,
                       c10::ArrayRef<int64_t> obs_shape,
                       ActionSpace action_space,
                       int64_t hidden_state_size,
                       torch::Device device);

        RolloutStorage(std::vector<RolloutStorage *> individual_storages, torch::Device device);

        void after_update();

        void compute_returns(torch::Tensor next_value,
                             bool use_gae,
                             float gamma,
                             float tau);

        std::unique_ptr<Generator> feed_forward_generator(torch::Tensor advantages,
                                                          int num_mini_batch);

        void insert(torch::Tensor observation,
                    torch::Tensor hidden_state,
                    torch::Tensor action,
                    torch::Tensor action_log_prob,
                    torch::Tensor value_prediction,
                    torch::Tensor reward,
                    torch::Tensor mask);

        std::unique_ptr<Generator> recurrent_generator(torch::Tensor advantages,
                                                       int num_mini_batch);

        void set_first_observation(torch::Tensor observation);

        void to(torch::Device device);

        inline const torch::Tensor &get_actions() const { return actions; }

        inline const torch::Tensor &get_action_log_probs() const { return action_log_probs; }

        inline const torch::Tensor &get_hidden_states() const { return hidden_states; }

        inline const torch::Tensor &get_masks() const { return masks; }

        inline const torch::Tensor &get_observations() const { return observations; }

        inline const torch::Tensor &get_returns() const { return returns; }

        inline const torch::Tensor &get_rewards() const { return rewards; }

        inline const torch::Tensor &get_value_predictions() const {
            return value_predictions;
        }

        inline void set_actions(torch::Tensor actions) { this->actions = actions; }

        inline void set_action_log_probs(torch::Tensor action_log_probs) { this->action_log_probs = action_log_probs; }

        inline void set_hidden_states(torch::Tensor hidden_states) { this->hidden_states = hidden_states; }

        inline void set_masks(torch::Tensor masks) { this->masks = masks; }

        inline void set_observations(torch::Tensor observations) { this->observations = observations; }

        inline void set_returns(torch::Tensor returns) { this->returns = returns; }

        inline void set_rewards(torch::Tensor rewards) { this->rewards = rewards; }

        inline void set_value_predictions(torch::Tensor value_predictions) {
            this->value_predictions = value_predictions;
        }
    };
}