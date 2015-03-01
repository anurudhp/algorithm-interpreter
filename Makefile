CC=gcc
CXX=g++

all: build
test: 
	$(CXX) -o algoflex_test -Wall -g --std=c++11 src/interface.cpp
build:
	$(CXX) -o algoflex --std=c++11 src/interface.cpp
	
lint:
	cppcheck --error-exitcode=2 --enable=all --language=c++ --std=c++11 src/interface.cpp
	# python cpplint.py --filter=-legal lib/* src/* 
clean:
	rm algoflex algoflex_test
