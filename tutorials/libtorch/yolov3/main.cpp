#include <iostream>                    // std::cout, std::cerr
#include <fstream>                     // std::ifstream, std::ofstream
#include <filesystem>                  // std::filesystem
#include <string>                      // std::string
#include <sstream>                     // std::istringstream
#include <tuple>                       // std::tuple
#include <vector>                      // std::vector
#include <random>                      // std::random_device
#include <cstdlib>                     // std::srand, std::rand, std::exit
// For External Library
#include <torch/types.h>               // torch
#include <torch/cuda.h>
#include <opencv2/opencv.hpp>          // cv::Mat
#include <boost/program_options.hpp>   // boost::program_options
// For Original Header
#include "networks.hpp"                // YOLOv3
#include "augmentation.hpp"            // YOLOAugmentation
#include "transforms.hpp"              // transforms

// Define Namespace
namespace fs = std::filesystem;
namespace po = boost::program_options;

// Function Prototype
void train(po::variables_map &vm, torch::Device &device, YOLOv3 &model, std::vector<transforms_Compose > &transformBB,
           std::vector<transforms_Compose > &transformI, const std::vector<std::string> class_names,
           const std::vector<std::vector<std::tuple<float, float>>> anchors,
           const std::vector<std::tuple<long int, long int>> resizes, const size_t resize_step_max);

void test(po::variables_map &vm, torch::Device &device, YOLOv3 &model, std::vector<transforms_Compose > &transform,
          const std::vector<std::string> class_names, const std::vector<std::vector<std::tuple<float, float>>> anchors);

void detect(po::variables_map &vm, torch::Device &device, YOLOv3 &model, std::vector<transforms_Compose > &transformI,
            std::vector<transforms_Compose > &transformD, const std::vector<std::string> class_names,
            const std::vector<std::vector<std::tuple<float, float>>> anchors);

void demo(po::variables_map &vm, torch::Device &device, YOLOv3 &model, std::vector<transforms_Compose > &transformI,
          std::vector<transforms_Compose > &transformD, const std::vector<std::string> class_names,
          const std::vector<std::vector<std::tuple<float, float>>> anchors);

torch::Device Set_Device(po::variables_map &vm);

template<typename T>
void Set_Model_Params(po::variables_map &vm, T &model, const std::string name);

std::vector<std::string> Set_Class_Names(const std::string path, const size_t class_num);

std::vector<std::vector<std::tuple<float, float>>>
Set_Anchors(const std::string path, const size_t scales, const size_t na);

std::vector<std::tuple<long int, long int>> Set_Resizes(const std::string path, size_t &resize_step_max);

void
Set_Options(po::variables_map &vm, int argc, const char *argv[], po::options_description &args, const std::string mode);


