#pragma once
#include <ref_torch_public_export.h>
// For External Library
#include <torch/types.h>


// -------------------
// namespace{Losses}
// -------------------
namespace Losses {

    // -------------------------------------
    // namespace{Losses} -> class{SSIMLoss}
    // -------------------------------------
    class REF_TORCH_PUBLIC_EXPORT SSIMLoss {
    private:
        size_t nc;
        size_t window_size;
        float gauss_std;
        float c1_base;
        float c2_base;
        torch::Tensor window;
    public:
        SSIMLoss() {}

        SSIMLoss(const size_t nc_, const torch::Device device, const size_t window_size_ = 11,
                 const float gauss_std_ = 1.5, const float c1_base_ = 0.01, const float c2_base_ = 0.03);

        torch::Tensor Structural_Similarity(torch::Tensor &image1, torch::Tensor &image2);

        torch::Tensor operator()(torch::Tensor &input, torch::Tensor &target);
    };

}