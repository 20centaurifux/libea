all:
	g++ -c ./src/*.cpp -Wall -g
	g++ *.o -o ea

clean:
	rm -f *.o
	rm -f ea
