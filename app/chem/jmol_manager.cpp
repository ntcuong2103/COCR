#include "jmol_manager.hpp"
#include "jmol_adapter.hpp"

using namespace xgd;

JMolManager::JMolManager(JMolManager::mol_type _inputMol) :
        inputMol(std::move(_inputMol)), currentMol(_inputMol), expandedMol(nullptr),
        fullHydrogenExpandedMol(nullptr), fullHydrogenInputMol(nullptr) {

}

void JMolManager::setInputMol(JMolManager::mol_type _inputMol) {
    currentMol = inputMol = std::move(_inputMol);
    expandedMol = fullHydrogenExpandedMol = fullHydrogenInputMol = nullptr;
}

JMolManager::mol_type JMolManager::getInputMol() {
    currentMol = inputMol;
    return inputMol;
}

JMolManager::mol_type JMolManager::getFullHydrogenInputMol() {
    if (!fullHydrogenInputMol) {
        fullHydrogenInputMol = inputMol->deepClone();
        fullHydrogenInputMol->addAllHydrogens();
    }
    currentMol = fullHydrogenInputMol;
    return fullHydrogenInputMol;
}


JMolManager::mol_type JMolManager::getFullHydrogenExpandedMol() {
    if (!fullHydrogenExpandedMol) {
        fullHydrogenExpandedMol = getExpandedMol()->deepClone();
        fullHydrogenExpandedMol->addAllHydrogens();
    }
    currentMol = fullHydrogenExpandedMol;
    return fullHydrogenExpandedMol;
}

JMolManager::mol_type JMolManager::getExpandedMol() {
    if (!expandedMol) {
        expandedMol = inputMol->deepClone();
        expandedMol->tryExpand();
    }
    currentMol = expandedMol;
    return expandedMol;
}

JMolManager &JMolManager::GetInstance() {
    static JMolManager manager(nullptr);
    return manager;
}

JMolManager::mol_type JMolManager::getCurrentMol() {
    if (!currentMol) {
        currentMol = inputMol;
    }
    return currentMol;
}
