all: tarlib

tarlib: main.o microtar.o tarlib.o
	g++ main.o microtar.o tarlib.o -o tarlib

main.o: main.cpp
	g++ -c main.cpp

microtar.o: microtar.c
	gcc -c microtar.c

tarlib.o: tarlib.cpp
	g++ -c tarlib.cpp

clean:
	rm -rf *.o tarlib