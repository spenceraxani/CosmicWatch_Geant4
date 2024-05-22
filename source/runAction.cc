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
	man->CreateNtupleDColumn("Tot - OpAbs");
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

	//man->SetNtupleFileName(0, "../data/run"+strRunID.str()+".csv");
	//man->SetNtupleFileName(1, "../data/run"+strRunID.str()+"_SiPMHits.csv");
	//man->OpenFile("../data/run"+strRunID.str()+".csv");
	//man->OpenFile("../data/run"+strRunID.str()+"_SiPMHits.csv");
	//man->Reset();
	//man->OpenFile("../data/run"+strRunID.str()+"-ang_dis.csv");
	man->OpenFile("../data/LYSO_0,5doping_run"+strRunID.str()+"_3x3x20-base.csv");
	
	//man->OpenFile("../data/vis_data.csv");
}

void MyRunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager* man = G4AnalysisManager::Instance();

	man->Write();
	man->CloseFile();
}