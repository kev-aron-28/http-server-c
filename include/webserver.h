#ifndef WEBSERVER_H
#define WEBSERVER_H

typedef struct header {
  char method[10];
  char path[10];
  char version[10];
} HttpHeader;

#endif
