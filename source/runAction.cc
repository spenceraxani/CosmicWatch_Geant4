#include "runAction.hh"

MyRunAction::MyRunAction()
{
	//create analysis manager
	G4AnalysisManager* man = G4AnalysisManager::Instance();

	//Create ntuple
	man->CreateNtuple("Event", "Event");
	man->CreateNtupleIColumn("Event");
	man->CreateNtupleDColumn("E");
	man->FinishNtuple(0);
}

MyRunAction::~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
	//get run ID to print to a different file
	G4int runID = run->GetRunID();

	std::stringstream strRunID;
	strRunID << runID;

	G4AnalysisManager* man = G4AnalysisManager::Instance();
	man->OpenFile("../data/run"+strRunID.str()+".csv");
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager* man = G4AnalysisManager::Instance();

	man->Write();
	man->CloseFile();
}