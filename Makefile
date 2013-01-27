all:
	g++ -c ./src/*.cpp -Wall -g -std=c++11
	g++ *.o -o ea

clean:
	rm -f *.o
	rm -f ea
