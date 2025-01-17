#include "obtest.h"
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include <openbabel/atom.h>
#include <openbabel/obiter.h>

#include <random>
#include <iostream>
#include <vector>
#include <algorithm>

using std::endl;
using namespace OpenBabel;

BOOST_AUTO_TEST_CASE(canonstabletest)
{
  std::ifstream ifs(OBTestUtil::GetFilename("canonstable.can"));
  BOOST_REQUIRE( ifs );

  OBMol mol;
  OBConversion conv;
  conv.SetInFormat("smi");
  conv.SetOutFormat("can");

  std::string line;
  while (std::getline(ifs, line)) {
    line.erase(line.find_last_not_of(" \n\r\t") + 1);
    BOOST_REQUIRE( conv.ReadString(&mol, line) );

    std::vector<OBAtom*> atoms;
    FOR_ATOMS_OF_MOL(atom, mol)
      atoms.push_back(&*atom);

    for (int i = 0; i < 5; ++i) {
      // shuffle the atoms
      std::shuffle(atoms.begin(), atoms.end(), std::default_random_engine());
      mol.RenumberAtoms(atoms);

      // get can smiles
      mol.SetTitle("");
      std::string cansmi = conv.WriteString(&mol, true);
      // comapare with ref
      if (cansmi != line) {
        std::cerr << "ref = " << line << endl
            << "can = " << cansmi << endl;
      }
      BOOST_REQUIRE_EQUAL( cansmi, line);
    }
  }
}

