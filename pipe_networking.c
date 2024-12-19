#include "pipe_networking.h"
//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server

char error_string[256];
void error(char *message) {
  sprintf(error_string, "%s: errno %d", message, errno);
  perror(error_string);
  exit(1);
}

/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  if (mkfifo(WKP, 0666) == -1) { error("mkfifo"); }

  int WKP_fd = open(WKP, O_RDONLY, 0);
  if (WKP_fd == -1) { error("server_setup open WKP"); }

  int x;
  if (read(WKP_fd, &x, sizeof(x)) == -1) { error("read from WKP"); }

  printf("read x from WKP: %d\n", x);

  if (remove(WKP) != 0) { error("remove WKP"); }

  int from_client = 0;
  return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  int from_client;
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  int WKP_fd = open(WKP, O_WRONLY, 0);
  if (WKP_fd == -1) { error("client_handshake open WKP"); }

  int num = 123;
  if (write(WKP_fd, &num, sizeof(num)) == -1) { error("write to WKP"); }

  int from_server;
  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = 0;
  return to_client;
}
