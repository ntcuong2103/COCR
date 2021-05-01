#ifndef _XGD_TEXT_CORRECTION_HPP_
#define _XGD_TEXT_CORRECTION_HPP_

#include <string>
#include <vector>
#include <unordered_map>

namespace xgd {
    class TextCorrector {
        inline static std::string ALPHABET = "-=#+_()0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabdefghijnqrty";


    private:
        inline static std::unordered_map<std::string, std::string> c2Map;
        inline static std::unordered_multimap<char, char> ccMap;
        static std::vector<std::string> similarChar;

        const std::string &correct2(const std::string &_text);

    public:
        static const std::string &GetAlphabet();

        static void InitData();

        std::string correct(const std::string &_text);
    };
}
#endif//_XGD_TEXT_CORRECTION_HPP_