#include "eventAction.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
	fEdep = 0;
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
	fEdep = 0;
}

void MyEventAction::EndOfEventAction(const G4Event* event)
{
	G4int eventID = event->GetEventID();

	G4cout << eventID << "\t" << fEdep/keV << G4endl;

	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->FillNtupleIColumn(0, 0, eventID);
	man->FillNtupleDColumn(0, 1, fEdep/keV);
	man->AddNtupleRow(0);
}