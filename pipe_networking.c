#include "pipe_networking.h"

struct addrinfo *get_addrinfo(char *node, char *service) {
  struct addrinfo hints = {0};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  if (!node) {
    hints.ai_flags = AI_PASSIVE;
  }
  struct addrinfo *results;
  getaddrinfo(node, service, &hints, &results);
  return results;
}

/*=========================
  server_setup
  args:

  creates the WKP (upstream) and opens it, waiting for a
  connection.

  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/

int server_setup() {
  struct addrinfo *results = get_addrinfo(NULL, "2187");
  printf("Server: got info for socket\n");
  int from_client = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  printf("Server: created socket %d\n", from_client);
  bind(from_client, results->ai_addr, results->ai_addrlen);
  printf("Server: binded socket %d\n", from_client);
  listen(from_client, 10);
  printf("Server: listening to socket %d\n", from_client);
  freeaddrinfo(results);
  return from_client;
}

/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/

int server_connect(int from_client) {
  struct sockaddr_storage client_address;
  socklen_t sock_size = sizeof client_address;
  int to_client = accept(from_client, (struct sockaddr *) &client_address, &sock_size);
  printf("Server: just accepted socket %d through socket %d\n", from_client, from_client);
  return to_client;
}

/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/

int client_connect() {
  struct addrinfo *results = get_addrinfo("127.0.0.1", "2187");
  printf("Client: just got info for socket\n");
  int to_server = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  printf("Client: created socket %d\n", to_server);
  connect(to_server, results->ai_addr, results->ai_addrlen);
  printf("Client: just connected socket %d\n", to_server);
  freeaddrinfo(results);
  return to_server;
}
