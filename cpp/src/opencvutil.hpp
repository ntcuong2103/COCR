#ifndef _OPENCVUTIL_HPP_
#define _OPENCVUTIL_HPP_

#include "config.hpp"
#include <optional>
#include <string>
#include <utility>
#include <opencv2/core/mat.hpp>

/**
 *
 * @param _img
 * @param _newWidth
 * @param _newHeight
 * @return 图像，{比例、水平偏移、竖直偏移}
 */
std::pair<cv::Mat, std::tuple<float, float, float>>
padCvMatTo(const cv::Mat &_img, const int &_newWidth, const int &_newHeight);

/**
 * 在单通道黑白[0-255]位图上，寻找非 _bgPixel 像素的最小正包围盒
 * @param _uMat 填充为uchar，与 _bgPixel 可作相等比较
 * @param _bgPixel 背景颜色值
 * @return 正包围盒
 */
std::optional<cv::Rect2i> getBoundBoxForBWFont(const cv::Mat &_uMat, const uchar&_bgPixel=255);

#endif//_OPENCVUTIL_HPP_