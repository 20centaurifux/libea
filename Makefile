CC=g++
CPPFLAGS=-std=gnu++0x -Wall -O2

SRCS=./src/ARandomNumberGenerator.cpp
OBJS=$(SRCS:.cpp=.o)

MAJOR_VERSION=0
MINOR_VERSION=1
PATCHLEVEL=0

.PHONY: depend clean

all: $(OBJS)
	ar rcs libea.a $(OBJS)
	$(CC) -shared -Wl,-soname,libea.so.0 -o libea-$(MAJOR_VERSION).$(MINOR_VERSION).$(PATCHLEVEL).so $(OBJ)

cpp.o:
	$(CC) $(CPPFLAGS) -c $< -o $@

test:
	$(CC) ./src/test.cpp src/ARandomNumberGenerator.cpp -o ./test -lstdc++ -lcppunit -ldl -lm -std=c++11 -g

doc:
	doxygen ./doxygen

clean:
	rm -fr ./src/*.o
	rm -fr ./doc
	rm -fr ./test
	rm -f ./libea.a
	rm -f ./libea-$(MAJOR_VERSION).$(MINOR_VERSION).$(PATCHLEVEL).so
