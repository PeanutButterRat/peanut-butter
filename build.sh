#!/bin/bash

# Build directory for project files
mkdir build
cd build || exit

# Generate and compilen project files
cmake ..
cmake --build . --config Release

# Create binary directories
mkdir ../bin
mkdir ../tests

# Move executables to proper directories
mv Release/cpbpl ../bin
mv Release/pbpl ../bin

read -n 1 -s -r -p "Press any key to continue..."
echo