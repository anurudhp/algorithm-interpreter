CC=gcc
CXX=g++

all: build
test: 
	$(CXX) -o algoflex_test -Wall -g src/interface.cpp
build:
	$(CXX) -o algoflex src/interface.cpp
clean:
	rm algoflex algoflex_test