// -----------------------------------
// 0. Argument Function
// -----------------------------------
po::options_description parse_arguments() {

    po::options_description args("Options", 200, 30);

    args.add_options()

            // (1) Define for General Parameter
            ("help", "produce help message")
            ("dataset", po::value<std::string>(), "dataset name")
            ("class_list", po::value<std::string>()->default_value("list/VOC2012.txt"),
             "file name in which class names are listed")
            ("anchor_list", po::value<std::string>()->default_value("cfg/anchor.txt"),
             "file name in which anchors are listed")
            ("resize_list", po::value<std::string>()->default_value("cfg/resize.txt"),
             "file name in which resizes are listed")
            ("class_num", po::value<size_t>()->default_value(20), "total classes")
            ("size", po::value<size_t>()->default_value(608), "image width and height")
            ("prob_thresh", po::value<float>()->default_value(0.1),
             "threshold of simultaneous probability with confidence and class score")
            ("ignore_thresh", po::value<float>()->default_value(0.7),
             "threshold of IoU between no object anchor and ground truth for excluding loss of 'no object confidence term'")
            ("nms_thresh", po::value<float>()->default_value(0.5),
             "threshold of IoU between bounding boxes in Non-Maximum Suppression")
            ("nc", po::value<size_t>()->default_value(3), "input image channel : RGB=3, grayscale=1")
            ("na", po::value<size_t>()->default_value(3), "the number of anchor in each grid")
            ("scales", po::value<size_t>()->default_value(3), "the number of predicted map scales")
            ("gpu_id", po::value<int>()->default_value(0), "cuda device : 'x=-1' is cpu device")
            ("seed_random", po::value<bool>()->default_value(false),
             "whether to make the seed of random number in a random")
            ("seed", po::value<int>()->default_value(0), "seed of random number")

            // (2) Define for Training
            ("train", po::value<bool>()->default_value(false), "training mode on/off")
            ("train_in_dir", po::value<std::string>()->default_value("trainI"),
             "training input image directory : ./datasets/<dataset>/<train_in_dir>/<image files>")
            ("train_out_dir", po::value<std::string>()->default_value("trainO"),
             "training output image directory : ./datasets/<dataset>/<train_out_dir>/<annotation files>")
            ("epochs", po::value<size_t>()->default_value(200), "training total epoch")
            ("batch_size", po::value<size_t>()->default_value(32), "training batch size")
            ("train_load_epoch", po::value<std::string>()->default_value(""), "epoch of model to resume learning")
            ("save_epoch", po::value<size_t>()->default_value(20), "frequency of epoch to save model and optimizer")
            /*************************** Data Augmentation ***************************/
            ("augmentation", po::value<bool>()->default_value(true), "data augmentation mode on/off")
            ("jitter", po::value<double>()->default_value(0.3), "the distortion of image shifting")
            ("flip_rate", po::value<double>()->default_value(0.5), "frequency to flip")
            ("scale_rate", po::value<double>()->default_value(0.5), "frequency to scale")
            ("blur_rate", po::value<double>()->default_value(0.5), "frequency to blur")
            ("brightness_rate", po::value<double>()->default_value(0.5), "frequency to change brightness")
            ("hue_rate", po::value<double>()->default_value(0.5), "frequency to change hue")
            ("saturation_rate", po::value<double>()->default_value(0.5), "frequency to change saturation")
            ("shift_rate", po::value<double>()->default_value(0.5), "frequency to shift")
            ("crop_rate", po::value<double>()->default_value(0.5), "frequency to crop")

            // (3) Define for Validation
            ("valid", po::value<bool>()->default_value(false), "validation mode on/off")
            ("valid_in_dir", po::value<std::string>()->default_value("validI"),
             "validation input image directory : ./datasets/<dataset>/<valid_in_dir>/<image files>")
            ("valid_out_dir", po::value<std::string>()->default_value("validO"),
             "validation output image directory : ./datasets/<dataset>/<valid_out_dir>/<annotation files>")
            ("valid_batch_size", po::value<size_t>()->default_value(1), "validation batch size")
            ("valid_freq", po::value<size_t>()->default_value(1), "validation frequency to training epoch")

            // (4) Define for Test
            ("test", po::value<bool>()->default_value(false), "test mode on/off")
            ("test_in_dir", po::value<std::string>()->default_value("testI"),
             "test input image directory : ./datasets/<dataset>/<test_in_dir>/<image files>")
            ("test_out_dir", po::value<std::string>()->default_value("testO"),
             "test output image directory : ./datasets/<dataset>/<test_out_dir>/<annotation files>")
            ("test_load_epoch", po::value<std::string>()->default_value("latest"), "training epoch used for testing")
            ("test_result_dir", po::value<std::string>()->default_value("test_result"),
             "test result directory : ./<test_result_dir>")

            // (5) Define for Detection
            ("detect", po::value<bool>()->default_value(false), "detection mode on/off")
            ("detect_dir", po::value<std::string>()->default_value("detect"),
             "detection image directory : ./datasets/<dataset>/<detect_dir>/<image files>")
            ("detect_load_epoch", po::value<std::string>()->default_value("latest"),
             "training epoch used for detection")
            ("detect_result_dir", po::value<std::string>()->default_value("detect_result"),
             "detection result directory : ./<detect_result_dir>")

            // (6) Define for Demo
            ("demo", po::value<bool>()->default_value(false), "demo mode on/off")
            ("cam_num", po::value<size_t>()->default_value(0), "the port number of camera")
            ("movie", po::value<std::string>()->default_value(""), "movie file name : ./<movie>")
            ("window_w", po::value<size_t>()->default_value(1920), "width of window size")
            ("window_h", po::value<size_t>()->default_value(1080), "height of window size")
            ("demo_load_epoch", po::value<std::string>()->default_value("latest"), "training epoch used for demo")

            // (7) Define for Network Parameter
            ("lr_init", po::value<float>()->default_value(1e-3), "learning rate in the initial iteration")
            ("lr_base", po::value<float>()->default_value(1e-2), "learning rate in the base iteration")
            ("lr_decay1", po::value<float>()->default_value(1e-3), "learning rate in the decay 1 iteration")
            ("lr_decay2", po::value<float>()->default_value(1e-4), "learning rate in the decay 2 iteration")
            ("momentum", po::value<float>()->default_value(0.9), "momentum in SGD of optimizer method")
            ("weight_decay", po::value<float>()->default_value(5e-4), "weight decay in SGD of optimizer method")
            ("Lambda_coord", po::value<float>()->default_value(1.0), "the multiple of coordinate term")
            ("Lambda_object", po::value<float>()->default_value(1.0), "the multiple of object confidence term")
            ("Lambda_noobject", po::value<float>()->default_value(1.0), "the multiple of no object confidence term")
            ("Lambda_class", po::value<float>()->default_value(1.0), "the multiple of class term");

    // End Processing
    return args;
}


