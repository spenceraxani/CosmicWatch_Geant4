#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	DefineMaterials();

	if (SiPMpos == "base"){
		SiPMsize = G4ThreeVector(SiPMBase/2., SiPMBase/2., SiPMHeight/2.);
		SiPMplacement = G4ThreeVector(0., 0., -5.5*mm);
	}
	else{
		SiPMsize = G4ThreeVector(SiPMHeight/2., SiPMBase/2., SiPMBase/2.);
		SiPMplacement = G4ThreeVector(25.5*mm, 0., 0.);
	}
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
	mirrorCoatPScint = new G4OpticalSurface("mirrorCoatPScint");

	mirrorCoatPScint->SetType(dielectric_metal);
	mirrorCoatPScint->SetFinish(ground);
	mirrorCoatPScint->SetModel(unified);

	std::vector<G4double> reflectivityPSinct = {1.0, 1.0};

	G4MaterialPropertiesTable *mptMirrorPSinct = new G4MaterialPropertiesTable();
	mptMirrorPSinct->AddProperty("REFLECTIVITY", energy, reflectivityPSinct);
	
	mirrorCoatPScint->SetMaterialPropertiesTable(mptMirrorPSinct);

	//mirror coating for SiPM
	mirrorCoatSiPM = new G4OpticalSurface("mirrorCoatSiPM");

	mirrorCoatSiPM->SetType(dielectric_dielectric);
	mirrorCoatSiPM->SetFinish(groundfrontpainted);
	mirrorCoatSiPM->SetModel(unified);

	std::vector<G4double> reflectivitySiPM = {0.0, 0.0};

	G4MaterialPropertiesTable *mptMirrorSiPM = new G4MaterialPropertiesTable();
	mptMirrorSiPM->AddProperty("REFLECTIVITY", energy, reflectivitySiPM);

	mirrorCoatSiPM->SetMaterialPropertiesTable(mptMirrorSiPM);

	//Optica boundary between Scintillator and SiPM
	opBoundary = new G4OpticalSurface("Boundary");

	opBoundary->SetType(dielectric_dielectric);
	opBoundary->SetFinish(Rough_LUT);
	opBoundary->SetModel(unified);
}

void MyDetectorConstruction::BuildPlasticScint()
{
	//-------------Volumes-------------//
	//scintillator
	solidPScint = new G4Box("solidPScint", PScintBase/2., PScintBase/2., PScintHeight/2.);
	logicPScint = new G4LogicalVolume(solidPScint, PScintMat, "logicPScint");
	physPScint = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
		logicPScint, "physPScint", logicWorld, false, 0, true);

	//SiPM
	solidSiPM = new G4Box("solidSiPM", SiPMsize[0], SiPMsize[1], SiPMsize[2]);
	logicSiPM = new G4LogicalVolume(solidSiPM, Air, "logicSiPM");
	physSiPM = new G4PVPlacement(0, SiPMplacement,
		logicSiPM, "physSiPM", logicWorld, false, 0, true);

	//-------------Optical surfaces-------------//
	//reflective surface (air, scintillator)
	G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicPScint, mirrorCoatPScint);
	//opaque surface (air, SiPM)
	G4LogicalSkinSurface *skinSiPM = new G4LogicalSkinSurface("skinSiPM", logicSiPM, mirrorCoatSiPM);
	//transparent surface (scintillator, SiPM)
	G4LogicalBorderSurface* SiPMSurface = new G4LogicalBorderSurface("SiPMSurface", physPScint, physSiPM, opBoundary);

	auto visAttributes = new G4VisAttributes(G4Colour(0, 0, 255, 0.3));
	visAttributes->SetVisibility(true);
	visAttributes->SetForceSolid(true);
	logicPScint->SetVisAttributes(visAttributes);
	fVisAttributes.push_back(visAttributes);

	visAttributes = new G4VisAttributes(G4Colour(0, 0, 0, 0.3));
	visAttributes->SetVisibility(true);
	visAttributes->SetForceSolid(true);
	logicSiPM->SetVisAttributes(visAttributes);
	fVisAttributes.push_back(visAttributes);

	fScoringVolume = logicSiPM;
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

	if(fScoringVolume != NULL)
	{
		fScoringVolume->SetSensitiveDetector(sensDet);
	}
}