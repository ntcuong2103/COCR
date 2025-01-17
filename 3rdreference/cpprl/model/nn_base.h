#pragma once

#include <torch/types.h>
#include <torch/nn/module.h>
#include <torch/nn/modules/rnn.h>
#include <vector>

using namespace torch;

namespace cpprl {
    class NNBase : public nn::Module {
    private:
        nn::GRU gru;
        unsigned int hidden_size;
        bool recurrent;

    public:
        NNBase(bool recurrent,
               unsigned int recurrent_input_size,
               unsigned int hidden_size);

        virtual std::vector<torch::Tensor> forward(torch::Tensor inputs,
                                                   torch::Tensor hxs,
                                                   torch::Tensor masks);

        std::vector<torch::Tensor> forward_gru(torch::Tensor x,
                                               torch::Tensor hxs,
                                               torch::Tensor masks);

        unsigned int get_hidden_size() const;

        inline int get_output_size() const { return hidden_size; }

        inline bool is_recurrent() const { return recurrent; }
    };
}