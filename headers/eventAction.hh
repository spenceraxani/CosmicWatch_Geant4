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

	void AddEdep(G4double edep){fEdep+=edep;}

private:
	G4double fEdep;
};

#endif