#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4MTRunManager.hh"

#include "G4OpticalPhoton.hh"

#include "G4VSensitiveDetector.hh"

#include "runAction.hh"

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
	MySensitiveDetector(G4String name);
	~MySensitiveDetector();

public:
	G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist);
	void Initialize(G4HCofThisEvent* HCE);
	void EndOfEvent(G4HCofThisEvent* HCE);

private:

	G4int fNumberOfOpticalPhotons = 0;
};

#endif