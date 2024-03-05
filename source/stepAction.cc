#include "stepAction.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
	fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step* step)
{
	//Get current volume
	G4StepPoint* prePoint = step->GetPreStepPoint();

	G4LogicalVolume *currentVolume = prePoint->GetTouchableHandle()
									->GetVolume()
									->GetLogicalVolume();

	G4String CurrentVolName = currentVolume->GetName();

	const MyDetectorConstruction *detectorConstruction = 
			static_cast<const MyDetectorConstruction*> 
			(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

	//Get scoring volume
	G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();
	G4String ScoringVolName = fScoringVolume->GetName();

	//Particle Track
	G4Track *track = step->GetTrack();

	//get primatry particle incident direction
	if(track->GetCurrentStepNumber()==1 && track->GetTrackID()==1)
	{
		const G4ThreeVector ParticleMomDir = track->GetMomentumDirection();
		fEventAction->SetParticleMomDir(ParticleMomDir);
	}

	//Particle name
	const G4ParticleDefinition* particle = step->GetTrack()->GetDefinition();
	//const G4String pName = particle->GetParticleName();

	//count number of photons reaching SiPM
	if(particle==G4OpticalPhoton::OpticalPhotonDefinition() && currentVolume==fScoringVolume){
		//G4int id = track->GetTrackID();
		//G4int idP = track->GetParentID();
		track->SetTrackStatus(fStopAndKill);

		//G4cout << "optical photon in SiPM\t" << id << "\t" << idP << G4endl;
		fEventAction->AddPhoton();
	}

	//Compare current and scoring volume
	//if(currentVolume == fScoringVolume){
	if(CurrentVolName == "logicPScint"){

		//Check if there is energy deposition
		G4double edep = step->GetTotalEnergyDeposit();

		if (edep > 0){

			G4StepPoint* postPoint = step->GetPostStepPoint();

			//const G4VProcess *pre_step = prePoint->GetProcessDefinedStep();
			const G4VProcess *post_step = postPoint->GetProcessDefinedStep();

			//const G4String& processName_pre = pre_step ? pre_step->GetProcessName() : "nn";
			const G4String& processName_post = post_step ? post_step->GetProcessName() : "nn";

			G4int subtype = post_step-> GetProcessSubType();
 			G4int nbsec = step->GetNumberOfSecondariesInCurrentStep();

			if (processName_post=="Cerenkov"){
				fEventAction->AddCerenkovEdep(edep);
			}
			else if (processName_post=="compt"){
				fEventAction->AddComptEdep(edep);
			}
			else if (processName_post=="CoupledTransportation"){
				fEventAction->AddCoupledTEdep(edep);
			}
			else if (processName_post=="eBrem"){
				fEventAction->AddeBremEdep(edep);
			}
			else if (processName_post=="eIoni"){
				fEventAction->AddeIoniEdep(edep);
				//G4cout << pName << "\t" << processName_pre << "\t" << processName_post << "\t" << edep/keV << G4endl;
			}
			else if (processName_post=="msc"){
				fEventAction->AddMSCEdep(edep);
			}
			else if (processName_post=="muIoni"){
				fEventAction->AddmuIoniEdep(edep);
			}
			else if (processName_post=="OpAbsorption"){
				fEventAction->AddOpAbsEdep(edep);
			}
			else if (processName_post=="phot"){
				fEventAction->AddPhotEdep(edep);
				//G4cout << pName << "\t" << processName_pre << "\t" << processName_post << "\t" << edep/keV << G4endl;
				G4int subtype = post_step-> GetProcessSubType();
				G4cout << subtype << G4endl;
			}
			//else if ((subtype == 2)&&(nbsec == 0)){
			//	fEventAction->AddEdepAlone(edep);
			//}
			else {
				G4cout << processName_post << "\t" << edep/keV << "\t" << G4endl;
			}

			if (processName_post!="OpAbsorption"){
				fEventAction->AddEdep(edep);
			}
		}
		//G4cout << processName_pre << "\t" << processName_post << "\t" << edep/keV << G4endl;
	}
}