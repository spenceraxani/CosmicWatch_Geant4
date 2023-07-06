#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	DefineMaterials();
}

MyDetectorConstruction::~MyDetectorConstruction()
{
	for (auto visAttributes: fVisAttributes){
    	delete visAttributes;
	}

	G4cout << "\nDeleting vis attributes\n" << G4endl;
}

void MyDetectorConstruction::DefineMaterials()
{
	G4NistManager *nist = G4NistManager::Instance();

	//world mat
	Air = nist->FindOrBuildMaterial("G4_AIR");

	//NaI
	/*Na = nist->FindOrBuildElement("Na");
	I = nist->FindOrBuildElement("I");
	NaI = new G4Material("NaI", 3.67*g/cm3, 2);
	NaI->AddElement(Na, 1);
	NaI->AddElement(I, 1);*/

	PScintMat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

	//Material properties

	//E=hc/lambda
	//hc=1.23984197x10^-6 eVm
	//"visible light" 200nm to 900nm
	std::vector<G4double> energy = {1.23984197*eV/0.9, 1.23984197*eV/0.2};
	
	//air
	std::vector<G4double> rindexAir = {1.0, 1.0};

	G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable();
	mptAir->AddProperty("RINDEX", energy, rindexAir);

	Air->SetMaterialPropertiesTable(mptAir);

	//Plastic scintillator
	std::vector<G4double> rindexPScint = {1.78, 1.78};
	std::vector<G4double> absorPScint = {38.1*cm, 38.1*cm};
	//std::vector<G4double> absorption = {7.1314e-06*mm, 7.13875e-06*mm};
	std::vector<G4double> fractionPSint = {1.0, 1.0};

	G4MaterialPropertiesTable *mptPScint = new G4MaterialPropertiesTable();
	mptPScint->AddProperty("RINDEX", energy, rindexPScint);
	mptPScint->AddProperty("ABSLENGTH", energy, absorPScint, false, true);
	mptPScint->AddProperty("SCINTILLATIONCOMPONENT1", energy, fractionPSint);
	mptPScint->AddConstProperty("SCINTILLATIONYIELD", 38./keV); //# of photons per energy loss
	mptPScint->AddConstProperty("RESOLUTIONSCALE", 0.); //sigma photon distribution
	mptPScint->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 250*ns); //decay time inside the scintillator
	mptPScint->AddConstProperty("SCINTILLATIONYIELD1", 1.);

	PScintMat->SetMaterialPropertiesTable(mptPScint);

	//mirror coating for scintillators
	mirrorCoat = new G4OpticalSurface("mirrorCoat");

	mirrorCoat->SetType(dielectric_metal);
	mirrorCoat->SetFinish(ground);
	mirrorCoat->SetModel(unified);

	std::vector<G4double> reflectivity = {1.0, 1.0};

	G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();
	mptMirror->AddProperty("REFLECTIVITY", energy, reflectivity);

	mirrorCoat->SetMaterialPropertiesTable(mptMirror);
}

void MyDetectorConstruction::BuildPlasticScint()
{
	//scintillator
	solidPScint = new G4Box("solidPScint", PScintBase/2., PScintBase/2., PScintHeight/2.);
	logicPScint = new G4LogicalVolume(solidPScint, PScintMat, "logicPScint");
	physPScint = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
		logicPScint, "physPScint", logicWorld, false, 0, true);

	//reflective surface
	G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicPScint, mirrorCoat);

	fScoringVolume = logicPScint;

	auto visAttributes = new G4VisAttributes(G4Colour(0, 0, 255, 0.3));
	visAttributes->SetVisibility(true);
	visAttributes->SetForceSolid(true);
	logicPScint->SetVisAttributes(visAttributes);
	fVisAttributes.push_back(visAttributes);
}

G4VPhysicalVolume* MyDetectorConstruction::Construct()
{
	//simulation world
	solidWorld = new G4Box("solidWorld", hWorld/2., hWorld/2., hWorld/2.);
	logicWorld = new G4LogicalVolume(solidWorld, Air, "logicWorld");
	physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
		logicWorld, "physWorld", 0, false, 0, true);

	auto visAttributes = new G4VisAttributes(G4Colour(0, 0, 0, 1.));
	visAttributes->SetVisibility(true);
	logicWorld->SetVisAttributes(visAttributes);
	fVisAttributes.push_back(visAttributes);

	BuildPlasticScint();

	G4cout << "\nDetector constructed\n" << G4endl;
	return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{
	MySensitiveDetector *sensDet = new MySensitiveDetector("sensitiveDetector");

	if(logicPScint != NULL)
	{
		logicPScint->SetSensitiveDetector(sensDet);
	}
}