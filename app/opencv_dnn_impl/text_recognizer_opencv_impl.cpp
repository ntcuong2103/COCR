#include "text_recognizer_opencv_impl.hpp"
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>


bool xgd::TextRecognitionOpenCVImpl::initModel(
        const std::string &_onnxFile, const std::string &_words, int _width) {
    try {
        dstWidth = _width;
        model = std::make_shared<cv::dnn::TextRecognitionModel>(_onnxFile);
        model->setDecodeType("CTC-greedy");
        for (auto &c:_words) {
            wordVec.push_back(std::string(1, c));
        }
        model->setVocabulary(wordVec);
        model->setInputParams(normValues, cv::Size(dstWidth, dstHeight), cv::Scalar(meanValues));
    } catch (...) {
        return false;
    }
    return true;
}

void xgd::TextRecognitionOpenCVImpl::freeModel() {
    model = nullptr;
}

std::pair<std::string, std::vector<float>> xgd::TextRecognitionOpenCVImpl::recognize(
        const cv::Mat &_originImage) {
    cv::Mat srcResized = preProcess(_originImage);
    std::string recognitionResult = model->recognize(srcResized);
    std::vector<float> scores(recognitionResult.size(), -1);
    return {recognitionResult, scores};
}

cv::Mat xgd::TextRecognitionOpenCVImpl::preProcess(const cv::Mat &_src) {
    cv::Mat srcResized = TextRecognition::preProcess(_src);
    if (srcResized.cols < dstWidth) {
        cv::hconcat(srcResized, cv::Mat(dstHeight, dstWidth - srcResized.cols, CV_8UC1, cv::Scalar(255)), srcResized);
    } else if (srcResized.cols > dstWidth) {
        cv::resize(srcResized, srcResized, cv::Size(dstWidth, dstHeight), 0, 0, cv::INTER_CUBIC);
    }
    return srcResized;
}