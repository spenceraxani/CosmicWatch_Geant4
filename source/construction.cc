#include "construction.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	DefineMaterials();

	if (SiPMpos == "base"){
		SiPMsize = G4ThreeVector(SiPMBase/2., SiPMBase/2., SiPMHeight/2.);
		SiPMplacement = G4ThreeVector(0., 0., -(ScintZLen+SiPMHeight)/2.);
	}
	else{
		SiPMsize = G4ThreeVector(SiPMHeight/2., SiPMBase/2., SiPMBase/2.);
		SiPMplacement = G4ThreeVector((ScintXLen+SiPMHeight)/2., 0., 0.);
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

	//-------------World mat-------------//
	Air = nist->FindOrBuildMaterial("G4_AIR");

	//Properties
	//std::vector<G4double> rindexAir = {1.0, 1.0};//For use with PScint
	std::vector<G4double> rindexAir  =  {1.00, 1.00, 1.00, 1.00,
										1.00, 1.00, 1.00, 1.00,
										1.00, 1.00, 1.00, 1.00,
										1.00, 1.00, 1.00, 1.00,
										1.00, 1.00, 1.00, 1.00};//For use with LYSO

	//-------------NaI-------------//
	/*Na = nist->FindOrBuildElement("Na");
	I = nist->FindOrBuildElement("I");
	NaI = new G4Material("NaI", 3.67*g/cm3, 2);
	NaI->AddElement(Na, 1);
	NaI->AddElement(I, 1);*/

	//-------------LYSO:Ce-------------//
	G4double LYSO_density = 7.1*g/cm3;
  	LYSO = new G4Material("LYSO", LYSO_density, 4);
  	LYSO->AddElement(nist->FindOrBuildElement("Lu"), 71*perCent);
  	LYSO->AddElement(nist->FindOrBuildElement("Si"), 7*perCent);
  	LYSO->AddElement(nist->FindOrBuildElement("O"), 18*perCent);
  	LYSO->AddElement(nist->FindOrBuildElement("Y"), 4*perCent);

	//Cerium doping
	//0.19
	G4double Ce_doping_percent = 0.5*perCent;
  	Scint = new G4Material("Scint", LYSO_density, 2);
  	Scint->AddMaterial(LYSO, 100.00*perCent-Ce_doping_percent);
  	Scint->AddElement(nist->FindOrBuildElement("Ce"), Ce_doping_percent);

	//Properties
	const G4int ene_Len = 20;
  	std::vector<G4double> ene	=	{1.79*eV, 1.85*eV, 1.91*eV, 1.97*eV,
									2.04*eV, 2.11*eV, 2.19*eV, 2.27*eV,
									2.36*eV, 2.45*eV, 2.56*eV, 2.67*eV,
									2.80*eV, 2.94*eV, 3.09*eV, 3.25*eV,
									3.44*eV, 3.65*eV, 3.89*eV, 4.16*eV};

  	std::vector<G4double> specLYSO  =	{0.01, 0.10, 0.20, 0.50,
										0.90, 1.70, 2.90, 5.00,
										8.30, 12.5, 17.0, 22.9,
										26.4, 25.6, 16.8, 4.20,
										0.30, 0.20, 0.10, 0.01};

  	std::vector<G4double> rindexLYSO =  {1.81, 1.81, 1.81, 1.81,
										1.81, 1.81, 1.81, 1.81,
										1.81, 1.81, 1.81, 1.81,
										1.81, 1.81, 1.81, 1.81,
										1.81, 1.81, 1.81, 1.81};

  	std::vector<G4double> absorLenLYSO   =  {3.5*m, 3.5*m, 3.5*m, 3.5*m,
											3.5*m, 3.5*m, 3.5*m, 3.5*m,
											3.5*m, 3.5*m, 3.5*m, 3.5*m,
											3.5*m, 3.5*m, 3.5*m, 3.5*m,
											3.5*m, 3.5*m, 3.5*m, 3.5*m};

	G4double decaytLYSO = 36.0*ns;
	G4double LYieldLYSO = 33./keV;

	//-------------Plastic Scintillator-------------//
	/*ScintMat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

	//Properties
	//E=hc/lambda
	//hc=1.23984197x10^-6 eVm
	//"visible light" 200nm to 900nm
	std::vector<G4double> ene = {1.23984197*eV/0.9, 1.23984197*eV/0.2};
	std::vector<G4double> rindexPScint = {1.78, 1.78};
	std::vector<G4double> absorLenPScint = {38.1*cm, 38.1*cm};
	std::vector<G4double> absorLenPScint = {7.1314e-06*mm, 7.13875e-06*mm};
	std::vector<G4double> specPSint = {1.0, 1.0};
	G4double decaytPScint = 250*ns;
	G4double LYieldPScint = 38./keV*/

	//-------------Set Material Properties-------------//

	//Air
	G4MaterialPropertiesTable *mptAir = new G4MaterialPropertiesTable();
	mptAir->AddProperty("RINDEX", ene, rindexAir);

	Air->SetMaterialPropertiesTable(mptAir);

	//Scint
	G4MaterialPropertiesTable *mptScint = new G4MaterialPropertiesTable();
	mptScint->AddProperty("RINDEX", ene, rindexLYSO);
	mptScint->AddProperty("ABSLENGTH", ene, absorLenLYSO, false, true);
	mptScint->AddProperty("SCINTILLATIONCOMPONENT1", ene, specLYSO); //energy spectrum of the emitted photons
	mptScint->AddConstProperty("SCINTILLATIONYIELD", LYieldLYSO); //# of photons per energy loss
	mptScint->AddConstProperty("RESOLUTIONSCALE", 1.0); //photon distribution sigma
	mptScint->AddConstProperty("SCINTILLATIONTIMECONSTANT1", decaytLYSO); //decay time inside the scintillator
	mptScint->AddConstProperty("SCINTILLATIONYIELD1", 1.);

	Scint->SetMaterialPropertiesTable(mptScint);

	//-------------Mirror Coating for Scint-------------//
	mirrorCoatScint = new G4OpticalSurface("mirrorCoatScint");

	mirrorCoatScint->SetType(dielectric_metal);
	mirrorCoatScint->SetFinish(ground);
	mirrorCoatScint->SetModel(unified);

	std::vector<G4double> reflectivityScint(ene_Len, 1.0);//fully reflect all photons

	G4MaterialPropertiesTable *mptMirrorScint = new G4MaterialPropertiesTable();
	mptMirrorScint->AddProperty("REFLECTIVITY", ene, reflectivityScint);
	
	mirrorCoatScint->SetMaterialPropertiesTable(mptMirrorScint);

	//-------------Mirror Coating for SiPM-------------//
	mirrorCoatSiPM = new G4OpticalSurface("mirrorCoatSiPM");

	mirrorCoatSiPM->SetType(dielectric_dielectric);
	mirrorCoatSiPM->SetFinish(groundfrontpainted);
	mirrorCoatSiPM->SetModel(unified);

	std::vector<G4double> reflectivitySiPM(ene_Len, 0.0);//fully absorb all photons

	G4MaterialPropertiesTable *mptMirrorSiPM = new G4MaterialPropertiesTable();
	mptMirrorSiPM->AddProperty("REFLECTIVITY", ene, reflectivitySiPM);

	mirrorCoatSiPM->SetMaterialPropertiesTable(mptMirrorSiPM);

	//-------------Optica boundary between Scint and SiPM-------------//
	opBoundary = new G4OpticalSurface("Boundary");

	opBoundary->SetType(dielectric_dielectric);
	opBoundary->SetFinish(Rough_LUT);
	opBoundary->SetModel(unified);
}

