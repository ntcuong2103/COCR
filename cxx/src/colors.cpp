#include "colors.hpp"

static const std::vector<std::tuple<unsigned char, unsigned char, unsigned char>>
        predefinedColors{
        {255, 250, 250},
        {248, 248, 255},
        {245, 245, 245},
        {220, 220, 220},
        {255, 250, 240},
        {253, 245, 230},
        {250, 240, 230},
        {250, 235, 215},
        {255, 239, 213},
        {255, 235, 205},
        {255, 228, 196},
        {255, 218, 185},
        {255, 222, 173},
        {255, 228, 181},
        {255, 248, 220},
        {255, 255, 240},
        {255, 250, 205},
        {255, 245, 238},
        {240, 255, 240},
        {245, 255, 250},
        {240, 255, 255},
        {240, 248, 255},
        {230, 230, 250},
        {255, 240, 245},
        {255, 228, 225},
        {255, 255, 255},
        {0,   0,   0},
        {47,  79,  79},
        {105, 105, 105},
        {112, 128, 144},
        {119, 136, 153},
        {190, 190, 190},
        {211, 211, 211},
        {25,  25,  112},
        {0,   0,   128},
        {100, 149, 237},
        {72,  61,  139},
        {106, 90,  205},
        {123, 104, 238},
        {132, 112, 255},
        {0,   0,   205},
        {65,  105, 225},
        {0,   0,   255},
        {30,  144, 255},
        {0,   191, 255},
        {135, 206, 235},
        {135, 206, 250},
        {70,  130, 180},
        {176, 196, 222},
        {173, 216, 230},
        {176, 224, 230},
        {175, 238, 238},
        {0,   206, 209},
        {72,  209, 204},
        {64,  224, 208},
        {0,   255, 255},
        {224, 255, 255},
        {95,  158, 160},
        {102, 205, 170},
        {127, 255, 212},
        {0,   100, 0},
        {85,  107, 47},
        {143, 188, 143},
        {46,  139, 87},
        {60,  179, 113},
        {32,  178, 170},
        {152, 251, 152},
        {0,   255, 127},
        {124, 252, 0},
        {0,   255, 0},
        {127, 255, 0},
        {0,   250, 154},
        {173, 255, 47},
        {50,  205, 50},
        {154, 205, 50},
        {34,  139, 34},
        {107, 142, 35},
        {189, 183, 107},
        {238, 232, 170},
        {250, 250, 210},
        {255, 255, 224},
        {255, 255, 0},
        {255, 215, 0},
        {238, 221, 130},
        {218, 165, 32},
        {184, 134, 11},
        {188, 143, 143},
        {205, 92,  92},
        {139, 69,  19},
        {160, 82,  45},
        {205, 133, 63},
        {222, 184, 135},
        {245, 245, 220},
        {245, 222, 179},
        {244, 164, 96},
        {210, 180, 140},
        {210, 105, 30},
        {178, 34,  34},
        {165, 42,  42},
        {233, 150, 122},
        {250, 128, 114},
        {255, 160, 122},
        {255, 165, 0},
        {255, 140, 0},
        {255, 127, 80},
        {240, 128, 128},
        {255, 99,  71},
        {255, 69,  0},
        {255, 0,   0},
        {255, 105, 180},
        {255, 20,  147},
        {255, 192, 203},
        {255, 182, 193},
        {219, 112, 147},
        {176, 48,  96},
        {199, 21,  133},
        {208, 32,  144},
        {255, 0,   255},
        {238, 130, 238},
        {221, 160, 221},
        {218, 112, 214},
        {186, 85,  211},
        {153, 50,  204},
        {148, 0,   211},
        {138, 43,  226},
        {160, 32,  240},
        {147, 112, 219},
        {216, 191, 216},
        {255, 250, 250},
        {238, 233, 233},
        {205, 201, 201},
        {139, 137, 137},
        {255, 245, 238},
        {238, 229, 222},
        {205, 197, 191},
        {139, 134, 130},
        {255, 239, 219},
        {238, 223, 204},
        {205, 192, 176},
        {139, 131, 120},
        {255, 228, 196},
        {238, 213, 183},
        {205, 183, 158},
        {139, 125, 107},
        {255, 218, 185},
        {238, 203, 173},
        {205, 175, 149},
        {139, 119, 101},
        {255, 222, 173},
        {238, 207, 161},
        {205, 179, 139},
        {139, 121, 94},
        {255, 250, 205},
        {238, 233, 191},
        {205, 201, 165},
        {139, 137, 112},
        {255, 248, 220},
        {238, 232, 205},
        {205, 200, 177},
        {139, 136, 120},
        {255, 255, 240},
        {238, 238, 224},
        {205, 205, 193},
        {139, 139, 131},
        {240, 255, 240},
        {224, 238, 224},
        {193, 205, 193},
        {131, 139, 131},
        {255, 240, 245},
        {238, 224, 229},
        {205, 193, 197},
        {139, 131, 134},
        {255, 228, 225},
        {238, 213, 210},
        {205, 183, 181},
        {139, 125, 123},
        {240, 255, 255},
        {224, 238, 238},
        {193, 205, 205},
        {131, 139, 139},
        {131, 111, 255},
        {122, 103, 238},
        {105, 89,  205},
        {71,  60,  139},
        {72,  118, 255},
        {67,  110, 238},
        {58,  95,  205},
        {39,  64,  139},
        {0,   0,   255},
        {0,   0,   238},
        {0,   0,   205},
        {0,   0,   139},
        {30,  144, 255},
        {28,  134, 238},
        {24,  116, 205},
        {16,  78,  139},
        {99,  184, 255},
        {92,  172, 238},
        {79,  148, 205},
        {54,  100, 139},
        {0,   191, 255},
        {0,   178, 238},
        {0,   154, 205},
        {0,   104, 139},
        {135, 206, 255},
        {126, 192, 238},
        {108, 166, 205},
        {74,  112, 139},
        {176, 226, 255},
        {164, 211, 238},
        {141, 182, 205},
        {96,  123, 139},
        {198, 226, 255},
        {185, 211, 238},
        {159, 182, 205},
        {108, 123, 139},
        {202, 225, 255},
        {188, 210, 238},
        {162, 181, 205},
        {110, 123, 139},
        {191, 239, 255},
        {178, 223, 238},
        {154, 192, 205},
        {104, 131, 139},
        {224, 255, 255},
        {209, 238, 238},
        {180, 205, 205},
        {122, 139, 139},
        {187, 255, 255},
        {174, 238, 238},
        {150, 205, 205},
        {102, 139, 139},
        {152, 245, 255},
        {142, 229, 238},
        {122, 197, 205},
        {83,  134, 139},
        {0,   245, 255},
        {0,   229, 238},
        {0,   197, 205},
        {0,   134, 139},
        {0,   255, 255},
        {0,   238, 238},
        {0,   205, 205},
        {0,   139, 139},
        {151, 255, 255},
        {141, 238, 238},
        {121, 205, 205},
        {82,  139, 139},
        {127, 255, 212},
        {118, 238, 198},
        {102, 205, 170},
        {69,  139, 116},
        {193, 255, 193},
        {180, 238, 180},
        {155, 205, 155},
        {105, 139, 105},
        {84,  255, 159},
        {78,  238, 148},
        {67,  205, 128},
        {46,  139, 87},
        {154, 255, 154},
        {144, 238, 144},
        {124, 205, 124},
        {84,  139, 84},
        {0,   255, 127},
        {0,   238, 118},
        {0,   205, 102},
        {0,   139, 69},
        {0,   255, 0},
        {0,   238, 0},
        {0,   205, 0},
        {0,   139, 0},
        {127, 255, 0},
        {118, 238, 0},
        {102, 205, 0},
        {69,  139, 0},
        {192, 255, 62},
        {179, 238, 58},
        {154, 205, 50},
        {105, 139, 34},
        {202, 255, 112},
        {188, 238, 104},
        {162, 205, 90},
        {110, 139, 61},
        {255, 246, 143},
        {238, 230, 133},
        {205, 198, 115},
        {139, 134, 78},
        {255, 236, 139},
        {238, 220, 130},
        {205, 190, 112},
        {139, 129, 76},
        {255, 255, 224},
        {238, 238, 209},
        {205, 205, 180},
        {139, 139, 122},
        {255, 255, 0},
        {238, 238, 0},
        {205, 205, 0},
        {139, 139, 0},
        {255, 215, 0},
        {238, 201, 0},
        {205, 173, 0},
        {139, 117, 0},
        {255, 193, 37},
        {238, 180, 34},
        {205, 155, 29},
        {139, 105, 20},
        {255, 185, 15},
        {238, 173, 14},
        {205, 149, 12},
        {139, 101, 8},
        {255, 193, 193},
        {238, 180, 180},
        {205, 155, 155},
        {139, 105, 105},
        {255, 106, 106},
        {238, 99,  99},
        {205, 85,  85},
        {139, 58,  58},
        {255, 130, 71},
        {238, 121, 66},
        {205, 104, 57},
        {139, 71,  38},
        {255, 211, 155},
        {238, 197, 145},
        {205, 170, 125},
        {139, 115, 85},
        {255, 231, 186},
        {238, 216, 174},
        {205, 186, 150},
        {139, 126, 102},
        {255, 165, 79},
        {238, 154, 73},
        {205, 133, 63},
        {139, 90,  43},
        {255, 127, 36},
        {238, 118, 33},
        {205, 102, 29},
        {139, 69,  19},
        {255, 48,  48},
        {238, 44,  44},
        {205, 38,  38},
        {139, 26,  26},
        {255, 64,  64},
        {238, 59,  59},
        {205, 51,  51},
        {139, 35,  35},
        {255, 140, 105},
        {238, 130, 98},
        {205, 112, 84},
        {139, 76,  57},
        {255, 160, 122},
        {238, 149, 114},
        {205, 129, 98},
        {139, 87,  66},
        {255, 165, 0},
        {238, 154, 0},
        {205, 133, 0},
        {139, 90,  0},
        {255, 127, 0},
        {238, 118, 0},
        {205, 102, 0},
        {139, 69,  0},
        {255, 114, 86},
        {238, 106, 80},
        {205, 91,  69},
        {139, 62,  47},
        {255, 99,  71},
        {238, 92,  66},
        {205, 79,  57},
        {139, 54,  38},
        {255, 69,  0},
        {238, 64,  0},
        {205, 55,  0},
        {139, 37,  0},
        {255, 0,   0},
        {238, 0,   0},
        {205, 0,   0},
        {139, 0,   0},
        {255, 20,  147},
        {238, 18,  137},
        {205, 16,  118},
        {139, 10,  80},
        {255, 110, 180},
        {238, 106, 167},
        {205, 96,  144},
        {139, 58,  98},
        {255, 181, 197},
        {238, 169, 184},
        {205, 145, 158},
        {139, 99,  108},
        {255, 174, 185},
        {238, 162, 173},
        {205, 140, 149},
        {139, 95,  101},
        {255, 130, 171},
        {238, 121, 159},
        {205, 104, 137},
        {139, 71,  93},
        {255, 52,  179},
        {238, 48,  167},
        {205, 41,  144},
        {139, 28,  98},
        {255, 62,  150},
        {238, 58,  140},
        {205, 50,  120},
        {139, 34,  82},
        {255, 0,   255},
        {238, 0,   238},
        {205, 0,   205},
        {139, 0,   139},
        {255, 131, 250},
        {238, 122, 233},
        {205, 105, 201},
        {139, 71,  137},
        {255, 187, 255},
        {238, 174, 238},
        {205, 150, 205},
        {139, 102, 139},
        {224, 102, 255},
        {209, 95,  238},
        {180, 82,  205},
        {122, 55,  139},
        {191, 62,  255},
        {178, 58,  238},
        {154, 50,  205},
        {104, 34,  139},
        {155, 48,  255},
        {145, 44,  238},
        {125, 38,  205},
        {85,  26,  139},
        {171, 130, 255},
        {159, 121, 238},
        {137, 104, 205},
        {93,  71,  139},
        {255, 225, 255},
        {238, 210, 238},
        {205, 181, 205},
        {139, 123, 139},
        {28,  28,  28},
        {54,  54,  54},
        {79,  79,  79},
        {105, 105, 105},
        {130, 130, 130},
        {156, 156, 156},
        {181, 181, 181},
        {207, 207, 207},
        {232, 232, 232},
        {169, 169, 169},
        {0,   0,   139},
        {0,   139, 139},
        {139, 0,   139},
        {139, 0,   0},
        {144, 238, 144}
};
#ifdef QT_GUI_LIB

QColor getQColor(const ColorName &_colorName) {
    auto&[r, g, b]=predefinedColors[static_cast<int>(_colorName)];
    return QColor(r, g, b);
}

#endif

//#include <iostream>
cv::Scalar getScalar(const ColorName &_colorName) {
    // debug 遗址：注意静态变量初始化依赖问题 qwq
//    std::cout << predefinedColors.size() << std::endl;
//    std::cout << static_cast<int>(ColorName::rgbLightGreen) << std::endl;
//    exit(-1);
    auto&[r, g, b]=predefinedColors[static_cast<int>(_colorName)];
    return cv::Scalar(b, g, r);
}