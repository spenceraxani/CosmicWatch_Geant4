# CosmicWatch_Geant4

This repository contains a simple Geant4 project simulating a CosmicWatch detector. I will work on a quick guide on how to install Geant4 on Linux, on the mean time you can find a good installation and Geant4 basics tutorial on this [tutorial series](https://www.youtube.com/playlist?list=PLLybgCU6QCGWgzNYOV0SKen9vqg4KXeVL) by [Physics Matters](https://www.youtube.com/@physics_matters) on YouTube. Here we are using python instead of root to analyse the data.

## Structure

### headers

The `headers` folder contains all `C++` header files.

### source

The `source` folder contains all function declarations for the header files in the `headers` folder.

## Run

In order to run, first create a build folder, inside it run `cmake ..` and then `make`. If everything works fine you should have an executable file called `cosmic_watch`, execute it by running `./cosmic_watch`, this will open OpneGL gui where you can run commands from the command line. If you do not want to run visualization and run another macro intead, simply run `./cosmic_watch macro-name.mac`.