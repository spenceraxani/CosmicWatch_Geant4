#include "generator.hh"

MyPrimaryGeneratorAction::MyPrimaryGeneratorAction()
{
	particleGun = new G4GeneralParticleSource();
}

MyPrimaryGeneratorAction::~MyPrimaryGeneratorAction()
{
	delete particleGun;

	//G4cout << "\nDeleting particle gun\n" << G4endl;
}

void MyPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	particleGun->GeneratePrimaryVertex(anEvent);
}