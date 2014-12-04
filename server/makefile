all: p2pserver

p2pserver: p2pserver.o
	gcc p2pserver.o -o p2pserver

p2pserver.o: p2pserver.c
	gcc -c p2pserver.c

clean:
	rm -f *.o p2pserver *~
