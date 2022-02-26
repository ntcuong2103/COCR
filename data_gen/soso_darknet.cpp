#include "soso_darknet.hpp"
#include "mol_op.hpp"
#include "hw_mol.hpp"
#include "isomer.hpp"
#include "std_util.hpp"
//#include "couch_data.hpp"

#include <filesystem>
#include <iostream>
#include <random>

SOSODarknet::SOSODarknet() : isInited(false) {
}

bool SOSODarknet::init(const std::string &_topDir) {
    if (!std::filesystem::exists(_topDir)) {
        if (!std::filesystem::create_directories(_topDir)) {
            std::cerr << "fail to create dir: " << _topDir << std::endl;
            exit(-1);
        }
    }
    imgPath = _topDir + imgDir;
    labelPath = _topDir + labelDir;
    if (std::filesystem::exists(imgPath)) {
        std::filesystem::remove_all(imgPath);
    }
    if (std::filesystem::exists(labelPath)) {
        std::filesystem::remove_all(labelPath);
    }
    std::filesystem::create_directories(imgPath);
    std::filesystem::create_directories(labelPath);
    if (!std::filesystem::is_empty(imgPath) ||
        !std::filesystem::is_empty(labelPath)) {
        std::cerr << "target dir not empty: " << imgPath << " or " << labelPath << std::endl;
        exit(-1);
    }
    isInited = true;
    return true;
}

void SOSODarknet::dump(const size_t &_numOfSamples, const size_t &_repeatTimes) {
    if (!isInited) {
        std::cerr << "you must call SOSODarknet::init before dump data" << std::endl;
        exit(-1);
    }
    auto &isomer = IsomerCounter::GetInstance();
    auto alkanes = isomer.getIsomers({
                                             2, 3, 4, 5,
                                             6, 7,
                                             8, 9, 10, 11, 12, 13,
                                             14, 15, 16
                                     });
    const int loopTime = _numOfSamples / _repeatTimes;
    std::shuffle(alkanes.begin(), alkanes.end(), std::default_random_engine());
//#pragma omp parallel for num_threads(8)
    for (int i = 0; i < loopTime; i++) {
        std::shared_ptr<HwMol> hwMol(nullptr);
        if (byProb(0.1)) {
            hwMol = HwMol::GetSpecialExample(0.1);
        } else {
            auto mol = std::make_shared<JMol>();
            mol->setAlkane(alkanes[i % alkanes.size()]);
            auto molOp = std::make_shared<MolOp>(mol);
            bool add_aro = byProb(0.5), add_com = byProb(0.5);
            if (alkanes[i % alkanes.size()].length() > 40) {
                add_com = false;
            }
            if (alkanes[i % alkanes.size()].length() > 30) {
                add_aro = false;
            }
            molOp->randomize(0.1, byProb(0.95), byProb(0.95),
                             add_aro, add_com);
            hwMol = std::make_shared<HwMol>(molOp);
        }
        if (hwMol) {
            auto idxStr = std::to_string(i);
            auto subDir = "/" + std::to_string(i % 100) + "/";
            if (!std::filesystem::exists(imgPath + subDir)) {
                std::filesystem::create_directory(imgPath + subDir);
            }
            if (!std::filesystem::exists(labelPath + subDir)) {
                std::filesystem::create_directory(labelPath + subDir);
            }
            hwMol->dumpAsDarknet(imgPath + subDir + idxStr,
                                 labelPath + subDir + idxStr, _repeatTimes);
        }
        if (i % 5000 == 0) {
            std::cout << i << std::endl;
        }
    }
}

void SOSODarknet::display() {
    auto &isomer = IsomerCounter::GetInstance();
    auto alkanes = isomer.getIsomers({
                                             2, 3, 4, 5, 6, 7,
                                             8, 9, 10, 11, 12, 13, 14, 15,
//                                             16
                                     });
    const int loopTime = 10000;
    std::shuffle(alkanes.begin(), alkanes.end(), std::default_random_engine());
    for (int i = 0; i < loopTime; i++) {
        std::shared_ptr<HwMol> hwMol(nullptr);
        if (byProb(0.1)) {
            hwMol = HwMol::GetSpecialExample(0.1);
        } else {
            auto mol = std::make_shared<JMol>();
            mol->setAlkane(alkanes[i % alkanes.size()]);
            auto molOp = std::make_shared<MolOp>(mol);
            bool add_aro = byProb(0.5), add_com = byProb(0.5);
            if (alkanes[i % alkanes.size()].length() > 40) {
                add_com = false;
            }
            if (alkanes[i % alkanes.size()].length() > 30) {
                add_aro = false;
            }
            molOp->randomize(0.1, byProb(0.95), byProb(0.95),
                             add_aro, add_com);
            hwMol = std::make_shared<HwMol>(molOp);
        }
        if (hwMol) {
            hwMol->showOnScreen(1, true);
        }
    }
}