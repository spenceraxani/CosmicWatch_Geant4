#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4Box.hh"
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
	G4double PScintBase = 50.*mm;
	G4double PScintHeight = 10.*mm;

	//SiPM
	G4String SiPMpos = "side"; //"base" or "side"
	G4ThreeVector SiPMsize, SiPMplacement;

	G4double SiPMBase = 3.*mm;
	G4double SiPMHeight = 1.*mm;

	G4Box *solidWorld, *solidPScint, *solidSiPM;
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