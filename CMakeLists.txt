cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

#project name
project(Cosmic_Watch)

#find geant4, and ui, visualization packages
find_package(Geant4 REQUIRED ui_all vis_all)

#including necessary libraries
include(${Geant4_USE_FILE})

#compile all source files (cc files)
file(GLOB sources ${PROJECT_SOURCE_DIR}/source/*.cc)

#compile all header files (hh files)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/headers ${Geant4_INCLUDE_DIR})
file(GLOB headers ${PROJECT_SOURCE_DIR}*.hh)

#add all macro files (.mac files)
file(GLOB MACRO_FILES "macros/*.mac")
file(COPY ${MACRO_FILES} DESTINATION ${PROJECT_BINARY_DIR})

#add all data files (.dat files)
file(GLOB DATA_FILES "data/*.dat")
file(COPY ${DATA_FILES} DESTINATION ${PROJECT_BINARY_DIR})

#specify executable file name
add_executable(cosmic_watch cosmic_watch.cc ${sources} ${headers})

#link libraries implemented in geant4
target_link_libraries(cosmic_watch ${Geant4_LIBRARIES})

#define target
add_custom_target(Cosmic_Watch DEPENDS scint)