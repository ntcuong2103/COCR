#include "model/model_utils.h"
#include <torch/nn/init.h>
#include <torch/cuda.h>
#include <torch/nn/modules/container/sequential.h>
#include <torch/nn/modules/linear.h>
#include <torch/nn/modules/container/functional.h>
#include <doctest/doctest.h>
#include <torch/linalg.h>

using namespace torch;

namespace cpprl {
    torch::Tensor orthogonal_(Tensor tensor, double gain) {
        NoGradGuard guard;
        // Only tensors with 2 or more dimensions are supported
        DOCTEST_CHECK(tensor.ndimension() >= 2);

        const auto rows = tensor.size(0);
        const auto columns = tensor.numel() / rows;
        auto flattened = torch::randn({rows, columns})
                // try skip lapack on msvc cpu mode
                .to(torch::cuda::is_available() ? torch::kCUDA : torch::kCPU);

        if (rows < columns) {
            flattened.t_();
        }

        // Compute the qr factorization
        Tensor q, r;
        std::tie(q, r) = torch::linalg::qr(flattened);
        // Make Q uniform according to https://arxiv.org/pdf/math-ph/0609050.pdf
        auto d = torch::diag(r, 0);
        auto ph = d.sign();
        q *= ph;

        if (rows < columns) {
            q.t_();
        }

        tensor.view_as(q).copy_(q);
        tensor.mul_(gain);

        return tensor;
    }

    torch::Tensor FlattenImpl::forward(torch::Tensor x) {
        return x.view({x.size(0), -1});
    }

    void init_weights(torch::OrderedDict<std::string, torch::Tensor> parameters,
                      double weight_gain,
                      double bias_gain) {
        for (const auto &parameter: parameters) {
            if (parameter.value().size(0) != 0) {
                if (parameter.key().find("bias") != std::string::npos) {
                    nn::init::constant_(parameter.value(), bias_gain);
                } else if (parameter.key().find("weight") != std::string::npos) {
                    orthogonal_(parameter.value(), weight_gain);
                }
            }
        }
    }

    TEST_CASE("Flatten") {
        auto flatten = Flatten();

        SUBCASE("Flatten converts 3 dimensional vector to 2 dimensional") {
            auto input = torch::rand({5, 5, 5});
            auto output = flatten->forward(input);

            DOCTEST_CHECK(output.size(0) == 5);
            DOCTEST_CHECK(output.size(1) == 25);
        }

        SUBCASE("Flatten converts 5 dimensional vector to 2 dimensional") {
            auto input = torch::rand({2, 2, 2, 2, 2});
            auto output = flatten->forward(input);

            DOCTEST_CHECK(output.size(0) == 2);
            DOCTEST_CHECK(output.size(1) == 16);
        }

        SUBCASE("Flatten converts 1 dimensional vector to 2 dimensional") {
            auto input = torch::rand({10});
            auto output = flatten->forward(input);

            DOCTEST_CHECK(output.size(0) == 10);
            DOCTEST_CHECK(output.size(1) == 1);
        }
    }

    TEST_CASE("init_weights()")
    {
        auto module = nn::Sequential(
                nn::Linear(5, 10),
                nn::Functional(torch::relu),
                nn::Linear(10, 8));
        init_weights(module->named_parameters(), 1, 0);
        SUBCASE("Bias weights are initialized to 0") {
            for (const auto &parameter: module->named_parameters()) {
                if (parameter.key().find("bias") != std::string::npos) {
                    DOCTEST_CHECK(parameter.value()[0].item().toDouble() == doctest::Approx(0));
                }
            }
        }
    }
}