#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4MTRunManager.hh"

#include "G4VSensitiveDetector.hh"

class MySensitiveDetector : public G4VSensitiveDetector
{
public:
	MySensitiveDetector(G4String);
	~MySensitiveDetector();

private:
	virtual G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist);
};

#endif