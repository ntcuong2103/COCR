#pragma once

#include "storage.h"

#include <string>
#include <vector>

namespace cpprl {
    struct UpdateDatum {
        std::string name;
        float value;
    };

    class Algorithm {
    public:
        virtual ~Algorithm() = 0;

        virtual std::vector<UpdateDatum> update(RolloutStorage &rollouts, float decay_level = 1) = 0;
    };

    inline Algorithm::~Algorithm() {}
}