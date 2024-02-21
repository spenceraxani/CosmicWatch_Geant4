#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{
	fNumberOfOpticalPhotons = 0;
}

MySensitiveDetector::~MySensitiveDetector()
{}


void MySensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{
	/*fNumberOfOpticalPhotons = 0;
	G4cout << "fNumberOfOpticalPhotons = " << fNumberOfOpticalPhotons << G4endl;*/
}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
	G4Track *track = aStep->GetTrack();
	const G4ParticleDefinition* particle = track->GetDefinition();

	//position where the photon enters the detector
	//G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	//G4ThreeVector posPhoton = preStepPoint->GetPosition();

	//const G4VProcess *CurrentProcess = preStepPoint->GetProcessDefinedStep();

	//if (CurrentProcess != 0) {
	//	const G4String & StepProcessName = CurrentProcess->GetProcessName();
		//G4cout << StepProcessName << G4endl;
	//}

	/*if(particle == G4OpticalPhoton::OpticalPhotonDefinition() && aStep->IsFirstStepInVolume()){
		G4int id = track->GetTrackID();
		G4int idP = track->GetParentID();
		track->SetTrackStatus(fStopAndKill);

		G4cout << "optical photon in SiPM\t" << id << "\t" << idP << G4endl;
		fNumberOfOpticalPhotons += 1;
	}*/

	return true;
}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent* HCE)
{
	//G4cout << "fNumberOfOpticalPhotons = " << fNumberOfOpticalPhotons << G4endl;
	//G4AnalysisManager* man = G4AnalysisManager::Instance();
	//man->FillNtupleIColumn(1, 0, fNumberOfOpticalPhotons);
	//man->AddNtupleRow(1);
}