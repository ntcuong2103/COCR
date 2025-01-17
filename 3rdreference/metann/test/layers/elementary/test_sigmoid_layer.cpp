#include <MetaNN/meta_nn.h>
#include "../../facilities/data_gen.h"
#include <cassert>
#include <iostream>

using namespace MetaNN;
using namespace std;

namespace {
    void test_sigmoid_layer1() {
        cout << "Test sigmoid layer case 1 ...\t";
        using RootLayer = InjectPolicy<SigmoidLayer>;
        static_assert(!RootLayer::IsFeedbackOutput, "Test Error");
        static_assert(!RootLayer::IsUpdate, "Test Error");

        RootLayer layer;

        Matrix<float, DeviceTags::CPU> in(2, 1);
        in.SetValue(0, 0, -0.27f);
        in.SetValue(1, 0, -0.41f);

        auto input = LayerIO::Create().Set<LayerIO>(in);

        LayerNeutralInvariant(layer);
        auto out = layer.FeedForward(input);
        auto res = Evaluate(out.Get<LayerIO>());
        REQUIRE(fabs(res(0, 0) - (1 / (1 + exp(0.27f)))) < 0.001);
        REQUIRE(fabs(res(1, 0) - (1 / (1 + exp(0.41f)))) < 0.001);

        NullParameter fbIn;
        auto out_grad = layer.FeedBackward(fbIn);
        auto fb1 = out_grad.Get<LayerIO>();
        static_assert(std::is_same<decltype(fb1), NullParameter>::value, "Test error");

        LayerNeutralInvariant(layer);
        cout << "done" << endl;
    }

    void test_sigmoid_layer2() {
        cout << "Test sigmoid layer case 2 ...\t";
        using RootLayer = InjectPolicy<SigmoidLayer, PFeedbackOutput>;
        static_assert(RootLayer::IsFeedbackOutput, "Test Error");
        static_assert(!RootLayer::IsUpdate, "Test Error");

        RootLayer layer;

        Matrix<float, DeviceTags::CPU> in(2, 1);
        in.SetValue(0, 0, -0.27f);
        in.SetValue(1, 0, -0.41f);

        auto input = LayerIO::Create().Set<LayerIO>(in);

        LayerNeutralInvariant(layer);
        auto out = layer.FeedForward(input);
        auto res = Evaluate(out.Get<LayerIO>());
        REQUIRE(fabs(res(0, 0) - (1 / (1 + exp(0.27f)))) < 0.001);
        REQUIRE(fabs(res(1, 0) - (1 / (1 + exp(0.41f)))) < 0.001);

        Matrix<float, DeviceTags::CPU> grad(2, 1);
        grad.SetValue(0, 0, 0.1f);
        grad.SetValue(1, 0, 0.3f);

        auto out_grad = layer.FeedBackward(LayerIO::Create().Set<LayerIO>(grad));
        auto fb = Evaluate(out_grad.Get<LayerIO>());
        REQUIRE(fabs(fb(0, 0) - 0.1f * exp(0.27f) / (1 + exp(0.27f)) / (1 + exp(0.27f))) < 0.001);
        REQUIRE(fabs(fb(1, 0) - 0.3f * exp(0.41f) / (1 + exp(0.41f)) / (1 + exp(0.41f))) < 0.001);

        LayerNeutralInvariant(layer);
        cout << "done" << endl;
    }

    void test_sigmoid_layer3() {
        cout << "Test sigmoid layer case 3 ...\t";
        using RootLayer = InjectPolicy<SigmoidLayer, PFeedbackOutput>;
        static_assert(RootLayer::IsFeedbackOutput, "Test Error");
        static_assert(!RootLayer::IsUpdate, "Test Error");

        RootLayer layer;

        vector<Matrix<float, DeviceTags::CPU>> op;

        LayerNeutralInvariant(layer);
        for (size_t loop_count = 1; loop_count < 10; ++loop_count) {
            auto in = GenMatrix<float>(loop_count, 3, 0.1f, 0.13f);

            op.push_back(in);

            auto input = LayerIO::Create().Set<LayerIO>(in);

            auto out = layer.FeedForward(input);
            auto res = Evaluate(out.Get<LayerIO>());
            REQUIRE(res.RowNum() == loop_count);
            REQUIRE(res.ColNum() == 3);
            for (size_t i = 0; i < loop_count; ++i) {
                for (size_t j = 0; j < 3; ++j) {
                    float aim = 1 / (1 + exp(-in(i, j)));
                    REQUIRE(fabs(res(i, j) - aim) < 0.0001);
                }
            }
        }

        for (size_t loop_count = 9; loop_count >= 1; --loop_count) {
            auto grad = GenMatrix<float>(loop_count, 3, 2, 1.1f);
            auto out_grad = layer.FeedBackward(LayerIO::Create().Set<LayerIO>(grad));

            auto fb = Evaluate(out_grad.Get<LayerIO>());

            auto in = op.back();
            op.pop_back();
            for (size_t i = 0; i < loop_count; ++i) {
                for (size_t j = 0; j < 3; ++j) {
                    float aim = exp(-in(i, j)) / (1 + exp(-in(i, j))) / (1 + exp(-in(i, j)));
                    REQUIRE(fabs(fb(i, j) - grad(i, j) * aim) < 0.00001f);
                }
            }
        }

        LayerNeutralInvariant(layer);

        cout << "done" << endl;
    }
}

void test_sigmoid_layer() {
    test_sigmoid_layer1();
    test_sigmoid_layer2();
    test_sigmoid_layer3();
}
