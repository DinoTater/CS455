// Adam Berenter and Mike Marelli
// CS 455
// Project 1: Protocols and Encodings

#include "header.h"

struct hostent *hp;              
struct sockaddr_in  server_addr; 
int sock, r;
int SERVER_IP, SERVER_PORT; 

// Initialize client
int client_init(char *argv[])
{
	char servname[MAX], servport[MAX];
	
	printf("-------------------- client  init --------------------\n");
	
	// assign server name and port from command line
	strcpy(servname, argv[1]); 
	strcpy(servport, argv[2]); 

  	printf("Retrieving server info...\n");
  	hp = gethostbyname(servname);
  	if (!hp)
	{
     	printf("Error: Unknown host %s.\n", argv[1]);
     	exit(1);
  	}

  	SERVER_IP = *(long *)hp->h_addr;
  	SERVER_PORT = atoi(servport);

  	printf("Creating a TCP socket...\n");
  	sock = socket(AF_INET, SOCK_STREAM, 0);
  	if (sock < 0)
	{
  	   printf("Error: Failed to create socket.\n");
  	   exit(2);
  	}

  	printf("Populating server_addr with server's IP and port#...\n");
  	server_addr.sin_family = AF_INET;
  	server_addr.sin_addr.s_addr = SERVER_IP;
  	server_addr.sin_port = htons(SERVER_PORT);

  	// Connect to server
 	printf("Connecting to server...\n");
 	r = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
 	if (r < 0)
	{
     	printf("Error: Connect failed\n");
     	exit(1);
  	}

  	printf("Successfully connected to \007\n"); 
  	printf("\thostname=localhost  IP=127.0.0.1  port=%d\n", SERVER_PORT);  

  	printf("------------------ client init done ------------------\n");
}

// MAIN
main(int argc, char *argv[ ])
{
	int n;                             // num bytes sent/recvd
	uint16_t passlen, final_msglen, nbo;
	char inbuf[MAX];                   // multipurpose inbuffer
	char id[MAX], name[MAX], password[MAX], final_msg[MAX];   

	if (argc < 3)
	{
		printf("Usage: ./client serverName serverPort\n");
		exit(1);
	}

	// initialize client
	client_init(argv);
	
	while (1)
	{
		// recv the welcome message in inbuf
		n = recv_all(sock, inbuf, MAX-1, 0);
		inbuf[n] = 0;   // null terminate for printing

		// check if inbuf is welcome message
		if (strcmp(inbuf, "Welcome"))
		{
	 		// close connection, print error, and exit
	 		printf("ERROR! UNEXPECTED RESPONSE FROM SERVER!\n");
	 		printf("Exiting...");
	 		exit(1);
		}

		// Run calls here
		while (!noMoreCommands)
		{

		}

		// close socket		
		close(sock);

		// exit
		exit(0);
	}
}