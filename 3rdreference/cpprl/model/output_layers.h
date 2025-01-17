#pragma once

#include "distributions/distribution.h"

#include <torch/types.h>
#include <torch/nn/module.h>
#include <torch/nn/modules/linear.h>

#include <memory>

using namespace torch;

namespace cpprl {
    class OutputLayer : public nn::Module {
    public:
        virtual ~OutputLayer() = 0;

        virtual std::unique_ptr<Distribution> forward(torch::Tensor x) = 0;
    };

    inline OutputLayer::~OutputLayer() {}

    class BernoulliOutput : public OutputLayer {
    private:
        nn::Linear linear;

    public:
        BernoulliOutput(unsigned int num_inputs, unsigned int num_outputs);

        std::unique_ptr<Distribution> forward(torch::Tensor x);
    };

    class CategoricalOutput : public OutputLayer {
    private:
        nn::Linear linear;

    public:
        CategoricalOutput(unsigned int num_inputs, unsigned int num_outputs);

        std::unique_ptr<Distribution> forward(torch::Tensor x);
    };

    class NormalOutput : public OutputLayer {
    private:
        nn::Linear linear_loc;
        torch::Tensor scale_log;

    public:
        NormalOutput(unsigned int num_inputs, unsigned int num_outputs);

        std::unique_ptr<Distribution> forward(torch::Tensor x);
    };
}