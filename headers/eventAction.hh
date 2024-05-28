#ifndef EVENT_HH
#define EVENT_HH

#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
//#include "G4BestUnit.hh"

#include "G4UserEventAction.hh"

#include "runAction.hh"

class MyEventAction : public G4UserEventAction
{
public:
	MyEventAction(MyRunAction*);
	~MyEventAction();
	virtual void BeginOfEventAction(const G4Event*);
	virtual void EndOfEventAction(const G4Event* event);

	void SetParticleMomDir(G4ThreeVector momDir){ParticleMomDir=momDir;}

	void AddEdep(G4double edep){fEdep+=edep;}

	void AddCerenkovEdep(G4double edep){fCerenkovEdep+=edep;}
	void AddComptEdep(G4double edep){fComptEdep+=edep;}
	void AddCoupledTEdep(G4double edep){fCoupledTEdep+=edep;}
	void AddeBremEdep (G4double edep){feBremEdep+=edep;}
	void AddeIoniEdep (G4double edep){feIoniEdep+=edep;}
	void AddMSCEdep(G4double edep){fMSCEdep+=edep;}
	void AddmuIoniEdep(G4double edep){fmuIoniEdep+=edep;}
	void AddOpAbsEdep (G4double edep){fOpAbsorptionEdep+=edep;}
	void AddPhotEdep (G4double edep){fPhotEdep+=edep;}
	void AddPhoton (){fNumberOfOpticalPhotons+=1;}
	void AddScintPhoton (){fScintPhot+=1;}

private:
	G4ThreeVector ParticleMomDir;

	G4double fEdep;
	G4double fCerenkovEdep;
	G4double fComptEdep;
	G4double fCoupledTEdep;
	G4double feBremEdep;
	G4double feIoniEdep;
	G4double fMSCEdep;
	G4double fmuIoniEdep;
	G4double fOpAbsorptionEdep;
	G4double fPhotEdep;
	G4int fNumberOfOpticalPhotons;
	G4int fScintPhot;
};

#endif