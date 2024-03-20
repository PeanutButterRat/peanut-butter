#!/bin/bash

cd build
ctest

read -n 1 -s -r -p "Press any key to continue..."
echo