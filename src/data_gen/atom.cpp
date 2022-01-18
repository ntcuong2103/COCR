#include "atom.hpp"
#include <iostream>
#include <algorithm>

const std::string &JAtom::getElementName() const {
    return convertElementTypeToString(elementType);
}

void JAtom::setElementType(const ElementType &_elementType) {
    elementType = _elementType;
}

void JAtom::setElementType(const std::string &_elementName) {
    auto it = std::find(ElementsData.begin(), ElementsData.end(), _elementName);
    if (ElementsData.end() == it) {
        std::cerr << _elementName << "not found: In JAtom::setElementType" << std::endl;
        exit(-1);
    }
    elementType = static_cast<ElementType>(std::distance(ElementsData.begin(), it));
}

void JAtom::setElementType(const size_t &_atomicNumber) {
    elementType = static_cast<ElementType>(_atomicNumber);
}

const ElementType &JAtom::getElementType() const {
    return elementType;
}

size_t JAtom::getAtomicNumber() const {
    return static_cast<size_t>(elementType);
}

const size_t &JAtom::getId() const {
    return id;
}

void JAtom::setId(const size_t &_id) {
    id = _id;
}

JAtom::JAtom() : x(-1), y(-1), z(-1) {

}

JAtom::JAtom(const size_t &_id, const size_t &_atomicNumber) : x(-1), y(-1), z(-1), id(_id) {
    setElementType(_atomicNumber);
}

const AtomStereo &JAtom::getAtomStereo() const {
    return atomStereo;
}

void JAtom::setAtomStereo(const AtomStereo &_atomStereo) {
    atomStereo = _atomStereo;
}

bool JAtom::isCoord2dEmbedded() const {
    return std::fabs(x + 1) > 0.001 || std::fabs(y + 1) > 0.001;
}

void JAtom::setCoord2d(const float &_x, const float &_y) {
    x = _x;
    y = _y;
    z = -1;
}