// -----------------------------------
// 1. Main Function
// -----------------------------------
int main(int argc, const char *argv[]) {

    // (1) Extract Arguments
    po::options_description args = parse_arguments();
    po::variables_map vm{};
    po::store(po::parse_command_line(argc, argv, args), vm);
    po::notify(vm);
    if (vm.empty() || vm.count("help")) {
        std::cout << args << std::endl;
        return 1;
    }

    // (2) Select Device
    torch::Device device = Set_Device(vm);
    std::cout << "using device = " << device << std::endl;

    // (3) Set Seed
    if (vm["seed_random"].as<bool>()) {
        std::random_device rd;
        std::srand(rd());
        torch::manual_seed(std::rand());
        torch::globalContext().setDeterministicCuDNN(false);
        torch::globalContext().setBenchmarkCuDNN(true);
    } else {
        std::srand(vm["seed"].as<int>());
        torch::manual_seed(std::rand());
        torch::globalContext().setDeterministicCuDNN(true);
        torch::globalContext().setBenchmarkCuDNN(false);
    }

    // (4) Set Transforms
    std::vector<transforms_Compose > transformBB;
    if (vm["augmentation"].as<bool>()) {
        transformBB.push_back(
                YOLOAugmentation(  // apply "flip", "scale", "blur", "brightness", "hue", "saturation", "shift", "crop"
                        vm["jitter"].as<double>(),
                        vm["flip_rate"].as<double>(),
                        vm["scale_rate"].as<double>(),
                        vm["blur_rate"].as<double>(),
                        vm["brightness_rate"].as<double>(),
                        vm["hue_rate"].as<double>(),
                        vm["saturation_rate"].as<double>(),
                        vm["shift_rate"].as<double>(),
                        vm["crop_rate"].as<double>()
                )
        );
    }
    /*************************************************************************/
    std::vector<transforms_Compose > transformI{
            transforms_Resize(cv::Size(vm["size"].as<size_t>(), vm["size"].as<size_t>()),
                              cv::INTER_LINEAR),  // {IH,IW,C} ===method{OW,OH}===> {OH,OW,C}
            transforms_ToTensor()                                                                             // Mat Image [0,255] or [0,65535] ===> Tensor Image [0,1]
    };
    if (vm["nc"].as<size_t>() == 1) {
        transformI.insert(transformI.begin(), transforms_Grayscale(1));
    }
    /*************************************************************************/
    std::vector<transforms_Compose > transformD{
            transforms_ToTensor()  // Mat Image [0,255] or [0,65535] ===> Tensor Image [0,1]
    };
    if (vm["nc"].as<size_t>() == 1) {
        transformD.insert(transformD.begin(), transforms_Grayscale(1));
    }

    // (5) Define Network
    YOLOv3 model(vm);
    model->to(device);

    // (6) Make Directories
    std::string dir = "checkpoints/" + vm["dataset"].as<std::string>();
    fs::create_directories(dir);

    // (7) Save Model Parameters
    Set_Model_Params(vm, model, "YOLOv3");

    // (8) Set Class Names and Configs
    std::vector<std::string> class_names = Set_Class_Names(vm["class_list"].as<std::string>(),
                                                           vm["class_num"].as<size_t>());
    std::vector<std::vector<std::tuple<float, float>>> anchors = Set_Anchors(vm["anchor_list"].as<std::string>(),
                                                                             vm["scales"].as<size_t>(),
                                                                             vm["na"].as<size_t>());
    size_t resize_step_max;
    std::vector<std::tuple<long int, long int>> resizes = Set_Resizes(vm["resize_list"].as<std::string>(),
                                                                      resize_step_max);

    // (9.1) Training Phase
    if (vm["train"].as<bool>()) {
        Set_Options(vm, argc, argv, args, "train");
        train(vm, device, model, transformBB, transformI, class_names, anchors, resizes, resize_step_max);
    }

    // (9.2) Test Phase
    if (vm["test"].as<bool>()) {
        Set_Options(vm, argc, argv, args, "test");
        test(vm, device, model, transformI, class_names, anchors);
    }

    // (9.3) Detection Phase
    if (vm["detect"].as<bool>()) {
        Set_Options(vm, argc, argv, args, "detect");
        detect(vm, device, model, transformI, transformD, class_names, anchors);
    }

    // (9.4) Demo Phase
    if (vm["demo"].as<bool>()) {
        Set_Options(vm, argc, argv, args, "demo");
        demo(vm, device, model, transformI, transformD, class_names, anchors);
    }

    // End Processing
    return 0;

}


