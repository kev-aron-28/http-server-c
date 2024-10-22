#ifndef WEBSERVER_H
#define WEBSERVER_H

#define MAX_ROUTES 10

typedef struct header {
  char method[16];
  char path[256];
  char version[16];
  char host[256];
} HttpHeader;

typedef void (*RouteHandler)(int client_fd, const HttpHeader *request);

typedef struct {
  char method[8];
  char path[256];
  RouteHandler handler; 
} Route;

typedef struct routesTable {
  Route routes[MAX_ROUTES];
  int routesCounter;
} RoutesTable;

typedef struct {
  RoutesTable table;
  char publicRoute[250];
  int port;
} HttpServerInstance;

HttpServerInstance getInstance(int port);
void initRoutesTable(HttpServerInstance * serverInstance);

void initHttpHeader(HttpHeader *header);
void parseRequest(const char *request, HttpHeader *header);
void handleRequest(int clientFD, const HttpHeader *header, HttpServerInstance serverInstance);
int startHttpServer(HttpServerInstance instance);

void httpGET(HttpServerInstance *server, const char *path, RouteHandler handler);

#endif
