#include "pipe_networking.h"


/*=========================
  server_setup
  args:

  creates the WKP (upstream) and opens it, waiting for a
  connection.

  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/

struct addrinfo *server_addrinfo(char *service) {
  struct addrinfo hints = {0};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  struct addrinfo *results;
  getaddrinfo(NULL, service, &hints, &results);
  return results;
}

int server_setup() {
  struct addrinfo *results = server_addrinfo("2187");
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

struct addrinfo *client_addrinfo(char *node, char *service) {
  struct addrinfo hints = {0};
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  struct addrinfo *results;
  getaddrinfo(node, service, &hints, &results);
  return results;
}

int client_connect() {
  struct addrinfo *results = client_addrinfo("127.0.0.1", "2187");
  printf("Client: just got info for socket\n");
  int from_server = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
  printf("Client: created socket %d\n", from_server);
  connect(from_server, results->ai_addr, results->ai_addrlen);
  printf("Client: just connected socket %d\n", from_server);
  freeaddrinfo(results);
  return from_server;
}
