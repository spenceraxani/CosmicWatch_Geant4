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
	G4double hWorld = 20.*cm;
	
	//Plastic scintillator
	G4double PScintXLen = 50.*mm; //100 or 50
	G4double PScintYLen = 50.*mm; //50 or 50
	G4double PScintZLen = 10.*mm; //20 or 10

	//SiPM
	G4String SiPMpos = "base"; //"base" or "side"
	G4ThreeVector SiPMsize, SiPMplacement;

	G4double SiPMBase = 3.*mm;
	G4double SiPMHeight = 1.*mm;

	G4Tubs *solidPScint;
	G4Box *solidWorld, *solidSiPM;
	G4LogicalVolume *logicWorld, *logicPScint, *logicSiPM;
	G4VPhysicalVolume *physWorld, *physPScint, *physSiPM;

	//reflective scintillator surface
	G4OpticalSurface *mirrorCoatPScint, *mirrorCoatSiPM, *opBoundary;

	//Materials
	G4Material *Air, *PScintMat;

	//Scoring volume
	G4LogicalVolume *fScoringVolume;
};

#endif