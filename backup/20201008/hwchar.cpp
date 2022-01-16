//
// Created by xgd on 2020/9/18.
//

#include "hwchar.hpp"
#include <fstream>

bool HWChar::sIsLoaded = false;

std::vector<std::vector<Script>> HWChar::sData;

std::vector<int>HWChar::sLabelCharLikeLine = {
        8, 24, 53,
        130, 134, 141, 146, 158,
        172
};

std::vector<int>HWChar::sLabelCharLikeCircle = {9, 21, 47};

HWChar::HWChar(int label) {
    if (!sIsLoaded) {
        LoadDataSet(get_couch_data_path());
        if (!sIsLoaded) {
            std::cerr << "fail to load data from " << get_couch_data_path() << std::endl;
            exit(-1);
        }
    }
    if (label >= sData.size()) {
        std::cerr << "label " << label <<
                  "out of boundary: [0," << sData.size() << ")" << std::endl;
        exit(-1);
    }
    mLabel = label;
    auto &scripts = sData[mLabel];
    int tmp = rand() % scripts.size();
    if (mLabel == 43) {
        while (tmp == 43) {// 补丁：这个字符有问题
            tmp = rand() % scripts.size();
        }
    }
    mData = scripts[tmp];
}

std::map<std::string, int> HWChar::sStr2IntMap = {
        {"!",  183},
        {"Ⅴ",  182},
        {"\"", 181},
        {"Ⅳ",  180},
        {"θ",  179},
        {"#",  178},
        {"Ⅲ",  177},
        {"￥",  176},
        {"%",  173},
        {"ξ",  174},
        {"Ⅱ",  175},
        {"&",  171},
        {"Ⅰ",  172},
        {"`",  169},
        {"μ",  170},
        {"(",  167},
        {"γ",  168},
        {")",  165},
        {"β",  166},
        {"*",  163},
        {"α",  164},
        {"+",  162},
        {",",  160},
        {"η",  161},
        {"-",  158},
        {"ζ",  159},
        {".",  156},
        {"ε",  157},
        {"/",  154},
        {"δ",  155},
        {":",  152},
        {"ρ",  153},
        {";",  149},
        {"{",  150},
        {"π",  151},
        {"<",  145},
        {"|",  146},
        {"÷",  147},
        {"ψ",  148},
        {"=",  143},
        {"}",  144},
        {">",  140},
        {"￣",  141},
        {"φ",  142},
        {"?",  139},
        {"@",  138},
        {"[",  136},
        {"Π",  137},
        {"\\", 134},
        {"Ψ",  135},
        {"]",  133},
        {"^",  131},
        {"Φ",  132},
        {"_",  130},
        {"｀",  128},
        {"λ",  129},
        {"《",  127},
        {"》",  126},
        {"$",  124},
        {"Σ",  125},
        {"Ω",  123},
        {"σ",  122},
        {"τ",  121},
        {"ω",  120},
        {"Ⅵ",  119},
        {"Ⅶ",  118},
        {"Ⅷ",  117},
        {"Ⅸ",  116},
        {"Ⅹ",  115},
        {"Ⅺ",  114},
        {"Ⅻ",  113},
        {"ⅰ",  112},
        {"ⅱ",  111},
        {"ⅲ",  110},
        {"ⅳ",  109},
        {"ⅴ",  108},
        {"ⅵ",  107},
        {"ⅶ",  106},
        {"ⅷ",  105},
        {"ⅸ",  104},
        {"ⅹ",  103},
        {"←",  102},
        {"↑",  101},
        {"→",  100},
        {"↓",  99},
        {"↖",  98},
        {"↗",  97},
        {"↘",  96},
        {"↙",  95},
        {"∈",  94},
        {"√",  93},
        {"∞",  92},
        {"∫",  91},
        {"∮",  90},
        {"∴",  89},
        {"∵",  88},
        {"≈",  87},
        {"≌",  86},
        {"≠",  85},
        {"≤",  84},
        {"≥",  83},
        {"⊕",  81},
        {"7",  82},
        {"⊙",  80},
        {"±",  79},
        {"①",  78},
        {"②",  77},
        {"③",  76},
        {"④",  75},
        {"⑤",  74},
        {"⑥",  73},
        {"⑦",  72},
        {"⑧",  71},
        {"⑨",  70},
        {"⑩",  69},
        {"￡",  68},
        {"、",  67},
        {"‘",  66},
        {"’",  65},
        {"“",  64},
        {"”",  63},
        {"。",  62},
        {"A",  61},
        {"B",  60},
        {"C",  59},
        {"D",  58},
        {"E",  57},
        {"F",  56},
        {"G",  55},
        {"H",  54},
        {"I",  53},
        {"J",  52},
        {"K",  51},
        {"L",  50},
        {"M",  49},
        {"N",  48},
        {"O",  47},
        {"P",  46},
        {"Q",  45},
        {"R",  44},
        {"S",  43},
        {"T",  42},
        {"U",  41},
        {"V",  40},
        {"W",  39},
        {"X",  38},
        {"Y",  37},
        {"Z",  36},
        {"a",  35},
        {"b",  34},
        {"c",  33},
        {"d",  32},
        {"e",  31},
        {"f",  30},
        {"g",  29},
        {"h",  28},
        {"i",  27},
        {"j",  26},
        {"k",  25},
        {"l",  24},
        {"m",  23},
        {"n",  22},
        {"o",  21},
        {"p",  20},
        {"q",  19},
        {"r",  18},
        {"s",  17},
        {"t",  16},
        {"u",  15},
        {"v",  14},
        {"w",  13},
        {"x",  12},
        {"y",  11},
        {"z",  10},
        {"0",  9},
        {"1",  8},
        {"2",  7},
        {"3",  6},
        {"4",  5},
        {"5",  4},
        {"6",  3},
        {"8",  2},
        {"9",  1}
};

bool HWChar::LoadDataSet(const char *filename, bool clearBefore) {
    if (clearBefore) {
        sData.clear();
    }
    int a, b, c, d, e;
    std::ifstream ifsm(filename, std::ios::in | std::ios::binary);
    if (!ifsm.is_open()) {
        std::cerr << "fail to open " << filename << std::endl;
        sIsLoaded = false;
        return sIsLoaded;
    }
    // 【标签索引】、【宽度】、【长度】、【笔画数目】
    while (ifsm.read(reinterpret_cast<char *>(&a), 4)) {
        ifsm.read(reinterpret_cast<char *>(&b), 4);
        ifsm.read(reinterpret_cast<char *>(&c), 4);
        ifsm.read(reinterpret_cast<char *>(&d), 4);
        Script script;
        script.resize(d);
        for (int i = 0; i < d; i++) {
            // 【点数】
            ifsm.read(reinterpret_cast<char *>(&e), 4);
            script[i].resize(e);
            for (int j = 0; j < e; j++) {
                ifsm.read(reinterpret_cast<char *>(&script[i][j].x), 4);
                ifsm.read(reinterpret_cast<char *>(&script[i][j].y), 4);
            }
        }
        if (a >= sData.size()) {
            sData.resize(a + 1);
        }
        sData[a].emplace_back(std::move(script));
    }
    for (auto &dat:sData) {
        dat.resize(dat.size());
    }
    sIsLoaded = true;
    return sIsLoaded;
}