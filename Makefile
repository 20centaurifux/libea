CXX?=g++
override CXXFLAGS+=-std=c++17 -Wall -O2
LDFLAGS=-lcppunit -fopenmp

.PHONY: test doc clean

test:
	$(CXX) $(CXXFLAGS) -I./src ./test/test.cpp -o  ./libea-test $(LDFLAGS)

doc:
	doxygen ./doxygen_config

clean:
	rm -fr ./libea-test ./doc
