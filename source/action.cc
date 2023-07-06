#include "action.hh"

MyActionInitialization::MyActionInitialization()
{}

MyActionInitialization::~MyActionInitialization()
{}

void MyActionInitialization::BuildForMaster() const
{
	MyRunAction *RunAction = new MyRunAction();
	SetUserAction(RunAction);
}

void MyActionInitialization::Build() const
{
	MyPrimaryGeneratorAction *generatorAction = new MyPrimaryGeneratorAction();
	SetUserAction(generatorAction);

	MyRunAction *RunAction = new MyRunAction();
	SetUserAction(RunAction);

	MyEventAction *EventAction = new MyEventAction(RunAction);
	SetUserAction(EventAction);

	MySteppingAction *steppingAction = new MySteppingAction(EventAction);
	SetUserAction(steppingAction);
}