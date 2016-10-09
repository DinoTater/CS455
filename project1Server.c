// Adam Berenter and Mike Marelli
// CS 455
// Project 1: Protocols and Encodings

#include "header.h"

struct sockaddr_in serv_addr, cli_addr, tmp_addr;  // struct containing addr
struct hostent *hp;   							   // defines host comp

int sock, comsock;  // sock = server sock, comsock = client communication socket
int servPort;       // server port number
int res, len, n;    // help variables


// Initialize the server.
int server_init(char* argv[])
{
	struct in_addr **addr_list; // janky way to get host addr

	printf("-------------------  server  init  -------------------\n");

	printf("Retrieving hostname and IP...\n");
	hp = gethostbyname("localhost");
	if (!hp)
	{
		printf("Error: Unknown host.");
		exit(1);
	}
	addr_list = (struct in_addr**)hp->h_addr_list;
	printf("\thostname=localhost  IP=%s\n", inet_ntoa(*addr_list[0]));

	printf("Creating a TCP socket...\n");
	
	// domain=>IPv4, type=>stream, protocol=>OS choose TCP for stream socket
	sock = socket(AF_INET, SOCK_STREAM, 0); 
	if (sock < 0)
	{
		printf("Error: Failed to create socket.\n");
		exit(2);
	}

	printf("Populating serv_addr with IP and Port#...\n");
	serv_addr.sin_family = AF_INET;                  // for TCP
   	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);   // serv IP address  

   	// assign port from command line arg
   	serv_addr.sin_port = htons((uint16_t) strtoul(argv[1], NULL, 0));

	printf("Binding socket to serv_addr info...\n"); 
	res = bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	if (res < 0)
	{
		printf("Error: Failed to bind.\n");
		exit(3);
	}

	printf("Finding kernel assigned Port#...\n");
	len = sizeof(tmp_addr);
	// get current addr to where sock is bound
	res = getsockname(sock, (struct sockaddr *)&tmp_addr, &len);
   	if (res < 0)
   	{
		printf("Error: getsockname() failed.\n");
		exit(4);
   	}

   	servPort = ntohs(tmp_addr.sin_port);
   	printf("\tPort=%d\n", servPort);

   	db_populate();
   	
   	printf("------------------ server init done ------------------\n");
}

// MAIN
main(int argc, char *argv[])
{
	char* msg;
	int n, found_pair, is_valid;  
	uint16_t passlen, finalmsglen;
	char outbuf[MAX], id[MAX], name[MAX], password[MAX]; 

	if (argc < 2)
	{
		printf("Usage: ./server serverPort\n");
		exit(1);
	}

	// initialize server
	server_init(argv);

	// while still serving (do not quit until Ctrl-C)
	while(1) 
	{
		// listen at serverPort 
   		listen(sock, QUEUE_MAX);
		printf("\n\nServer is listening...\n");

		// try to accept a client connection 
		len = sizeof(cli_addr);
		comsock = accept(sock, (struct sockaddr *)&cli_addr, &len);
		if (comsock < 0)
		{
			printf("Error: Failed to accept connection.\n");
			exit(1);
		}

		printf("Server accepted connection from\n");
		printf("-----------------------------------------------\n");
     	printf("\tIP=%s   port=%d\n", (char*)inet_ntoa(cli_addr.sin_addr),
            ntohs(cli_addr.sin_port));
     	printf("-----------------------------------------------\n");

     	// while still talking to client
     	while(1)
     	{
	     	// send welcome msg as newline terminated string
	     	printf("Creating and sending welcome msg...");
	     	msg = "Welcome";

	     	bzero(outbuf, MAX);  
	     	strncpy(outbuf, msg, strlen(msg));
	     	outbuf[strlen(msg)] = '\n';

	     	n = send_all(comsock, outbuf, strlen(outbuf), 0);  
	     	printf(" %d bytes sent\n", n);

			// close connection
			close(comsock);

			break;
     	}
	}

    return 0;
}

/*

// Sample server usage from Donahoo and Calvert
#define RCVBUFSIZE 32   // Size of receive buffer

void DieWithError(char *errorMessage);   // Error handling function

void HandleTCPClient(int clntSocket)
{
    char echoBuffer[RCVBUFSIZE];        // Buffer for echo string
    int recvMsgSize;                    // Size of received message

    // Receive message from client
    if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed");

    // Send received string and receive again until end of transmission 
    while (recvMsgSize > 0)      //zero indicates end of transmission 
    {
        // Echo message back to client
        if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
            DieWithError("send() failed");

        // See if there is more data to receive
        if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
            DieWithError("recv() failed");
    }

    close(clntSocket);    // Close client socket
}*/