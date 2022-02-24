#ifndef NETWORKS_HPP
#define NETWORKS_HPP

#include <string>
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

torch::Tensor AbsoluteEuclideanDistance(torch::Tensor x1, torch::Tensor x2, const bool rec_detach = true);

torch::Tensor RelativeEuclideanDistance(torch::Tensor x1, torch::Tensor x2, const bool rec_detach = true);

torch::Tensor CosineSimilarity(torch::Tensor x1, torch::Tensor x2, const bool rec_detach = true);

void save_params(const std::string path, torch::Tensor mu, torch::Tensor sigma, torch::Tensor phi);

void load_params(const std::string path, torch::Tensor &mu, torch::Tensor &sigma, torch::Tensor &phi);


// -------------------------------------------------
// struct{EncoderImpl}(nn::Module)
// -------------------------------------------------
struct EncoderImpl : nn::Module {
private:
    nn::Sequential model;
public:
    EncoderImpl() {}

    EncoderImpl(po::variables_map &vm);

    torch::Tensor forward(torch::Tensor x);
};

// -------------------------------------------------
// struct{DecoderImpl}(nn::Module)
// -------------------------------------------------
struct DecoderImpl : nn::Module {
private:
    nn::Sequential model;
public:
    DecoderImpl() {}

    DecoderImpl(po::variables_map &vm);

    torch::Tensor forward(torch::Tensor z_c);
};

// -------------------------------------------------
// struct{EstimationNetworkImpl}(nn::Module)
// -------------------------------------------------
struct EstimationNetworkImpl : nn::Module {
private:
    float eps;
    size_t N;
    torch::Tensor gamma_sum_keep, mu_sum_keep, sigma_sum_keep;
    torch::Tensor energy_keep, NVI_keep, precision_keep;
    nn::Sequential model;
public:
    EstimationNetworkImpl() {}

    EstimationNetworkImpl(po::variables_map &vm, const float eps_ = 1e-12);

    torch::Tensor forward(torch::Tensor z);

    void estimation(torch::Tensor z, torch::Tensor gamma_ap);

    void estimationNVI(torch::Tensor z, torch::Tensor gamma_ap);

    void resetGMP(torch::Device device);

    void estimationGMP(torch::Tensor z, torch::Tensor gamma_ap);

    torch::Tensor estimated_mu();

    torch::Tensor estimated_sigma();

    torch::Tensor estimated_phi();

    torch::Tensor energy_just_before();

    torch::Tensor NVI_just_before();

    torch::Tensor precision_just_before();

    torch::Tensor anomaly_score(torch::Tensor z, torch::Tensor mu, torch::Tensor sigma, torch::Tensor phi);
};

// -------------------------------------------------
// struct{LinearResBlockImpl}(nn::Module)
// -------------------------------------------------
struct LinearResBlockImpl : nn::Module {
private:
    nn::Sequential model;
public:
    LinearResBlockImpl() {}

    LinearResBlockImpl(const size_t nr, bool use_dropout = false);

    torch::Tensor forward(torch::Tensor x);
};


TORCH_MODULE(Encoder);

TORCH_MODULE(Decoder);

TORCH_MODULE(EstimationNetwork);

TORCH_MODULE(LinearResBlock);


#endif