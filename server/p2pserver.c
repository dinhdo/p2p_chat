/*
	Created by Kim Ngo and Dinh Do
	Dec. 4, 2014
	
	p2pserver.c
	
	Server portion of a P2P talk. Clients interact with server to communicate with other users on the server through a UDP protocol
	
	Server usage: ./p2pserver port#
		Sends response message from rendezvous server to client
		S -- success
		F -- failure
		List of group members and their IP address list[Username:Address:]
		user_name: -- name of user in group terminated by ":"
		ip_address: -- 32-bit integer in network byte order terminated by ":"
		sample_message: -- message terminates with "::" to mark the last group member

*/

#include <stdio.h>#include <stdlib.h>#include <unistd.h>#include <errno.h>
#include <string.h>#include <sys/types.h>#include <sys/socket.h>#include <netinet/in.h>#include <arpa/inet.h>#include <netdb.h>
#include <time.h>
#include <sys/time.h>


