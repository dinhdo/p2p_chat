/*
	Created by Kim Ngo and Dinh Do
	Dec. 4, 2014
	
	p2pclient.c
	
	Client portion of a P2P talk. Interacts with a server to allow user to communicate with other users through a UDP protocol
	
	Client usage: ./p2pchat hostname:port#
	
	Operations:
		list -- lists groups on the server
		join [group name] [username] -- joins group with username
		[message text] -- type message and press enter to send
		leave -- leaves the group
		quit -- exit p2p

*/

#include <stdio.h>#include <stdlib.h>#include <unistd.h>#include <errno.h>
#include <string.h>#include <sys/types.h>#include <sys/socket.h>#include <netinet/in.h>#include <arpa/inet.h>#include <netdb.h>
#include <time.h>
#include <stdbool.h>

#define BSIZE 256 // Buffer size

// Returns true if string starts with leading substring
// Returns false otherwise
bool str_starts_with(const char pre[], const char str[]) {
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}


// Sends server ASCII character "L" to notify server to send list of groups
// Prints list of groups
int list(const int sockfd) {
	int n;
	char list_buffer[BSIZE];
	
	if ((n = write(sockfd, "L", 1)) < 0) {
		fprintf(stderr, "Error writing to socket: list\n");
		return 0;
	}
	
	if ((n = read(sockfd, list_buffer, BSIZE)) < 0) {
		fprintf(stderr, "Error reading from socket: list\n");
		return 0;
	}
	
	char *s = strtok(list_buffer, ":");
	while (s != NULL) {
		printf ("%s\n", s);
		s = strtok(NULL, ":");
	}
	return 1;
}


int join(const int sockfd, const char buffer[], char group[], char username[]) {
	printf("Still needs to implement join()\n");
}

int leave(const int sockfd, const char group[], const char username[]) {
	printf("Still needs to implement leave()\n");
	if (strlen(group) == 0) {
	
	}
}


// Sends server ASCII character "T" to notify server prepare for receiving message
// Prepends group and username to message being sent
int message(const int sockfd, const char username[], const char group[], const uint32_t id_num, char message[]) {
	int n;
	char msg_id[BSIZE];
	uint32_t message_len = strlen(message);
	
	strcpy(msg_id, group);
	strcat(msg_id, ":");
	strcat(msg_id, username);
	strcat(msg_id, ":");

	// Notifies server message type
	if ((n = write(sockfd, "T", 1)) < 0) {
		fprintf(stderr, "Error writing to socket: message T\n");
		return 0;
	}

	// Sends group_name:username:
	if ((n = write(sockfd, msg_id, strlen(msg_id))) < 0) {
		fprintf(stderr, "Error writing to socket: message msg_id\n");
		return 0;
	}
	
	// Sends unique message ID number
	if ((n = write(sockfd, &id_num, sizeof(uint32_t))) < 0) {
			fprintf(stderr, "Error writing to socket: message id_num\n");
			return 0;
	}
	
	// Sends message length
	if ((n = write(sockfd, &message_len, sizeof(uint32_t))) < 0) {
			fprintf(stderr, "Error writing to socket: message length\n");
			return 0;
	}
	
	// Sends message
	if ((n = write(sockfd, message, strlen(message))) < 0) {
			fprintf(stderr, "Error writing to socket: message\n");
			return 0;
	}
	
	return 1;
}






int main(int argc, char *argv[]) {

	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[BSIZE], group[BSIZE], username[BSIZE];
	uint32_t msg_id_num = 0;
	
	bzero(buffer, BSIZE);
	bzero(group, BSIZE);
	bzero(username, BSIZE);
	
	// Port = user input
	// Exit program otherwise
	if (argc == 3) {
		portno = atoi(argv[2]);
	} else {		printf("usage: %s [hostname]:[port]\n", argv[0]);
		exit(0);
	}
	
	// Openning socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		fprintf(stderr, "Error openning socket\n");
	}
	
	// Getting host name
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "Error, no such host\n");
		exit(0);
	}
	
	// Server information
	bzero((char*) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr, server->h_length);
	
	// Connect to port
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		fprintf(stderr, "Error connecting\n");
		exit(0);
	}

	printf("\nOperations:\n\tlist -- lists groups on the server\n\tjoin [group name] [username] -- joins group with username\n\t[message text] -- type message and press enter to send\n\tleave -- leaves the group\n\tquit -- exit p2p\n\n");

	
	// Prompts user for operation
	while(1) {
		bzero(buffer, BSIZE);
		scanf("%s", buffer);
		
		// Get list of groups on the server
		if (strcmp(buffer, "list") == 0) {
			list(sockfd);
		
		// Join group if buffer begins with "join"
		} else if (str_starts_with("join", buffer)) {
			join(sockfd, buffer, group, username);
		
		// Leaves group
		} else if (strcmp(buffer, "leave") == 0) {
			leave(sockfd, group, username);

		// Exists -- disconnects from port and exits program
		} else if (strcmp(buffer, "quit") == 0 ) {
			leave(sockfd, group, username);
			printf("Session has closed.\n");
			break;

		// Sends message
		} else {
			message(sockfd, username, group, msg_id_num, buffer);
			msg_id_num++;
		}
	}
	
	close(sockfd);
	return(0);}


