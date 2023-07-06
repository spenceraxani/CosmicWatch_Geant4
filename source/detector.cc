#include "detector.hh"

MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
	//G4Track *track = step->GetTrack();

	//position where the photon enters the detector
	G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
	//G4ThreeVector posPhoton = preStepPoint->GetPosition();

	const G4VProcess *CurrentProcess = preStepPoint->GetProcessDefinedStep();

	if (CurrentProcess != 0) {
		const G4String & StepProcessName = CurrentProcess->GetProcessName();
		//G4cout << StepProcessName << G4endl;
	}

	return true;
}