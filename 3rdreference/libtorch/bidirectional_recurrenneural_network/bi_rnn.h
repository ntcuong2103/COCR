// Copyright 2020-present pytorch-cpp Authors
#pragma once

#include <torch/types.h>
#include <torch/nn/modules/rnn.h>
#include <torch/nn/module.h>
#include <torch/nn/modules/linear.h>

class BiRNNImpl : public torch::nn::Module {
public:
    BiRNNImpl(int64_t input_size, int64_t hidden_size, int64_t num_layers, int64_t num_classes);

    torch::Tensor forward(torch::Tensor x);

private:
    torch::nn::LSTM lstm;
    torch::nn::Linear fc;
};

TORCH_MODULE(BiRNN);
