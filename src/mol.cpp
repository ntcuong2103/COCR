#include "mol.hpp"
#include "alkane_graph.hpp"
#include "mol_util.hpp"
#include "std_util.hpp"
#include <random>

std::shared_ptr<JAtom> JMol::addAtom(const size_t &_atomicNumber) {
    auto aid = mAids++;
    auto atom = std::make_shared<JAtom>(aid, _atomicNumber);
    atomsMap[aid] = atom;
    return std::move(atom);
}

std::shared_ptr<JBond> JMol::addBond(const size_t &_atomFromId, const size_t &_atomToId,
                                     const JBondType &_bondType) {
    auto bid = mBids++;
    auto bond = std::make_shared<JBond>(bid, _atomFromId, _atomToId, _bondType);
    bondsMap[bid] = bond;
    return std::move(bond);
}

JMol::JMol() {
    mAids = mBids = std::numeric_limits<size_t>::lowest();
}

void JMol::clear() {
    bondsMap.clear();
    atomsMap.clear();
    mAids = mBids = std::numeric_limits<size_t>::lowest();
}

void JMol::safeTraverseBonds(const std::function<void(const size_t &)> &func) const {
    std::vector<size_t> bids;
    bids.reserve(bondsMap.size());
    for (auto&[id, _]:bondsMap) bids.push_back(id);
    for (auto &bid:bids) func(bid);
}

void JMol::setAlkane(const std::string &_alkaneSMILES) {
    clear();
    AlkaneGraph<node_type> alkane;
    alkane.fromSMILES(_alkaneSMILES);
    std::unordered_map<node_type, size_t> a2jAidMap;
    std::unordered_set<std::pair<size_t, size_t>, pair_hash> bMap;
    auto before = [&](const node_type &current, const node_type &from) {
        if (notExist(a2jAidMap, current)) {
            a2jAidMap[current] = addAtom(6)->getId();
        }
        if (from != alkane.EMPTY_NODE && notExist(a2jAidMap, from)) {
            a2jAidMap[from] = addAtom(6)->getId();
        }
        if (from != alkane.EMPTY_NODE) {
            size_t jFrom = a2jAidMap[from], jTo = a2jAidMap[current];
            if (notExist(bMap, std::make_pair(jFrom, jTo))) {
                addBond(jFrom, jTo);
                bMap.insert({jFrom, jTo});
                bMap.insert({jTo, jFrom});
            }
        }
    };
    alkane.dfsWrapper(before, before, before);
}

void JMol::safeTraverseAtoms(const std::function<void(const size_t &)> &func) const {
    std::vector<size_t> aids;
    aids.reserve(atomsMap.size());
    for (auto&[id, _]:atomsMap) aids.push_back(id);
    for (auto &aid:aids) func(aid);
}

std::shared_ptr<JAtom> JMol::getAtomById(const size_t &_aid) const {
    auto itr = atomsMap.find(_aid);
    if (itr == atomsMap.end()) return nullptr;
    return itr->second;
}

std::shared_ptr<JBond> JMol::getBondById(const size_t &_bid) const {
    auto itr = bondsMap.find(_bid);
    if (itr == bondsMap.end()) return nullptr;
    return itr->second;
}

bool JMol::IsBondsEmpty() const {
    return bondsMap.empty();
}

size_t JMol::bondsNum() const {
    return bondsMap.size();
}

size_t JMol::atomsNum() const {
    return atomsMap.size();
}

JMol::JMol(const JMol &_mol)
        : mAids(_mol.mAids), mBids(_mol.mBids),
          atomsMap(_mol.atomsMap), bondsMap(_mol.bondsMap) {
}

JMol::JMol(JMol &&_mol)
        : mAids(_mol.mAids), mBids(_mol.mBids),
          atomsMap(std::move(_mol.atomsMap)), bondsMap(std::move(_mol.bondsMap)) {
}

std::shared_ptr<JMol> JMol::clone() const {
    return std::make_shared<JMol>(*this);
}

void JMol::safeTraverseBondsBreakIf(const std::function<bool(const size_t &)> &func) const {
    std::vector<size_t> bids;
    bids.reserve(bondsMap.size());
    for (auto&[id, _]:bondsMap) bids.push_back(id);
    for (auto &bid:bids) {
        if (!func(bid))return;
    }
}

void JMol::safeTraverseAtomsBreakIf(const std::function<bool(const size_t &)> &func) const {
    std::vector<size_t> aids;
    aids.reserve(atomsMap.size());
    for (auto&[id, _]:atomsMap) aids.push_back(id);
    for (auto &aid:aids) {
        if (!func(aid))return;
    }
}

void JMol::removeBond(const size_t &_bid) {
    bondsMap.erase(_bid);
}

std::shared_ptr<JAtom> JMol::addAtom(const ElementType &_elementType) {
    return addAtom(static_cast<size_t>(_elementType));
}
