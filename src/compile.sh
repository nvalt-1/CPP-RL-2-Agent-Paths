#!/bin/bash

echo "compiling..."

echo "g++ -c position.cpp worldconfig.cpp rlstate.cpp qtable.cpp agent.cpp world.cpp clidisplay.cpp pdworld.cpp $2"

g++ -c position.cpp worldconfig.cpp rlstate.cpp qtable.cpp agent.cpp world.cpp clidisplay.cpp pdworld.cpp $2

echo "g++ position.o worldconfig.o rlstate.o qtable.o agent.o world.o clidisplay.o pdworld.o -o $1 $2"

g++ position.o worldconfig.o rlstate.o qtable.o agent.o world.o clidisplay.o pdworld.o -o $1 $2
