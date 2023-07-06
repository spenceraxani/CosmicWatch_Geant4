#ifndef RUN_ACTION
#define RUN_ACTION

#include "G4Run.hh"
#include "G4GenericAnalysisManager.hh"

#include "G4UserRunAction.hh"

using G4AnalysisManager = G4GenericAnalysisManager;

class MyRunAction : public G4UserRunAction
{
public:
  MyRunAction();
  virtual ~MyRunAction();

public:
  //virtual G4Run* GenerateRun();
  virtual void BeginOfRunAction(const G4Run* run);
  virtual void EndOfRunAction(const G4Run*);
};

#endif