#!/bin/sh

g++ MD.cpp Timer.cpp -o MD
mpic++ MDmpi.cpp Timer.cpp -fopenmp -o MDmpi
javac MDout.java






