CC=gcc
CXX=g++

all: build
test: build
	
build:
	$(CXX) -o algoflex src/interface.cpp
