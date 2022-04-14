#include "d_material_name.h"

const std::unordered_set<std::string> KNOWN_MATTER_SET{
        "CCl4",
        "CH3Cl",
        "C2H5OC2H5",
        "KO2",
        "CuCl2",
        "CHCl3",
        "CH3COONa",
        "CH3COOCH3",
        "CH3COOC2H5",
        "CH3CH2Br",
        "CH2Cl2",
        "C2H5ONa",
        "(C2H5O)2Mg",
        "C2H5ONO2",
        "CH3CH2OSO3H",
        "(C2H5O)3Al",
        "CH3CH2OH",
        "C11H22O11",
        "Ba(NO3)2",
        "Al2S3",
        "NaAlO2",
        "NH3.H2O",
        "AgOH",
        "Ca(ClO)2",
        "Ca(H2PO4)2",
        "CH4",
        "C2H6",
        "C2H4",
        "C2H2",
        "Fe(NO3)2",
        "CaC2",
        "CH3CHO",
        "C2H5OH",
        "Fe(OH)2",
        "Ca(HCO3)2",
        "CH3OH",
        "Ag(NH3)2OH",
        "HCHO",
        "HClO",
        "K2MnO4",
        "NaH",
        "Mg(HCO3)2",
        "Mg3N2",
        "MgSiO3",
        "NO2",
        "NO",
        "N2O4",
        "Na2SO4",
        "Cu(OH)2",
        "H4SiO4",
        "H2SiO3",
        "NaBr",
        "NaHS",
        "NaHSO3",
        "NaHSO4",
        "Cu3P",
        "HPO3",
        "PCl5",
        "PCl3",
        "C6H5SO3H",
        "C6H12",
        "C6H6",
        "C6H5Br",
        "C6H5Cl",
        "CS2",
        "K2S",
        "Ca(HSO3)2",
        "CaSO3",
        "CaSiO3",
        "ZrOCl2",
        "ZrO2",
        "Zr(SO4)2",
        "Zr(NO3)4",
        "Zr",
        "ZnSO4",
        "ZnS",
        "ZnO",
        "ZnCl2",
        "ZnCO3",
        "ZnC2O4",
        "Zn2P2O7",
        "Zn(NO3)2",
        "Zn",
        "WO3",
        "W",
        "VO2",
        "V2O5",
        "V",
        "UO3",
        "UF4",
        "UCl4",
        "U3O8",
        "U",
        "TiOSO4",
        "TiO2",
        "TiCl4",
        "TiCl3",
        "Ti",
        "ThCl4",
        "Th(SO4)2",
        "Th(NO3)4",
        "Th(C2O4)2",
        "Th",
        "SrSO4",
        "SrO",
        "SrCl2",
        "SrCO3",
        "SrC2O4",
        "Sr3(PO4)2",
        "Sr(NO3)2",
        "Sr",
        "SnS2",
        "SnS",
        "SnO2",
        "SnCl2",
        "Sn",
        "SiO2",
        "SiF4",
        "SiCl4",
        "Si",
        "SbCl5",
        "SbCl3",
        "Sb2O5",
        "Sb2O3",
        "Sb",
        "SO3",
        "SO2",
        "S",
        "PbbI2",
        "PbSO4",
        "PbS",
        "PbO2",
        "PbO",
        "PbCrO4",
        "PbCl2",
        "PbCO3",
        "PbC2O4",
        "Pb3O4",
        "Pb3(PO4)2",
        "Pb2O3",
        "Pb(NO3)2",
        "Pb(IO3)2",
        "Pb",
        "P2O5",
        "P",
        "O3",
        "O2",
        "O",
        "NiSO4",
        "NiS",
        "NiO",
        "NiCl2",
        "Ni(NO3)2",
        "Ni",
        "NaOH",
        "NaNO3",
        "NaNO2",
        "NaI",
        "NaHCO3",
        "NaH2PO4",
        "NaH",
        "NaClO",
        "NaCl",
        "NaCO3",
        "NaCN",
        "NaBrO3",
        "NaBiO3",
        "NaAsO4",
        "Na3PO4",
        "Na3AsO4",
        "Na3AsO3",
        "Na2SO4",
        "Na2SiO3",
        "Na2SO3",
        "Na2S2O3",
        "Na2S",
        "Na2O2",
        "Na2O",
        "Na2HPO4",
        "Na2HAsO3",
        "Na2CrO4",
        "Na2CO3",
        "Na2C2O4",
        "Na2B4O7",
        "Na",
        "NH4VO3",
        "NH4NO3",
        "NH4HCO3",
        "NH4H2PO4",
        "NH4Cl",
        "NH3",
        "N2",
        "N",
        "MnSO4",
        "MnS",
        "MnO2",
        "MnO",
        "MnCl2",
        "MnCO3",
        "Mn3O4",
        "Mn2P2O7",
        "Mn2O3",
        "Mn(NO3)2",
        "Mn",
        "MgSO4",
        "MgO",
        "MgNH4PO4",
        "MgNH4AsO4",
        "MgH2",
        "MgCl2",
        "MgCO3",
        "MgC2O4",
        "Mg2P2O7",
        "Mg(OH)2",
        "Mg(NO3)2",
        "Mg",
        "LiOH",
        "LiH",
        "LiCl",
        "Li2O",
        "Li2CO3",
        "Li",
        "KOH",
        "KNaC4O6H4",
        "KNO3",
        "KNO2",
        "KMnO4",
        "KIO3",
        "KI",
        "KHSO4",
        "KHC8O4H8",
        "KHC4O6H4",
        "KHC2O4",
        "KH",
        "KFe(SO4)2",
        "KF",
        "KClO4",
        "KClO3",
        "KCl",
        "KBrO3",
        "KBr",
        "KAl(SO4)2",
        "K4Fe(CN)6",
        "K3PO4",
        "K3Fe(CN)6",
        "K3AsO4",
        "K2SO4",
        "K2S2O7",
        "K2PtCl6",
        "K2O",
        "K2CrO4",
        "K2Cr2O7",
        "K2CO3",
        "K",
        "I2",
        "I",
        "HgSO4",
        "HgS",
        "HgO",
        "HgI2",
        "HgCl2",
        "Hg2SO4",
        "Hg2I2",
        "Hg2Cl2",
        "Hg2Br2",
        "Hg2(NO3)2",
        "Hg(NO3)2",
        "Hg(CN)2",
        "Hg",
        "HNO3",
        "HNO2",
        "HIO3",
        "HI",
        "HF",
        "HClO4",
        "HCl",
        "HCOONa",
        "HCOOH",
        "HCN",
        "HBr",
        "H3PO4",
        "H3PO3",
        "H3BO3",
        "H3AsO4",
        "H3AsO3",
        "H2SO4",
        "H2SO3",
        "H2S",
        "H2O2",
        "H2O",
        "H2CO3",
        "H2C2O4",
        "H2",
        "H",
        "FeSO4",
        "FeS2",
        "FeS",
        "FeO",
        "FeCl3",
        "FeCl2",
        "FeCO3",
        "Fe3O4",
        "Fe2O3",
        "Fe2(SO4)3",
        "Fe(OH)3",
        "Fe(NO3)3",
        "Fe",
        "F2",
        "F",
        "CuSO4",
        "CuSCN",
        "CuS",
        "CuO",
        "CuCl2CuI",
        "CuCl",
        "Cu2S",
        "Cu2O",
        "Cu2(OH)2CO3",
        "Cu(NO3)2",
        "Cu",
        "CsOH",
        "Cs",
        "CrCl3",
        "Cr2O3",
        "Cr2(SO4)3",
        "Cr",
        "CoSO4",
        "CoS",
        "CoCl2",
        "Co3O4",
        "Co2O3",
        "Co(NO3)2",
        "Co",
        "Cl2",
        "Cl",
        "CeO2",
        "Ce(SO4)2",
        "Ce",
        "CdS",
        "CdCl2",
        "CdCO3",
        "Cd",
        "CaSO4",
        "CaO",
        "CaCl2",
        "CaCO3",
        "CaC2O4",
        "Ca3(PO4)2",
        "Ca(OH)2",
        "Ca",
        "CS(NH2)2",
        "CO2",
        "CO(NH2)2",
        "CO",
        "CH3COONH4",
        "CH3COOH",
        "C6H5SO3H",
        "C6H5ONa",
        "C6H5OH",
        "C6H5NO2",
        "C6H5Cl",
        "C6H5COOH",
        "C6H5Br",
        "C6H12",
        "C",
        "Br2",
        "Br",
        "BiONO3",
        "BiOHCO3",
        "BiOCl",
        "BiCl3",
        "Bi2S3",
        "Bi2O3",
        "Bi(NO3)3",
        "Bi",
        "BeO",
        "Be",
        "BaSO4",
        "BaO",
        "BaCrO4",
        "BaCl2",
        "BaCO3",
        "BaBr2",
        "Ba3(AsO4)2",
        "Ba(OH)2",
        "Ba",
        "B2O3",
        "B",
        "As2S3",
        "As2O5",
        "As2O3",
        "As",
        "AlCl3",
        "AlBr3",
        "Al2O3",
        "Al2(SO4)3",
        "Al(OH)3",
        "Al(NO3)3",
        "Al",
        "AgSCN",
        "AgNO3",
        "AgI",
        "AgCl",
        "AgCN",
        "AgBrO3",
        "AgBr",
        "Ag3AsO4",
        "Ag2SO4",
        "Ag2CrO4",
        "Ag",
        "(NH4)3PO4",
        "(NH4)2SO4",
        "(NH4)2S",
        "(NH4)2PtCl6",
        "(NH4)2MoO4",
        "(NH4)2HPO4",
        "(NH4)2Fe(SO4)2",
        "(NH4)2CO3",
        "(NH4)2C2O4",
        "(CH3COO)2UO2",
        "(CH3COO)2Pb",
        "(CH3CO)2O",
        "(CH2COO)2Zn"
};