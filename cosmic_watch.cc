#include <iostream>

#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"

#include "G4EmCalculator.hh"

#include "construction.hh"
#include "physics.hh"
#include "action.hh"

int main(int argc, char** argv){

	G4MTRunManager* runManager = new G4MTRunManager();

	runManager->SetUserInitialization(new MyPhysicsList);
	runManager->SetUserInitialization(new MyDetectorConstruction);
	runManager->SetUserInitialization(new MyActionInitialization);

	//runManager->Initialize();

	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	//visualization
	if(argc == 1){
		G4UIExecutive *ui = new G4UIExecutive(argc, argv);
		G4VisManager *visManager = new G4VisExecutive();
		visManager->Initialize();

		UImanager->ApplyCommand("/control/execute vis.mac");
		ui->SessionStart();
		delete visManager;
		delete ui;
	}
	//batch mode
	else{
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);

		std::stringstream x;
		std::stringstream y;

		G4int range = 48;
		for(G4int i = -range; i <= range; i+=2){
			for(G4int j = -range; j <= range; j+=2){
				x << i;
				y << j;
				UImanager->ApplyCommand("/gps/pos/centre "+x.str()+" "+y.str()+" 5 mm");
				UImanager->ApplyCommand("/run/beamOn 1");
			}
		} 
	}

	delete runManager;

	return 0;
}
