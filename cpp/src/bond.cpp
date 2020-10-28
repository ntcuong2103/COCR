#include "bond.hpp"

JBond::JBond( const size_t &_atomFromId, const size_t &_atomToId)
        : atomFrom(_atomFromId), atomTo(_atomToId) {
}

void JBond::setType(const JBondType &_bondType) {
    bondType = _bondType;
}
