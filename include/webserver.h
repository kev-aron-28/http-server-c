#ifndef WEBSERVER_H
#define WEBSERVER_H

typedef struct header {
  char method[16];
  char path[256];
  char version[16];
  char host[256];
} HttpHeader;

void initHttpHeader(HttpHeader *header);
void parseRequest(const char *request, HttpHeader *header);


#endif
