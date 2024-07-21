#include "runAction.hh"

MyRunAction::MyRunAction()
{
	//create analysis manager
	G4AnalysisManager* man = G4AnalysisManager::Instance();

	//Create ntuple
	man->CreateNtuple("Event", "Event");
	man->CreateNtupleIColumn("Event");
	man->CreateNtupleDColumn("Px");
	man->CreateNtupleDColumn("Py");
	man->CreateNtupleDColumn("Pz");
	man->CreateNtupleDColumn("Tot-OpAbs");
	man->CreateNtupleDColumn("Cerenkov");
	man->CreateNtupleDColumn("compt");
	man->CreateNtupleDColumn("CoupledTransportation");
	man->CreateNtupleDColumn("eBrem");
	man->CreateNtupleDColumn("eIon");
	man->CreateNtupleDColumn("msc");
	man->CreateNtupleDColumn("muIoni");
	man->CreateNtupleDColumn("OpAbsorption");
	man->CreateNtupleDColumn("phot");
	man->CreateNtupleIColumn("NOfOptPhotons");
	man->CreateNtupleIColumn("NOfScintPhotons");
	man->FinishNtuple();

	//man->CreateNtuple("NOfOptPhotons", "NOfOptPhotons");
	//man->CreateNtupleIColumn("NOfOptPhotons");
	//man->FinishNtuple();
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

	G4String data_folder = "../data/test/";
	man->OpenFile(data_folder+"run"+strRunID.str()+".csv");
	
	//man->OpenFile("../data/test_data.csv");
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager* man = G4AnalysisManager::Instance();

	man->Write();
	man->CloseFile();
}