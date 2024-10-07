#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include "webserver.h"

#define PORT 3000
#define QUEUE_LEN 5 // Define a backlog length for pending connections

int main(int argc, char const *argv[])
{
  int socketDescriptor, clientDescriptor;
  ssize_t dataFromClient;

  struct sockaddr_in serverAddress, clientAddress;
  socklen_t addressLength = sizeof(serverAddress);

  // Create socket
  socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (socketDescriptor < 0)
  {
    perror("FAILED TO CREATE SOCKET \n");
    exit(EXIT_FAILURE);
  }

  // Configure server address
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(PORT);

  // Bind socket
  int serverBinding = bind(socketDescriptor, (struct sockaddr *)&serverAddress, addressLength);
  if (serverBinding < 0)
  {
    perror("FAILED TO BIND THE SERVER \n");
    close(socketDescriptor); // Close the socket before exiting
    exit(EXIT_FAILURE);
  }

  // Start listening for connections
  int isServerListening = listen(socketDescriptor, QUEUE_LEN);
  if (isServerListening < 0)
  {
    perror("SERVER FAILED TO LISTEN ON SELECTED PORT \n");
    close(socketDescriptor); // Close the socket before exiting
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", PORT);

  while (1)
  {
    socklen_t clientAddrLen = sizeof(clientAddress);
    clientDescriptor = accept(socketDescriptor, (struct sockaddr *)&clientAddress, &clientAddrLen);
    
    if (clientDescriptor < 0)
    {
      perror("FAILED TO ACCEPT REQUEST \n");
      continue; // Don't exit, just continue accepting new connections
    }

    char buffer[1024] = {0};

    dataFromClient = read(clientDescriptor, buffer, 1024 - 1);
    
    if(dataFromClient <= 0) {
      printf("CLIENT DESCONNECTED \n");

      break;
    }
    // TODO: BREAKDOWN HTTP HEADER 
    printf("%s \n", buffer);

    HttpHeader header;

    printf("CLIENT CONNECTED: %s:%d\n", inet_ntoa(clientAddress.sin_addr), ntohs(clientAddress.sin_port));

    // Handle client here

    close(clientDescriptor); // Close the client socket after handling
  }

  close(socketDescriptor); // Close the server socket
  return 0;
}
