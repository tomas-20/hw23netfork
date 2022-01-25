#include "pipe_networking.h"

void get_input(char *string, int length) {
  fgets(string, length, stdin);
  while (*string) {
    if (*string == '\n') {
      *string = '\0';
    }
    else {
      string ++;
    }
  }
}

int main() {

  int to_server;

  to_server = client_connect();
  char line[1000];
  while (1) {
    get_input(line, sizeof line);
    write(to_server, line, sizeof line);
    read(to_server, line, sizeof line);
    printf("[%s]\n", line);
  }
}
