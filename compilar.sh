#!/bin/bash
find . -maxdepth 1 -name '*.cpp' -execdir g++ -o paint '{}' -lGL -lGLU -lglut \;
./paint