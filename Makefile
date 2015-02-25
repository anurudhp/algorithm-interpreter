all: std
test: std clang
	
std:
	g++ -o algoflex src/interface.cpp	
clang:
	clang++ -stdlib=libstdc++ src/interface.cpp
