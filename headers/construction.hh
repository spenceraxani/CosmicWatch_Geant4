#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"

#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4VisAttributes.hh"

#include "G4VUserDetectorConstruction.hh"

#include "detector.hh"
#include <vector>

class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public:
	MyDetectorConstruction();
	~MyDetectorConstruction();

	virtual G4VPhysicalVolume* Construct();

	G4LogicalVolume *GetScoringVolume() const {return fScoringVolume;}

private:
	//build materials
	void DefineMaterials();
	//build plastic scintillator and SiPM
	void BuildPlasticScint();
	//build sensitive detector
	void ConstructSDandField();

	std::vector<G4VisAttributes*> fVisAttributes;

	//simulation world
	G4double hWorld = 15.*cm; //for use with big PScint
	//G4double hWorld = 10.*cm;
	
	//Cylindrical PScint
	//G4double ScintXLen = 50.*mm; //100 or 50
	//G4double ScintYLen = 50.*mm; //100 or 50
	//G4double ScintZLen = 10.*mm; //20 or 10

	//scintillator
	G4double ScintXLen = 50.*mm; //4, 3 or 10
	G4double ScintYLen = 50.*mm; //22, 20 or 20
	G4double ScintZLen = 10.*mm; //4, 3 or 10

	//SiPM
	G4String SiPMpos = "base"; //"base" or "side"
	G4ThreeVector SiPMsize, SiPMplacement;

	G4double SiPMBase = 3.*mm;
	G4double SiPMHeight = 1.*mm;

	//G4Tubs *solidScint;
	G4Box *solidScint;
	G4Box *solidWorld, *solidSiPM;
	G4LogicalVolume *logicWorld, *logicScint, *logicSiPM;
	G4VPhysicalVolume *physWorld, *physScint, *physSiPM;

	//reflective scintillator surface
	G4OpticalSurface *mirrorCoatScint, *mirrorCoatSiPM, *opBoundary;

	//Materials
	G4Material *Air, *LYSO, *Scint;

	//Scoring volume
	G4LogicalVolume *fScoringVolume;
};

#endif