void MyDetectorConstruction::BuildPlasticScint()
{
	//-------------Volumes-------------//
	//scintillator
	//solidScint = new G4Tubs("solidScint", 0.*cm, ScintXLen/2., ScintZLen/2., 0.*deg, 360.*deg);
	solidScint = new G4Box("solidScint", ScintXLen/2., ScintYLen/2., ScintZLen/2.);
	logicScint = new G4LogicalVolume(solidScint, Scint, "logicScint");
	physScint = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
		logicScint, "physScint", logicWorld, false, 0, true);
	//SiPM
	solidSiPM = new G4Box("solidSiPM", SiPMsize[0], SiPMsize[1], SiPMsize[2]);
	logicSiPM = new G4LogicalVolume(solidSiPM, Air, "logicSiPM");
	physSiPM = new G4PVPlacement(0, SiPMplacement,
		logicSiPM, "physSiPM", logicWorld, false, 0, true);

	//-------------Optical surfaces-------------//
	//reflective surface (air, scintillator)
	G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicScint, mirrorCoatScint);
	//opaque surface (air, SiPM)
	G4LogicalSkinSurface *skinSiPM = new G4LogicalSkinSurface("skinSiPM", logicSiPM, mirrorCoatSiPM);
	//transparent surface (scintillator, SiPM)
	G4LogicalBorderSurface* SiPMSurface = new G4LogicalBorderSurface("SiPMSurface", physScint, physSiPM, opBoundary);

	auto visAttributes = new G4VisAttributes(G4Colour(0, 0, 255, 0.2));
	visAttributes->SetVisibility(true);
	visAttributes->SetForceSolid(true);
	logicScint->SetVisAttributes(visAttributes);
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