#include "eventAction.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
	fEdep = 0;

	fCerenkovEdep = 0;
	fComptEdep = 0;
	fCoupledTEdep = 0;
	feBremEdep = 0;
	feIoniEdep = 0;
	fMSCEdep = 0;
	fOpAbsorptionEdep = 0;
	fPhotEdep = 0;
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event*)
{
	fEdep = 0;

	fCerenkovEdep = 0;
	fComptEdep = 0;
	fCoupledTEdep = 0;
	feBremEdep = 0;
	feIoniEdep = 0;
	fMSCEdep = 0;
	fOpAbsorptionEdep = 0;
	fPhotEdep = 0;
}

void MyEventAction::EndOfEventAction(const G4Event* event)
{
	G4int eventID = event->GetEventID();

	/*G4cout << "OpAbsortion" << "\t" << fOpAbsorptionEdep/keV << G4endl;
	G4cout << "Phot" << "\t" << fPhotEdep/keV << G4endl;
	G4cout << "eIoni" << "\t" << feIoniEdep/keV << G4endl;

	G4cout << "Tot energy deposit except OpAbs" << "\t" << (feIoniEdep+fPhotEdep)/keV << G4endl;
	G4cout << "tot edep" << "\t" << eventID << "\t" << fEdep/keV << G4endl;*/


	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->FillNtupleIColumn(0, 0, eventID);
	man->FillNtupleDColumn(0, 1, fEdep/keV);
	man->FillNtupleDColumn(0, 2, fCerenkovEdep/keV);
	man->FillNtupleDColumn(0, 3, fComptEdep/keV);
	man->FillNtupleDColumn(0, 4, fCoupledTEdep/keV);
	man->FillNtupleDColumn(0, 5, feBremEdep/keV);
	man->FillNtupleDColumn(0, 6, feIoniEdep/keV);
	man->FillNtupleDColumn(0, 7, fMSCEdep/keV);
	man->FillNtupleDColumn(0, 8, fOpAbsorptionEdep/keV);
	man->FillNtupleDColumn(0, 9, fPhotEdep/keV);
	man->AddNtupleRow(0);
}