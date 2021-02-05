/**********************************************************************
Copyright (C) 2010 by Geoffrey R. Hutchison

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
***********************************************************************/
#include <openbabel/babelconfig.h>

#include <openbabel/obmolecformat.h>
#include <openbabel/mol.h>
#include <openbabel/atom.h>
#include <openbabel/bond.h>
#include <openbabel/obiter.h>
#include <openbabel/elements.h>
#include <openbabel/generic.h>

#include <cstdlib>


using namespace std;
namespace OpenBabel {

    class FHIaimsFormat : public OBMoleculeFormat {
    public:
        //Register this format type ID
        FHIaimsFormat() {
            OBConversion::RegisterFormat("fhiaims", this);
        }

        virtual const char *Description() //required
        {
            return
                    "FHIaims XYZ format\n"
                    "Read Options e.g. -as\n"
                    " s  Output single bonds only\n"
                    " b  Disable bonding entirely\n\n";
        };

        virtual const char *SpecificationURL() { return "http://www.fhi-berlin.mpg.de/th/aims/"; }; //optional

        //Flags() can return be any the following combined by | or be omitted if none apply
        // NOTREADABLE  READONEONLY  NOTWRITABLE  WRITEONEONLY
        virtual unsigned int Flags() {
            return READONEONLY | WRITEONEONLY;
        };

        //*** This section identical for most OBMol conversions ***
        ////////////////////////////////////////////////////
        /// The "API" interface functions
        virtual bool ReadMolecule(OBBase *pOb, OBConversion *pConv);

        virtual bool WriteMolecule(OBBase *pOb, OBConversion *pConv);
    };
//***

//Make an instance of the format class
    FHIaimsFormat theFHIaimsFormat;

/////////////////////////////////////////////////////////////////
    bool FHIaimsFormat::ReadMolecule(OBBase *pOb, OBConversion *pConv) {

        OBMol *pmol = pOb->CastAndClear<OBMol>();
        if (pmol == nullptr)
            return false;

        //Define some references so we can use the old parameter names
        istream &ifs = *pConv->GetInStream();
        OBMol &mol = *pmol;
        const char *title = pConv->GetTitle();

        char buffer[BUFF_SIZE];
        string str;
        double x, y, z;
        OBAtom *atom;
        vector<string> vs;
        vector<vector3> lattice;

        mol.BeginModify();

        while (ifs.getline(buffer, BUFF_SIZE)) {
            if (buffer[0] == '#')
                continue; // comment line

            if (strstr(buffer, "atom") != nullptr) {
                // atom X Y Z element (in real-space Angstroms)
                tokenize(vs, buffer);
                if (vs.size() < 5)
                    continue; // invalid line
                atom = mol.NewAtom();
                x = atof((char *) vs[1].c_str());
                y = atof((char *) vs[2].c_str());
                z = atof((char *) vs[3].c_str());
                atom->SetVector(x, y, z); //set coordinates

                //set atomic number
                int atomicNum = OBElements::GetAtomicNum(vs[4].c_str());
                atom->SetAtomicNum(atomicNum);

            } else if (strstr(buffer, "lattice_vector") != nullptr) {
                // lattice_vector X Y Z (in real-space Angstroms)
                tokenize(vs, buffer);
                if (vs.size() < 4)
                    continue;

                x = atof((char *) vs[1].c_str());
                y = atof((char *) vs[2].c_str());
                z = atof((char *) vs[3].c_str());
                lattice.push_back(vector3(x, y, z));
            }
        }

        if (!pConv->IsOption("b", OBConversion::INOPTIONS))
            mol.ConnectTheDots();
        if (!pConv->IsOption("s", OBConversion::INOPTIONS) && !pConv->IsOption("b", OBConversion::INOPTIONS))
            mol.PerceiveBondOrders();

        // clean out remaining blank lines
        std::streampos ipos;
        do {
            ipos = ifs.tellg();
            ifs.getline(buffer, BUFF_SIZE);
        } while (strlen(buffer) == 0 && !ifs.eof());
        ifs.seekg(ipos);

        mol.EndModify();
        // Check if there are lattice vectors and add them
        if (lattice.size() == 3) {
            OBUnitCell *uc = new OBUnitCell;
            uc->SetOrigin(fileformatInput);
            uc->SetData(lattice[0], lattice[1], lattice[2]);
            mol.SetData(uc);
        }
        mol.SetTitle(title);
        return (true);
    }

////////////////////////////////////////////////////////////////

    bool FHIaimsFormat::WriteMolecule(OBBase *pOb, OBConversion *pConv) {
        OBMol *pmol = dynamic_cast<OBMol *>(pOb);
        if (pmol == nullptr)
            return false;

        //Define some references so we can use the old parameter names
        ostream &ofs = *pConv->GetOutStream();
        OBMol &mol = *pmol;

        char buffer[BUFF_SIZE];

        ofs << "#\n";
        ofs << "# " << mol.GetTitle() << '\n';
        ofs << "# Generated by Open Babel " << BABEL_VERSION << '\n';
        ofs << "#\n";

        FOR_ATOMS_OF_MOL(atom, mol) {
            snprintf(buffer, BUFF_SIZE,
                     "atom           %15.5f%15.5f%15.5f  %s",
                     atom->GetX(),
                     atom->GetY(),
                     atom->GetZ(),
                     OBElements::GetSymbol(atom->GetAtomicNum()));
            ofs << buffer << '\n';
        }

        if (mol.HasData(OBGenericDataType::UnitCell)) {
            OBUnitCell *uc = (OBUnitCell *) mol.GetData(OBGenericDataType::UnitCell);
            ofs << "#\n";
            ofs << "# unit cell\n";
            ofs << "#\n";

            vector<vector3> v = uc->GetCellVectors();

            for (unsigned int i = 0; i < v.size(); ++i) {
                snprintf(buffer, BUFF_SIZE,
                         "lattice_vector %15.5f%15.5f%15.5f\n", v[i].x(), v[i].y(), v[i].z());
                ofs << buffer;
            }
        }

        return (true);
    }

} //namespace OpenBabel
