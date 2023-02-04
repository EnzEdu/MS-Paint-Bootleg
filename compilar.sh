#!/bin/bash
#find . -maxdepth 1 -name 'CG.a15_EnzoEduardo.cpp' -execdir g++ -o paint '{}' -lGL -lGLU -lglut \;
#./paint

g++ -o paint CG.a15_EnzoEduardo.cpp -lGL -lGLU -lglut
./paint

#g++ -o teste teste.cpp
#./teste