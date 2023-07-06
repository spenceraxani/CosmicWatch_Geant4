#include "stepAction.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
	fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step* step)
{
	//G4Track *track = step->GetTrack();
	//Get current volume
	G4LogicalVolume *currentVolume = step->GetPreStepPoint()
									->GetTouchableHandle()
									->GetVolume()
									->GetLogicalVolume();

	G4String CurrentVolName = currentVolume->GetName();

	const MyDetectorConstruction *detectorConstruction = 
			static_cast<const MyDetectorConstruction*> 
			(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

	//Get scoring volume
	G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();
	G4String ScoringVolName = fScoringVolume->GetName();

	//Compare current and scoring volume
	if(currentVolume != fScoringVolume){return;}

	//G4cout << CurrentVolName << G4endl;

	G4double edep = step->GetTotalEnergyDeposit();
	fEventAction->AddEdep(edep);
}