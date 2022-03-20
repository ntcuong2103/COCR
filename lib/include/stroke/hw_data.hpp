#pragma once

#include "stroke/hw_script.hpp"
#include <map>
#include <string>
#include <vector>


enum class ShapeType {
    Line,
    Circle
};

class COCR_STROKE_EXPORT HwDataSample {
    std::vector<std::vector<cv::Point2f>> mData;
public:
    HwDataSample() = default;

    HwDataSample(std::vector<std::vector<cv::Point2f>> &&_data);

    HwScript toHwScript() const;
};


class COCR_STROKE_EXPORT HwDataLoader {
    std::vector<std::vector<HwDataSample>> mData;

    HwDataLoader();

    ~HwDataLoader() = default;

    HwDataLoader(const HwDataLoader &) = delete;

    const HwDataLoader &operator=(const HwDataLoader &) = delete;

    bool isDataLoaded;

    void LoadCouchDataSet();

public:
    static HwDataLoader &getInstance();

    HwScript GetByIntLabel(int label);

    HwScript GetByStrLabel(const std::string &label);

    HwScript GetShape(const ShapeType &_shapeType);

    HwScript GetHwScript(const size_t &_classIndex, const size_t &_sampleIndex);
};