.PHONY: clean all

all:
	ctags -R .
	g++ ./src/*.cpp -o foo -std=c++11 -Wall -g -O0
