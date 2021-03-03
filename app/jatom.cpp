#include "jatom.hpp"
#include <vector>
#include <string>
#include <unordered_map>

using xgd::ElementType;

std::vector<std::string> xgd::ELEMENT_NAME_LIST = {
        "None", "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K",
        "Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr",
        "Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La",
        "Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os",
        "Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am",
        "Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg", "Cn"
};

std::unordered_map<std::string, ElementType> xgd::STR_ELEMENT_SET = {
        {"None", ElementType::None},
        {"H",    ElementType::H},
        {"He",   ElementType::He},
        {"Li",   ElementType::Li},
        {"Be",   ElementType::Be},
        {"B",    ElementType::B},
        {"C",    ElementType::C},
        {"N",    ElementType::N},
        {"O",    ElementType::O},
        {"F",    ElementType::F},
        {"Ne",   ElementType::Ne},
        {"Na",   ElementType::Na},
        {"Mg",   ElementType::Mg},
        {"Al",   ElementType::Al},
        {"Si",   ElementType::Si},
        {"P",    ElementType::P},
        {"S",    ElementType::S},
        {"Cl",   ElementType::Cl},
        {"Ar",   ElementType::Ar},
        {"K",    ElementType::K},
        {"Ca",   ElementType::Ca},
        {"Sc",   ElementType::Sc},
        {"Ti",   ElementType::Ti},
        {"V",    ElementType::V},
        {"Cr",   ElementType::Cr},
        {"Mn",   ElementType::Mn},
        {"Fe",   ElementType::Fe},
        {"Co",   ElementType::Co},
        {"Ni",   ElementType::Ni},
        {"Cu",   ElementType::Cu},
        {"Zn",   ElementType::Zn},
        {"Ga",   ElementType::Ga},
        {"Ge",   ElementType::Ge},
        {"As",   ElementType::As},
        {"Se",   ElementType::Se},
        {"Br",   ElementType::Br},
        {"Kr",   ElementType::Kr},
        {"Rb",   ElementType::Rb},
        {"Sr",   ElementType::Sr},
        {"Y",    ElementType::Y},
        {"Zr",   ElementType::Zr},
        {"Nb",   ElementType::Nb},
        {"Mo",   ElementType::Mo},
        {"Tc",   ElementType::Tc},
        {"Ru",   ElementType::Ru},
        {"Rh",   ElementType::Rh},
        {"Pd",   ElementType::Pd},
        {"Ag",   ElementType::Ag},
        {"Cd",   ElementType::Cd},
        {"In",   ElementType::In},
        {"Sn",   ElementType::Sn},
        {"Sb",   ElementType::Sb},
        {"Te",   ElementType::Te},
        {"I",    ElementType::I},
        {"Xe",   ElementType::Xe},
        {"Cs",   ElementType::Cs},
        {"Ba",   ElementType::Ba},
        {"La",   ElementType::La},
        {"Ce",   ElementType::Ce},
        {"Pr",   ElementType::Pr},
        {"Nd",   ElementType::Nd},
        {"Pm",   ElementType::Pm},
        {"Sm",   ElementType::Sm},
        {"Eu",   ElementType::Eu},
        {"Gd",   ElementType::Gd},
        {"Tb",   ElementType::Tb},
        {"Dy",   ElementType::Dy},
        {"Ho",   ElementType::Ho},
        {"Er",   ElementType::Er},
        {"Tm",   ElementType::Tm},
        {"Yb",   ElementType::Yb},
        {"Lu",   ElementType::Lu},
        {"Hf",   ElementType::Hf},
        {"Ta",   ElementType::Ta},
        {"W",    ElementType::W},
        {"Re",   ElementType::Re},
        {"Os",   ElementType::Os},
        {"Ir",   ElementType::Ir},
        {"Pt",   ElementType::Pt},
        {"Au",   ElementType::Au},
        {"Hg",   ElementType::Hg},
        {"Tl",   ElementType::Tl},
        {"Pb",   ElementType::Pb},
        {"Bi",   ElementType::Bi},
        {"Po",   ElementType::Po},
        {"At",   ElementType::At},
        {"Rn",   ElementType::Rn},
        {"Fr",   ElementType::Fr},
        {"Ra",   ElementType::Ra},
        {"Ac",   ElementType::Ac},
        {"Th",   ElementType::Th},
        {"Pa",   ElementType::Pa},
        {"U",    ElementType::U},
        {"Np",   ElementType::Np},
        {"Pu",   ElementType::Pu},
        {"Am",   ElementType::Am},
        {"Cm",   ElementType::Cm},
        {"Bk",   ElementType::Bk},
        {"Cf",   ElementType::Cf},
        {"Es",   ElementType::Es},
        {"Fm",   ElementType::Fm},
        {"Md",   ElementType::Md},
        {"No",   ElementType::No},
        {"Lr",   ElementType::Lr},
        {"Rf",   ElementType::Rf},
        {"Db",   ElementType::Db},
        {"Sg",   ElementType::Sg},
        {"Bh",   ElementType::Bh},
        {"Hs",   ElementType::Hs},
        {"Mt",   ElementType::Mt},
        {"Ds",   ElementType::Ds},
        {"Rg",   ElementType::Rg},
        {"Cn",   ElementType::Cn}
};

const std::string &xgd::convertElementTypeToName(const ElementType &_type) {
    return ELEMENT_NAME_LIST[static_cast<size_t>(_type)];
}

ElementType xgd::convertNameToElementType(const std::string &_name) {
    auto it = STR_ELEMENT_SET.find(_name);
    if (it != STR_ELEMENT_SET.end())return it->second;
    return ElementType::None;
}

const std::string &xgd::JAtom::getName() const {
    return convertElementTypeToName(type);
}

const ElementType &xgd::JAtom::getType() const {
    return type;
}

void xgd::JAtom::set2D(const float &_x, const float &_y) {
    x = _x;
    y = _y;
}

void xgd::JAtom::set3D(const float &_x, const float &_y, const float &_z) {
    xx = _x;
    yy = _y;
    zz = _z;
}

int xgd::JAtom::getCharge() const {
    return charge;
}

size_t xgd::JAtom::getId() const {
    return id;
}

xgd::JAtom::JAtom(const size_t &_id, const ElementType &_element, const float &_x, const float &_y)
        : id(_id), type(_element), x(_x), y(_y), xx(0), yy(0), zz(0), charge(0) {

}

int xgd::JAtom::getAtomicNumber() const {
    return static_cast<size_t>(getType());
}