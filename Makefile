CPP=g++
CPPFLAGS=-std=c++14 -Wall -O0 -g
LDFLAGS=-lcppunit

all:
	$(CPP) $(CPPFLAGS) ./src/Random.cpp ./src/test.cpp -o test $(LDFLAGS)

clean:
	rm -f ./test
