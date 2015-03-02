CC=gcc
CXX=g++
CPPCHECK=cppcheck

all: build
test: lint
	$(CXX) -o algoflex_test -Wall -g --std=c++11 src/interface.cpp
build:
	$(CXX) -o algoflex --std=c++11 src/interface.cpp
	
lint:
	$(CPPCHECK) --error-exitcode=2 --enable=all --language=c++ --std=c++11 --suppress=*:lib/codestring.cpp:46 src/interface.cpp
	python cpplint.py --filter=-legal,-runtime/printf,-runtime/references lib/*
	python cpplint.py --filter=-legal,-whitespace/line_length,-runtime/references src/*
clean:
	rm algoflex algoflex_test
