CPP=g++
CPPFLAGS=-std=c++17 -Wall -O0 -g
LDFLAGS=-lcppunit

.PHONY: test doc clean

test:
	$(CPP) $(CPPFLAGS) -I./src ./test/test.cpp -o  ./libea-test $(LDFLAGS)

doc:
	doxygen ./doxygen_config

clean:
	rm -fr ./libea-test ./doc
