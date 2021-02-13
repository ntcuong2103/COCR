#ifndef _HW_STR_HPP_
#define _HW_STR_HPP_

#include "hw_item.hpp"

enum class HwCharType {
    Normal,
    RightDown,
    RightUp
};

enum class HwSpecText {
    PositiveElec,
    NegativeElec,
    Blank,
    LineLeftBracket,
    LineRightBracket,
    TripleHorizontalLine
};

/**
 * 数据流：
 * 输入：std::vector<std::string>
 * 处理：根据 <s1,s2,...,sn> 的 si 分发类型，存在于 sTextMap 的，
 * 非空格走 GetSpecText，然后 asScript 转 HwScript，空格单独处理
 * 不存在于 sTextMap 的直接构造 HwScript，
 * 维护一个字符串的水平游标，空格直接移动游标，非空格嵌入字符并移动游标
 */
class HwStr : public HwItem {
    std::vector<std::pair<std::string, HwCharType>> richText;
    DetectorClasses label;
    inline static const float
            centHeight = 40, offset = 5, offsetW = 3, maxHeight = centHeight + 2 * offset,
            subCentHeight = 20, subOffset = 5, subOffsetW = 2, subMaxHeight = subCentHeight + 2 * subOffset,
            subStartMinRatio = 0.5,
            randAngle = 3;
    inline static std::unordered_map<std::string, HwSpecText> sTextMap = {
            {POS_CHARGE_TEXT, HwSpecText::PositiveElec},
            {NEG_CHARGE_TEXT, HwSpecText::NegativeElec},
            {" ",             HwSpecText::Blank},// 空格
            {"(",             HwSpecText::LineLeftBracket},
            {")",             HwSpecText::LineRightBracket},
            {"#",             HwSpecText::TripleHorizontalLine},
    };


    HwCharType decideHwCharType(const std::string &_text) const;

    float floatX;

    void pushBlank();

    void pushHwData(HwScript &_hwScript, const HwCharType &_hwCharType);

    void pushSpecText(const HwSpecText &_specText, const HwCharType &_hwCharType);

    /**
     * 向后添加一个带属性标注的字符
     * @param _charStr 变长字符，有的字符的长度超过了char
     * @param _hwCharType 字符属性
     */
    void pushCommon(const std::string &_text,
                    const HwCharType &_hwCharType = HwCharType::Normal);

public:
    DetectorClasses getItemType() const override;

    static std::shared_ptr<HwItem> GetSpecText(const HwSpecText &_specText);

    HwStr();

    HwStr(const ElementType &_elementType);

    std::string getShownText() const;

    void loadRichText(const std::vector<std::string> &_unlabeledText);

    void loadRichACSII(const std::string &_text);

    void loadElement(const ElementType &_elementType);

    void loadPlainText(const std::string &_plainText);
};

#endif//_HW_STR_HPP_
