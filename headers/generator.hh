#ifndef GENERATOR_HH
#define GENERATOR_HH

#include "G4GeneralParticleSource.hh"

#include "G4VUserPrimaryGeneratorAction.hh"

class MyPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	MyPrimaryGeneratorAction();
	~MyPrimaryGeneratorAction();

	virtual void GeneratePrimaries(G4Event*);

private:
	G4GeneralParticleSource* particleGun;
};

#endif