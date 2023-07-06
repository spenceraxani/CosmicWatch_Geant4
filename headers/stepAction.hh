#ifndef STEPPING_HH
#define STEPPING_HH

#include "G4Step.hh"
#include "G4Track.hh"

#include "G4UserSteppingAction.hh"

#include "construction.hh"
#include "eventAction.hh"

class MySteppingAction : public G4UserSteppingAction
{
public:
	MySteppingAction(MyEventAction *eventAction);
	virtual ~MySteppingAction();

	virtual void UserSteppingAction(const G4Step* step);

private:
	MyEventAction *fEventAction;
};

#endif