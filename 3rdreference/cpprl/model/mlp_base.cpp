#include "model/mlp_base.h"
#include "model/model_utils.h"
#include <torch/types.h>
#include <torch/nn/modules/container/functional.h>
#include <doctest/doctest.h>

namespace cpprl {
    MlpBase::MlpBase(unsigned int num_inputs,
                     bool recurrent,
                     unsigned int hidden_size)
            : NNBase(recurrent, num_inputs, hidden_size),
              actor(nullptr),
              critic(nullptr),
              critic_linear(nullptr),
              num_inputs(num_inputs) {
        if (recurrent) {
            // If using a recurrent architecture, the inputs are first processed through
            // a GRU layer, so the actor and critic parts of the network take the hidden
            // size as their input size.
            num_inputs = hidden_size;
        }

        actor = nn::Sequential(nn::Linear(num_inputs, hidden_size),
                               nn::Functional(torch::tanh),
                               nn::Linear(hidden_size, hidden_size),
                               nn::Functional(torch::tanh));
        critic = nn::Sequential(nn::Linear(num_inputs, hidden_size),
                                nn::Functional(torch::tanh),
                                nn::Linear(hidden_size, hidden_size),
                                nn::Functional(torch::tanh));
        critic_linear = nn::Linear(hidden_size, 1);

        register_module("actor", actor);
        register_module("critic", critic);
        register_module("critic_linear", critic_linear);

        init_weights(actor->named_parameters(), sqrt(2.), 0);
        init_weights(critic->named_parameters(), sqrt(2.), 0);
        init_weights(critic_linear->named_parameters(), sqrt(2.), 0);

        train();
    }

    std::vector<torch::Tensor> MlpBase::forward(torch::Tensor inputs,
                                                torch::Tensor rnn_hxs,
                                                torch::Tensor masks) {
        auto x = inputs;

        if (is_recurrent()) {
            auto gru_output = forward_gru(x, rnn_hxs, masks);
            x = gru_output[0];
            rnn_hxs = gru_output[1];
        }

        auto hidden_critic = critic->forward(x);
        auto hidden_actor = actor->forward(x);

        return {critic_linear->forward(hidden_critic), hidden_actor, rnn_hxs};
    }

    TEST_CASE("MlpBase") {
        SUBCASE("Recurrent") {
            auto base = MlpBase(5, true, 10);

            SUBCASE("Sanity checks") {
                DOCTEST_CHECK(base.is_recurrent() == true);
                DOCTEST_CHECK(base.get_hidden_size() == 10);
            }

            SUBCASE("Output tensors are correct shapes") {
                auto inputs = torch::rand({4, 5});
                auto rnn_hxs = torch::rand({4, 10});
                auto masks = torch::zeros({4, 1});
                auto outputs = base.forward(inputs, rnn_hxs, masks);

                REQUIRE(outputs.size() == 3);
                // Critic
                DOCTEST_CHECK(outputs[0].size(0) == 4);
                DOCTEST_CHECK(outputs[0].size(1) == 1);

                // Actor
                DOCTEST_CHECK(outputs[1].size(0) == 4);
                DOCTEST_CHECK(outputs[1].size(1) == 10);

                // Hidden state
                DOCTEST_CHECK(outputs[2].size(0) == 4);
                DOCTEST_CHECK(outputs[2].size(1) == 10);
            }
        }

        SUBCASE("Non-recurrent") {
            auto base = MlpBase(5, false, 10);

            SUBCASE("Sanity checks") {
                DOCTEST_CHECK(base.is_recurrent() == false);
            }

            SUBCASE("Output tensors are correct shapes") {
                auto inputs = torch::rand({4, 5});
                auto rnn_hxs = torch::rand({4, 10});
                auto masks = torch::zeros({4, 1});
                auto outputs = base.forward(inputs, rnn_hxs, masks);

                REQUIRE(outputs.size() == 3);

                // Critic
                DOCTEST_CHECK(outputs[0].size(0) == 4);
                DOCTEST_CHECK(outputs[0].size(1) == 1);

                // Actor
                DOCTEST_CHECK(outputs[1].size(0) == 4);
                DOCTEST_CHECK(outputs[1].size(1) == 10);

                // Hidden state
                DOCTEST_CHECK(outputs[2].size(0) == 4);
                DOCTEST_CHECK(outputs[2].size(1) == 10);
            }
        }
    }
}