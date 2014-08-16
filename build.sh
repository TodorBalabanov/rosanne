#!/bin/bash
mkdir bin
cd bin
mkdir log
g++ ../*.cpp -c -Wwrite-strings `wx-config --cxxflags` `wx-config --libs`
g++ *.o -o ra.exe `wx-config --cxxflags` `wx-config --libs`
wxrc ../*.xrc -o gui.xrs
./ra.exe

