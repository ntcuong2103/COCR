#include <utility>
#include <typeinfo>
#include <cmath>
// For External Library
#include <torch/types.h>
#include <torch/nn/init.h>
#include <torch/nn/module.h>
#include <torch/nn/modules/container/sequential.h>
#include <torch/nn/modules/activation.h>
#include <torch/nn/modules/batchnorm.h>
#include <torch/nn/modules/conv.h>
#include <torch/nn/modules/linear.h>
// For Original Header
#include "networks.hpp"

// Define Namespace
namespace nn = torch::nn;


// ----------------------------------------------------------------------
// struct{GAN_EncoderImpl}(nn::Module) -> constructor
// ----------------------------------------------------------------------
GAN_EncoderImpl::GAN_EncoderImpl(po::variables_map &vm) {

    size_t feature = vm["nef"].as<size_t>();
    size_t num_downs = (size_t) (std::log2(vm["size"].as<size_t>()));

    DownSampling(this->model, vm["nc"].as<size_t>(), feature, /*BN=*/false, /*LReLU=*/
                 true);     // {C,256,256} ===> {F,128,128}
    DownSampling(this->model, feature, feature * 2, /*BN=*/true, /*LReLU=*/
                 true);                  // {F,128,128} ===> {2F,64,64}
    DownSampling(this->model, feature * 2, feature * 4, /*BN=*/true, /*LReLU=*/
                 true);                // {2F,64,64}  ===> {4F,32,32}
    DownSampling(this->model, feature * 4, feature * 8, /*BN=*/true, /*LReLU=*/
                 true);                // {4F,32,32}  ===> {8F,16,16}
    for (size_t i = 0; i < num_downs - 5; i++) {
        DownSampling(this->model, feature * 8, feature * 8, /*BN=*/true, /*LReLU=*/
                     true);            // {8F,16,16}    ===> {8F,2,2}
    }
    DownSampling(this->model, feature * 8, vm["nz"].as<size_t>(), /*BN=*/false, /*LReLU=*/
                 false);  // {8F,2,2}    ===> {Z,1,1}
    register_module("Encoder", this->model);

}


// ----------------------------------------------------------------------
// struct{GAN_EncoderImpl}(nn::Module) -> function{forward}
// ----------------------------------------------------------------------
torch::Tensor GAN_EncoderImpl::forward(torch::Tensor x) {
    torch::Tensor out = this->model->forward(x);  // {C,256,256} ===> {Z,1,1}
    out = out.view({out.size(0), -1});            // {Z,1,1}     ===> {Z}
    return out;
}


// ----------------------------------------------------------------------
// struct{GAN_GeneratorImpl}(nn::Module) -> constructor
// ----------------------------------------------------------------------
GAN_GeneratorImpl::GAN_GeneratorImpl(po::variables_map &vm) {

    size_t feature = vm["ngf"].as<size_t>();
    size_t num_downs = (size_t) (std::log2(vm["size"].as<size_t>()));

    UpSampling(this->model, vm["nz"].as<size_t>(), feature * 8, /*BN=*/true, /*ReLU=*/
               true);  // {Z,1,1}     ===> {8F,2,2}
    for (size_t i = 0; i < num_downs - 5; i++) {
        UpSampling(this->model, feature * 8, feature * 8, /*BN=*/true, /*ReLU=*/
                   true);          // {8F,2,2}    ===> {8F,16,16}
    }
    UpSampling(this->model, feature * 8, feature * 4, /*BN=*/true, /*ReLU=*/
               true);              // {8F,16,16}  ===> {4F,32,32}
    UpSampling(this->model, feature * 4, feature * 2, /*BN=*/true, /*ReLU=*/
               true);              // {4F,32,32}  ===> {2F,64,64}
    UpSampling(this->model, feature * 2, feature, /*BN=*/true, /*ReLU=*/
               true);                // {2F,64,64}  ===> {F,128,128}
    UpSampling(this->model, feature, vm["nc"].as<size_t>(), /*BN=*/false, /*ReLU=*/
               false);  // {F,128,128} ===> {C,256,256}
    this->model->push_back(nn::Tanh());                                                     // [-inf,+inf] ===> [-1,1]
    register_module("Generator", this->model);

}


// ----------------------------------------------------------------------
// struct{GAN_GeneratorImpl}(nn::Module) -> function{forward}
// ----------------------------------------------------------------------
torch::Tensor GAN_GeneratorImpl::forward(torch::Tensor z) {
    z = z.view({z.size(0), z.size(1), 1, 1});     // {Z} ===> {Z,1,1}
    torch::Tensor out = this->model->forward(z);  // {Z,1,1} ===> {C,256,256}
    return out;
}


