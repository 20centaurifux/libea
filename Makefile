.PHONY: clean all

all:
	ctags -R .
	g++ ./src/*.cpp -o foo -std=c++11 -Wall -g -O0 -DTHREAD_SAFE

test: src/test.cpp
	g++ ./src/test.cpp ./src/ARandomNumberGenerator.cpp -o test -std=c++11 -Wall -g -O0 -lcppunit
