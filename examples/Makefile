CPP=g++
CPPFLAGS=-std=c++17 -Wall -O3
LDFLAGS=-fopenmp
INCLUDE=-I../src

.PHONY: all clean

all:
	$(CPP) $(CPPFLAGS) $(INCLUDE) ./tsp.cpp -o ./tsp $(LDFLAGS)

clean:
	rm -f ./tsp
