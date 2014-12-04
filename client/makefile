all: p2pchat

p2pchat: p2pclient.o
	gcc p2pclient.o -o p2pchat

p2pclient.o: p2pclient.c
	gcc -c p2pclient.c

clean:
	rm -f *.o p2pchat *~
