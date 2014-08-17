#!/bin/bash
mkdir Default
cd Default
mkdir log
g++ ../*.cpp -c -Wwrite-strings `wx-config --cxxflags` `wx-config --libs`
g++ *.o -o rosanne `wx-config --cxxflags` `wx-config --libs`
wxrc ../*.xrc -o gui.xrs
./rosanne

