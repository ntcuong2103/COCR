#include "distributions/categorical.h"
#include <torch/types.h>
#include <doctest/doctest.h>

namespace cpprl {
    Categorical::Categorical(const torch::Tensor *probs,
                             const torch::Tensor *logits) {
        if ((probs == nullptr) == (logits == nullptr)) {
            throw std::runtime_error("Either probs or logits is required, but not both");
        }

        if (probs != nullptr) {
            if (probs->dim() < 1) {
                throw std::runtime_error("Probabilities tensor must have at least one dimension");
            }
            this->probs = *probs / probs->sum(-1, true);
            // 1.21e-7 is used as the epsilon to match PyTorch's Python results as closely
            // as possible
            this->probs = this->probs.clamp(1.21e-7, 1. - 1.21e-7);
            this->logits = torch::log(this->probs);
        } else {
            if (logits->dim() < 1) {
                throw std::runtime_error("Logits tensor must have at least one dimension");
            }
            this->logits = *logits - logits->logsumexp(-1, true);
            this->probs = torch::softmax(this->logits, -1);
        }

        param = probs != nullptr ? *probs : *logits;
        num_events = param.size(-1);
        if (param.dim() > 1) {
            batch_shape = param.sizes().vec();
            batch_shape.resize(batch_shape.size() - 1);
        }
    }

    torch::Tensor Categorical::entropy() {
        auto p_log_p = logits * probs;
        return -p_log_p.sum(-1);
    }

    torch::Tensor Categorical::log_prob(torch::Tensor value) {
        value = value.to(torch::kLong).unsqueeze(-1);
        auto broadcasted_tensors = torch::broadcast_tensors({value, logits});
        value = broadcasted_tensors[0];
        value = value.narrow(-1, 0, 1);
        return broadcasted_tensors[1].gather(-1, value).squeeze(-1);
    }

    torch::Tensor Categorical::sample(c10::ArrayRef<int64_t> sample_shape) {
        auto ext_sample_shape = extended_shape(sample_shape);
        auto param_shape = ext_sample_shape;
        param_shape.insert(param_shape.end(), {num_events});
        auto exp_probs = probs.expand(param_shape);
        torch::Tensor probs_2d;
        if (probs.dim() == 1 || probs.size(0) == 1) {
            probs_2d = exp_probs.view({-1, num_events});
        } else {
            probs_2d = exp_probs.contiguous().view({-1, num_events});
        }
        auto sample_2d = torch::multinomial(probs_2d, 1, true);
        return sample_2d.contiguous().view(ext_sample_shape);
    }

    TEST_CASE("Categorical") {
        SUBCASE("Throws when provided both probs and logits") {
            auto tensor = torch::Tensor();
            CHECK_THROWS(Categorical(&tensor, &tensor)
            );
        }

        SUBCASE("Sampled numbers are in the right range") {
            float probabilities[] = {0.2, 0.2, 0.2, 0.2, 0.2};
            auto probabilities_tensor = torch::from_blob(probabilities, {5});
            auto dist = Categorical(&probabilities_tensor, nullptr);

            auto output = dist.sample({100});
            auto more_than_4 = output > 4;
            auto less_than_0 = output < 0;
            DOCTEST_CHECK(!more_than_4.any().item().toInt());
            DOCTEST_CHECK(!less_than_0.any().item().toInt());
        }

        SUBCASE("Sampled tensors are of the right shape") {
            float probabilities[] = {0.2, 0.2, 0.2, 0.2, 0.2};
            auto probabilities_tensor = torch::from_blob(probabilities, {5});
            auto dist = Categorical(&probabilities_tensor, nullptr);

            DOCTEST_CHECK(dist.sample({20}).sizes().vec() == std::vector<int64_t>{20});
            DOCTEST_CHECK(dist.sample({2, 20}).sizes().vec() == std::vector<int64_t>{2, 20});
            DOCTEST_CHECK(dist.sample({1, 2, 3, 4, 5}).sizes().vec() == std::vector<int64_t>{1, 2, 3, 4, 5});
        }

        SUBCASE("Multi-dimensional input probabilities are handled correctly") {
            SUBCASE("Sampled tensors are of the right shape") {
                float probabilities[2][4] = {{0.5,  0.5,  0.0,  0.0},
                                             {0.25, 0.25, 0.25, 0.25}};
                auto probabilities_tensor = torch::from_blob(probabilities, {2, 4});
                auto dist = Categorical(&probabilities_tensor, nullptr);

                DOCTEST_CHECK(dist.sample({20}).sizes().vec() == std::vector<int64_t>{20, 2});
                DOCTEST_CHECK(dist.sample({10, 5}).sizes().vec() == std::vector<int64_t>{10, 5, 2});
            }

            SUBCASE("Generated tensors have correct probabilities") {
                float probabilities[2][4] = {{0, 1, 0, 0},
                                             {0, 0, 0, 1}};
                auto probabilities_tensor = torch::from_blob(probabilities, {2, 4});
                auto dist = Categorical(&probabilities_tensor, nullptr);

                auto output = dist.sample({5});
                auto sum = output.sum({0});

                DOCTEST_CHECK(sum[0].item().toInt() == 5);
                DOCTEST_CHECK(sum[1].item().toInt() == 15);
            }
        }

        SUBCASE("entropy()") {
            float probabilities[2][4] = {{0.5,  0.5,  0.0,  0.0},
                                         {0.25, 0.25, 0.25, 0.25}};
            auto probabilities_tensor = torch::from_blob(probabilities, {2, 4});
            auto dist = Categorical(&probabilities_tensor, nullptr);

            auto entropies = dist.entropy();

            SUBCASE("Returns correct values") {
                DOCTEST_CHECK(entropies[0].item().toDouble() ==
                              doctest::Approx(0.6931).epsilon(1e-3));

                DOCTEST_CHECK(entropies[1].item().toDouble() ==
                              doctest::Approx(1.3863).epsilon(1e-3));
            }

            SUBCASE("Output tensor is the correct size") {
                DOCTEST_CHECK(entropies.sizes().vec() == std::vector<int64_t>{2});
            }
        }

        SUBCASE("log_prob()") {
            float probabilities[2][4] = {{0.5,  0.5,  0.0,  0.0},
                                         {0.25, 0.25, 0.25, 0.25}};
            auto probabilities_tensor = torch::from_blob(probabilities, {2, 4});
            auto dist = Categorical(&probabilities_tensor, nullptr);

            float actions[2][2] = {{0, 1},
                                   {2, 3}};
            auto actions_tensor = torch::from_blob(actions, {2, 2});
            auto log_probs = dist.log_prob(actions_tensor);

            INFO(log_probs
                         << "\n");
            SUBCASE("Returns correct values") {
                DOCTEST_CHECK(log_probs[0][0].item().toDouble() ==
                              doctest::Approx(-0.6931).epsilon(1e-3));
                DOCTEST_CHECK(log_probs[0][1].item().toDouble() ==
                              doctest::Approx(-1.3863).epsilon(1e-3));
                DOCTEST_CHECK(log_probs[1][0].item().toDouble() ==
                              doctest::Approx(-15.9424).epsilon(1e-3));
                DOCTEST_CHECK(log_probs[1][1].item().toDouble() ==
                              doctest::Approx(-1.3863).epsilon(1e-3));
            }

            SUBCASE("Output tensor is correct size") {
                DOCTEST_CHECK(log_probs.sizes().vec() == std::vector<int64_t>{2, 2});
            }
        }
    }
}