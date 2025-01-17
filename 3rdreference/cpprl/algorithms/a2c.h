#pragma once

#include "algorithms/algorithm.h"

#include <torch/types.h>
#include <torch/optim/rmsprop.h>
#include <string>
#include <vector>

namespace cpprl {
    class Policy;

    class ROlloutStorage;

    class A2C : public Algorithm {
    private:
        Policy &policy;
        float actor_loss_coef, value_loss_coef, entropy_coef, max_grad_norm, original_learning_rate;
        std::unique_ptr<torch::optim::RMSprop> optimizer;

    public:
        A2C(Policy &policy,
            float actor_loss_coef,
            float value_loss_coef,
            float entropy_coef,
            float learning_rate,
            float epsilon = 1e-8,
            float alpha = 0.99,
            float max_grad_norm = 0.5);

        std::vector<UpdateDatum> update(RolloutStorage &rollouts, float decay_level = 1);
    };
}