// -----------------------------------
// 2. Device Setting Function
// -----------------------------------
torch::Device Set_Device(po::variables_map &vm) {

    // (1) GPU Type
    int gpu_id = vm["gpu_id"].as<int>();
    if (torch::cuda::is_available() && gpu_id >= 0) {
        torch::Device device(torch::kCUDA, gpu_id);
        return device;
    }

    // (2) CPU Type
    torch::Device device(torch::kCPU);
    return device;

}


// -----------------------------------
// 3. Model Parameters Setting Function
// -----------------------------------
template<typename T>
void Set_Model_Params(po::variables_map &vm, T &model, const std::string name) {

    // (1) Make Directory
    std::string dir = "checkpoints/" + vm["dataset"].as<std::string>() + "/model_params/";
    fs::create_directories(dir);

    // (2.1) File Open
    std::string fname = dir + name + ".txt";
    std::ofstream ofs(fname);

    // (2.2) Calculation of Parameters
    size_t num_params = 0;
    for (auto param: model->parameters()) {
        num_params += param.numel();
    }
    ofs << "Total number of parameters : " << (float) num_params / 1e6f << "M" << std::endl << std::endl;
    ofs << model << std::endl;

    // (2.3) File Close
    ofs.close();

    // End Processing
    return;

}


// -----------------------------------
// 4. Class Names Setting Function
// -----------------------------------
std::vector<std::string> Set_Class_Names(const std::string path, const size_t class_num) {

    // (1) Memory Allocation
    std::vector<std::string> class_names = std::vector<std::string>(class_num);

    // (2) Get Class Names
    std::string class_name;
    std::ifstream ifs(path, std::ios::in);
    for (size_t i = 0; i < class_num; i++) {
        if (!getline(ifs, class_name)) {
            std::cerr << "Error : The number of classes does not match the number of lines in the class name file."
                      << std::endl;
            std::exit(1);
        }
        class_names.at(i) = class_name;
    }
    ifs.close();

    // End Processing
    return class_names;

}


