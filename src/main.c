#include <stdio.h>
#include <stdlib.h>
#include "webserver.h"
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

void helloController(int clientFD, const HttpHeader * request);

int main(int argc, char const *argv[])
{
  HttpServerInstance server = getInstance(3000);

  httpGET(&server, "/hello", helloController);

  startHttpServer(server);

  return 0;
}

void helloController(int clientFD, const HttpHeader * request) {
  const char* response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 48\r\n"
        "\r\n"
        "<html><body><h1>Welcome to Home!</h1></body></html>";

  send(clientFD, response, strlen(response), 0);
}