// ----------------------------------------------------------------------
// struct{GAN_DiscriminatorImpl}(nn::Module) -> constructor
// ----------------------------------------------------------------------
GAN_DiscriminatorImpl::GAN_DiscriminatorImpl(po::variables_map &vm) {

    size_t feature = vm["ndf"].as<size_t>();
    size_t num_downs = (size_t) (std::log2(vm["size"].as<size_t>()));

    DownSampling(this->x_down, vm["nc"].as<size_t>(), feature, /*BN=*/false, /*LReLU=*/
                 true);  // {C,256,256} ===> {F,128,128}
    DownSampling(this->x_down, feature, feature * 2, /*BN=*/true, /*LReLU=*/
                 true);               // {F,128,128} ===> {2F,64,64}
    DownSampling(this->x_down, feature * 2, feature * 4, /*BN=*/true, /*LReLU=*/
                 true);             // {2F,64,64}  ===> {4F,32,32}
    DownSampling(this->x_down, feature * 4, feature * 8, /*BN=*/true, /*LReLU=*/
                 true);             // {4F,32,32}  ===> {8F,16,16}
    for (size_t i = 0; i < num_downs - 5; i++) {
        DownSampling(this->x_down, feature * 8, feature * 8, /*BN=*/true, /*LReLU=*/
                     true);         // {8F,16,16}  ===> {8F,2,2}
    }
    register_module("x_down", this->x_down);

    this->z_down = nn::Sequential(
            nn::Linear(vm["nz"].as<size_t>(), feature * 8)  // {Z} ===> {8F}
    );
    register_module("z_down", this->z_down);

    this->features = nn::Sequential(
            nn::Linear(feature * 8 * 2 * 2 + feature * 8, feature * 16),  // {40F} ===> {16F}
            nn::LeakyReLU(nn::LeakyReLUOptions().negative_slope(0.2).inplace(true))
    );
    register_module("features", this->features);

    this->classifier = nn::Sequential(
            nn::Linear(feature * 16, 1)  // {16F} ===> {1}
    );
    register_module("classifier", this->classifier);

}


// ----------------------------------------------------------------------
// struct{GAN_DiscriminatorImpl}(nn::Module) -> function{forward}
// ----------------------------------------------------------------------
std::pair<torch::Tensor, torch::Tensor> GAN_DiscriminatorImpl::forward(torch::Tensor x, torch::Tensor z) {
    torch::Tensor x_mid, z_mid, mid, feature, out;
    std::pair<torch::Tensor, torch::Tensor> out_with_feature;
    x_mid = this->x_down->forward(x);             // {C,256,256}  ===> {8F,2,2}
    x_mid = x_mid.view({x_mid.size(0), -1});      // {8F,2,2}     ===> {32F}
    z_mid = this->z_down->forward(z);             // {Z}          ===> {8F}
    mid = torch::cat({x_mid, z_mid}, /*dim=*/1);  // {32F} + {8F} ===> {40F}
    feature = this->features->forward(mid);       // {40F}        ===> {16F}
    out = this->classifier->forward(feature);     // {16F}        ===> {1}
    out_with_feature = {out, feature};
    return out_with_feature;
}


// ----------------------------
// function{weights_init}
// ----------------------------
void weights_init(nn::Module &m) {
    if ((typeid(m) == typeid(nn::Conv2d)) || (typeid(m) == typeid(nn::Conv2dImpl)) ||
        (typeid(m) == typeid(nn::ConvTranspose2d)) || (typeid(m) == typeid(nn::ConvTranspose2dImpl))) {
        auto p = m.named_parameters(false);
        auto w = p.find("weight");
        auto b = p.find("bias");
        if (w != nullptr) nn::init::normal_(*w, /*mean=*/0.0, /*std=*/0.02);
        if (b != nullptr) nn::init::constant_(*b, /*bias=*/0.0);
    } else if ((typeid(m) == typeid(nn::Linear)) || (typeid(m) == typeid(nn::LinearImpl))) {
        auto p = m.named_parameters(false);
        auto w = p.find("weight");
        auto b = p.find("bias");
        if (w != nullptr) nn::init::normal_(*w, /*mean=*/0.0, /*std=*/0.02);
        if (b != nullptr) nn::init::constant_(*b, /*bias=*/0.0);
    } else if ((typeid(m) == typeid(nn::BatchNorm2d)) || (typeid(m) == typeid(nn::BatchNorm2dImpl))) {
        auto p = m.named_parameters(false);
        auto w = p.find("weight");
        auto b = p.find("bias");
        if (w != nullptr) nn::init::normal_(*w, /*mean=*/1.0, /*std=*/0.02);
        if (b != nullptr) nn::init::constant_(*b, /*bias=*/0.0);
    }
    return;
}


// ----------------------------
// function{DownSampling}
// ----------------------------
void DownSampling(nn::Sequential &sq, const size_t in_nc, const size_t out_nc, const bool BN, const bool LReLU,
                  const bool bias) {
    sq->push_back(nn::Conv2d(nn::Conv2dOptions(in_nc, out_nc, 4).stride(2).padding(1).bias(bias)));
    if (BN) {
        sq->push_back(nn::BatchNorm2d(out_nc));
    }
    if (LReLU) {
        sq->push_back(nn::LeakyReLU(nn::LeakyReLUOptions().negative_slope(0.2).inplace(true)));
    }
    return;
}


// ----------------------------
// function{UpSampling}
// ----------------------------
void UpSampling(nn::Sequential &sq, const size_t in_nc, const size_t out_nc, const bool BN, const bool ReLU,
                const bool bias) {
    sq->push_back(nn::ConvTranspose2d(nn::ConvTranspose2dOptions(in_nc, out_nc, 4).stride(2).padding(1).bias(bias)));
    if (BN) {
        sq->push_back(nn::BatchNorm2d(out_nc));
    }
    if (ReLU) {
        sq->push_back(nn::ReLU(nn::ReLUOptions().inplace(true)));
    }
    return;
}