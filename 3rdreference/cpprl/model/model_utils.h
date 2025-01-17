#pragma once

#include <torch/types.h>
#include <torch/nn/module.h>
#include <vector>

using namespace torch;

namespace cpprl {
    struct FlattenImpl : nn::Module {
        torch::Tensor forward(torch::Tensor x);
    };

    TORCH_MODULE(Flatten);

    void init_weights(torch::OrderedDict<std::string, torch::Tensor> parameters,
                      double weight_gain,
                      double bias_gain);
}