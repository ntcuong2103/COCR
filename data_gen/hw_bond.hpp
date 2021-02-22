#ifndef _HW_BOND_HPP_
#define _HW_BOND_HPP_

#include "bond_type.hpp"
#include "hw_item.hpp"

class HwBond : public HwItem {
protected:
    virtual void loadHwData() = 0;

public:
    static std::shared_ptr<HwBond> GetHwBond(
            const JBondType &_bondType = JBondType::SingleBond);

    virtual void setVertices(const std::vector<cv::Point2f> &_pts) = 0;
};

class HwCircleBond : public HwBond {
    cv::Point2f center;
    float rx, ry;

    void loadHwData() override;

public:

    std::shared_ptr<HwBase> clone() const override;

    DetectorClasses getItemType() const override;

    HwCircleBond() = default;

    void setVertices(const std::vector<cv::Point2f> &_pts) override;

    void rotateBy(float _angle, const cv::Point2f &_cent);

    void moveBy(const cv::Point2f &_offset);

    void mulK(float _kx, float _ky);
};

class HwSingleBond : public HwBond {
protected:
    cv::Point2f from, to;

    void loadHwData() override;

public:
    std::shared_ptr<HwBase> clone() const override;

    DetectorClasses getItemType() const override;

    HwSingleBond() = default;

    void setVertices(const std::vector<cv::Point2f> &_pts) override;

    void rotateBy(float _angle, const cv::Point2f &_cent);

    void moveBy(const cv::Point2f &_offset);

    void mulK(float _kx, float _ky);
};

class HwDoubleBond : public HwSingleBond {

    void loadHwData() override;

public:
    std::shared_ptr<HwBase> clone() const override;
    DetectorClasses getItemType() const override;

    HwDoubleBond() = default;
};

class HwTripleBond : public HwSingleBond {
    void loadHwData() override;

public:
    std::shared_ptr<HwBase> clone() const override;
    DetectorClasses getItemType() const override;

    HwTripleBond() = default;
};

class HwSolidWedgeBond : public HwSingleBond {
    void loadHwData() override;

public:
    std::shared_ptr<HwBase> clone() const override;
    DetectorClasses getItemType() const override;

    HwSolidWedgeBond() = default;
};

class HwDashWedgeBond : public HwSingleBond {
    void loadHwData() override;

public:
    std::shared_ptr<HwBase> clone() const override;
    DetectorClasses getItemType() const override;

    HwDashWedgeBond() = default;
};

class HwWaveBond : public HwSingleBond {
    void loadHwData() override;

public:
    std::shared_ptr<HwBase> clone() const override;
    DetectorClasses getItemType() const override;

    HwWaveBond() = default;
};

#endif//_HW_BOND_HPP_