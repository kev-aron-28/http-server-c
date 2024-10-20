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

int startHttpServer(int port, RoutesTable table)
{
  int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (socketDescriptor < 0)
  {
    perror("Failed to create socket");
    return -1;
  }

  struct sockaddr_in serverAddress;
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port = htons(port);

  if (bind(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
  {
    perror("Failed to bind");
    close(socketDescriptor);
    return -1;
  }

  if (listen(socketDescriptor, 5) < 0)
  {
    perror("Failed to listen");
    close(socketDescriptor);
    return -1;
  }

  printf("Server listening on port %d\n", port);

  while (1)
  {
    struct sockaddr_in clientAddress;
    socklen_t clientAddrLen = sizeof(clientAddress);
    int clientDescriptor = accept(socketDescriptor, (struct sockaddr *)&clientAddress, &clientAddrLen);
    if (clientDescriptor < 0)
    {
      perror("Failed to accept connection");
      continue; // Continue to accept other connections
    }

    char buffer[1024] = {0};
    ssize_t bytesRead = read(clientDescriptor, buffer, sizeof(buffer));
    if (bytesRead < 0)
    {
      perror("Failed to read from socket");
      close(clientDescriptor);
      continue; // Continue to accept other connections
    }

    HttpHeader header;
    initHttpHeader(&header);
    parseRequest(buffer, &header);

    handleRequest(clientDescriptor, &header, table); // Ensure handleRequest is defined

    close(clientDescriptor);
  }

  close(socketDescriptor); // This line should be after the while loop if you ever exit it
  return 0;                // This return statement will only be reached if you break out of the while loop
}

void handleRequest(int clientFD, const HttpHeader *request, RoutesTable table)
{
  for (int i = 0; i < table.routesCounter; i++)
  {
    Route currentRoute = table.routes[i];
    if (strcmp(currentRoute.method, request->method) == 0 && strcmp(currentRoute.path, request->path) == 0)
    {
      currentRoute.handler(clientFD, request);

      return;
    }
  }
}

// HTTP METHODS
void httpGET(struct routesTable *table, const char *path, RouteHandler handler)
{
  strcpy(table->routes[table->routesCounter].method, "GET");

  strcpy(table->routes[table->routesCounter].path, path);

  table->routes[table->routesCounter].handler = handler;

  table->routesCounter++;

  return;
}

void parseRequest(const char *request, HttpHeader *header)
{
  char *requestLine = strtok(strdup(request), "\r\n");

  sscanf(requestLine, "%s %s %s", header->method, header->path, header->version);

  char *line;

  while ((line = strtok(NULL, "\r\n")) != NULL)
  {
    if (strncmp(line, "Host:", 5) == 0)
    {
      sscanf(line, "Host: %s", header->host);
    }
  }
}

void initHttpHeader(HttpHeader *header)
{
  memset(header->method, 0, sizeof(header->method));

  memset(header->path, 0, sizeof(header->path));

  memset(header->version, 0, sizeof(header->version));

  memset(header->host, 0, sizeof(header->host));
}
