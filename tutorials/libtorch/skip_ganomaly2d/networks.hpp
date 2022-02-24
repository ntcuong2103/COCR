#ifndef NETWORKS_HPP
#define NETWORKS_HPP

#include <utility>
// For External Library
#include <torch/types.h>
#include <torch/nn/module.h>
#include <torch/nn/modules/container/sequential.h>
#include <boost/program_options.hpp>

// Define Namespace
namespace nn = torch::nn;
namespace po = boost::program_options;

// Function Prototype
void weights_init(nn::Module &m);

void DownSampling(nn::Sequential &sq, const size_t in_nc, const size_t out_nc, const bool BN, const bool ReLU,
                  const bool bias = false);

void UpSampling(nn::Sequential &sq, const size_t in_nc, const size_t out_nc, const bool BN, const bool ReLU,
                const bool bias = false);


// -------------------------------------------------
// struct{UNet_GeneratorImpl}(nn::Module)
// -------------------------------------------------
struct UNet_GeneratorImpl : nn::Module {
private:
    nn::Sequential model;
public:
    UNet_GeneratorImpl() {}

    UNet_GeneratorImpl(po::variables_map &vm);

    torch::Tensor forward(torch::Tensor x);
};

// -------------------------------------------------
// struct{UNetBlockImpl}(nn::Module)
// -------------------------------------------------
struct UNetBlockImpl : nn::Module {
private:
    bool outermost;
    nn::Sequential model;
public:
    UNetBlockImpl() {}

    UNetBlockImpl(const std::pair<size_t, size_t> outside_nc, const size_t inside_nc, UNetBlockImpl &submodule,
                  bool outermost_ = false, bool innermost = false, bool use_dropout = false);

    torch::Tensor forward(torch::Tensor x);
};

// ----------------------------------------------------------
// struct{GAN_DiscriminatorImpl}(nn::Module)
// ----------------------------------------------------------
struct GAN_DiscriminatorImpl : nn::Module {
private:
    nn::Sequential down, features, classifier;
public:
    GAN_DiscriminatorImpl() {}

    GAN_DiscriminatorImpl(po::variables_map &vm);

    std::pair<torch::Tensor, torch::Tensor> forward(torch::Tensor x);
};

TORCH_MODULE(UNet_Generator);

TORCH_MODULE(UNetBlock);

TORCH_MODULE(GAN_Discriminator);


#endif