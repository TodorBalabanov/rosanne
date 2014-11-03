#!/bin/bash
mkdir Default
#astyle *.cpp *.h --indent=force-tab --style=java / -A2 --recursive
#rm *.orig
cd Default
mkdir log
g++ ../*.cpp -c -Wwrite-strings `wx-config --cxxflags` `wx-config --libs`
g++ *.o -o rosanne `wx-config --cxxflags` `wx-config --libs`
wxrc ../*.xrc -o gui.xrs
ulimit -c unlimited
./rosanne