// -----------------------------------
// 5. Anchors Setting Function
// -----------------------------------
std::vector<std::vector<std::tuple<float, float>>>
Set_Anchors(const std::string path, const size_t scales, const size_t na) {

    // (1) Memory Allocation
    std::vector<std::vector<std::tuple<float, float>>> anchors = std::vector<std::vector<std::tuple<float, float>>>(
            scales);
    for (size_t i = 0; i < scales; i++) {
        anchors.at(i) = std::vector<std::tuple<float, float>>(na);
    }

    // (2) Get Anchors
    float pw, ph;
    std::string line;
    std::ifstream ifs(path, std::ios::in);
    for (size_t i = 0; i < scales; i++) {
        for (size_t j = 0; j < na; j++) {
            if (!getline(ifs, line)) {
                std::cerr << "Error : The number of anchors does not match the number of lines in the anchor file."
                          << std::endl;
                std::exit(1);
            }
            std::istringstream iss(line);
            iss >> pw;
            iss >> ph;
            anchors.at(i).at(j) = {pw, ph};
        }
    }
    ifs.close();

    // End Processing
    return anchors;

}


// -----------------------------------
// 6. Resizes Setting Function
// -----------------------------------
std::vector<std::tuple<long int, long int>> Set_Resizes(const std::string path, size_t &resize_step_max) {

    // (1) Memory Allocation
    std::vector<std::tuple<long int, long int>> resizes;

    // (2) Get Resizes
    size_t num;
    long int width, height;
    std::string line;
    std::ifstream ifs(path, std::ios::in);
    /**********************************************************************/
    for (size_t i = 0; i < 2; i++) {
        if (!getline(ifs, line)) {
            std::cerr
                    << "Error : The number of configs and resizes does not match the number of lines in the resize file."
                    << std::endl;
            std::exit(1);
        }
        std::istringstream iss(line);
        if (i == 0) {
            iss >> num;
        } else {
            iss >> resize_step_max;
        }
    }
    /**********************************************************************/
    resizes = std::vector<std::tuple<long int, long int>>(num);
    for (size_t i = 0; i < num; i++) {
        if (!getline(ifs, line)) {
            std::cerr
                    << "Error : The number of configs and resizes does not match the number of lines in the resize file."
                    << std::endl;
            std::exit(1);
        }
        std::istringstream iss(line);
        iss >> width;
        iss >> height;
        resizes.at(i) = {width, height};
    }
    ifs.close();

    // End Processing
    return resizes;

}


// -----------------------------------
// 7. Options Setting Function
// -----------------------------------
void Set_Options(po::variables_map &vm, int argc, const char *argv[], po::options_description &args,
                 const std::string mode) {

    // (1) Make Directory
    std::string dir = "checkpoints/" + vm["dataset"].as<std::string>() + "/options/";
    fs::create_directories(dir);

    // (2) Terminal Output
    std::cout << "--------------------------------------------" << std::endl;
    std::cout << args << std::endl;
    std::cout << "--------------------------------------------" << std::endl;

    // (3.1) File Open
    std::string fname = dir + mode + ".txt";
    std::ofstream ofs(fname, std::ios::app);

    // (3.2) Arguments Output
    ofs << "--------------------------------------------" << std::endl;
    ofs << "Command Line Arguments:" << std::endl;
    for (int i = 1; i < argc; i++) {
        if (i % 2 == 1) {
            ofs << "  " << argv[i] << '\t' << std::flush;
        } else {
            ofs << argv[i] << std::endl;
        }
    }
    ofs << "--------------------------------------------" << std::endl;
    ofs << args << std::endl;
    ofs << "--------------------------------------------" << std::endl << std::endl;

    // (3.3) File Close
    ofs.close();

    // End Processing
    return;

}
