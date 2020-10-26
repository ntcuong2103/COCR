#include "config.hpp"
#include "generator.hpp"
#include "isomer.hpp"
#include <iostream>

int main(int argc, char **argv) {
    const char *alkane_dir = "C:/Users/xgd/source/COCR/data/alkane";
//    // 所有异构体的结构生成
//    auto &IC = IsomerCounter::GetInstance();
//    // 1-27碳，总时间 ~ 4h，
//    IC.calculate(27, alkane_dir);
//    // 多线程方案：28碳，小新pro13，i7，kubuntu，4h，恰好占满15G内存
//    IC.calculate_i_from_i_1(alkane_dir, 28);
    // 读取烷烃异构体并生成SMILES串、添加杂原子和基团修饰
    auto &sg = SMILESGenerator::GetInstance();
    for (int i = 12; i <= 12; i++) {
        sg.HashToRichSMILES(alkane_dir, i);
    }
    return 0;

}
