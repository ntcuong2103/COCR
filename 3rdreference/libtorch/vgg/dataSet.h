#pragma once

#include <torch/torch.h>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

//遍历该目录下的.jpg图片
void load_data_from_folder(std::string image_dir, std::string type, std::vector<std::string> &list_images,
                           std::vector<int> &list_labels, int label);

class dataSetClc : public torch::data::Dataset<dataSetClc> {
public:
    int num_classes = 0;

    dataSetClc(std::string image_dir, std::string type) {
        load_data_from_folder(image_dir, std::string(type), image_paths, labels, num_classes - 1);
    }

    // Override get() function to return tensor at location index
    torch::data::Example<> get(size_t index) override {
        std::string image_path = image_paths.at(index);
        cv::Mat image = cv::imread(image_path);
        cv::resize(image, image, cv::Size(448, 448));
        int label = labels.at(index);
        torch::Tensor img_tensor = torch::from_blob(image.data, {image.rows, image.cols, 3}, torch::kByte).permute(
                {2, 0, 1}); // Channels x Height x Width
        torch::Tensor label_tensor = torch::full({1}, label);
        return {img_tensor.clone(), label_tensor.clone()};
    }

    // Return the length of data
    torch::optional<size_t> size() const override {
        return image_paths.size();
    }

private:
    std::vector<std::string> image_paths;
    std::vector<int> labels;
};
