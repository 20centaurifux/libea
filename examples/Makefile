CXX?=g++
override CXXFLAGS+=-std=c++17 -Wall -O3
LDFLAGS=-fopenmp
INCLUDE=-I../src

.PHONY: all clean

all:
	$(CXX) $(CXXFLAGS) $(INCLUDE) ./tsp.cpp -o ./tsp $(LDFLAGS)

clean:
	rm -f ./tsp
