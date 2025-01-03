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
  if (mkfifo(WKP, 0666) == -1) { error("mkfifo WKP"); }

  int from_client = open(WKP, O_RDONLY);
  if (from_client == -1) { error("server_setup open WKP"); }

  if (remove(WKP) != 0) { error("server_setup remove WKP"); }

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
  int from_client = server_setup();

  int client_pid;
  if (read(from_client, &client_pid, sizeof(client_pid)) == -1) { error("server_handshake reading from from_client"); }

  char private_pipe_name[BUFFER_SIZE];
  sprintf(private_pipe_name, "%d", client_pid);

  int random_fd = open("/dev/urandom", O_RDONLY, 0);
  if (random_fd == -1) { error("open random_fd"); }

  unsigned int x, x2;
  if (read(random_fd, &x, sizeof(x)) == -1) { error("server_handshake read from random_fd"); }

  *to_client = open(private_pipe_name, O_WRONLY);

  // send random number to client
  if (write(*to_client, &x, sizeof(x)) == -1) { error("server write to WKP"); }

  if (read(from_client, &x2, sizeof(x2)) == -1) { error("read from from_client"); }

  printf("client returned x2=%d, expected %d\n", x2, x+1);
  if (x2 != x + 1) {
    printf("client did not return correct x\n");
    exit(1);
  } else {
    printf("client is good!\n");
  }

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
  int pid = getpid();

  char private_pipe_name[BUFFER_SIZE];
  sprintf(private_pipe_name, "%d", pid);
  if (mkfifo(private_pipe_name, 0666) == -1) { error("mkfifo private pipe"); }

  *to_server = open(WKP, O_WRONLY);
   if (WKP_read_fd == -1) { error("client_handshake open WKP read"); }

  
  int private_pipe_fd = open(private_pipe_name, O_RDONLY, 0);
  if (private_pipe_fd == -1) { error("open private_pipe_fd"); }

  if (write(private_pipe_fd, &pid, sizeof(pid)) == -1) { error("client write to private_pipe_fd"); }


  unsigned int x;
  if (read(WKP_read_fd, &x, sizeof(x)) == -1) { error("read from WKP_read_fd"); }

  x++;
  if (write(private_pipe_fd, &x, sizeof(x)) == -1) { error("write to private_pipe_fd"); }


  int from_server = WKP_read_fd;
  *to_server = private_pipe_fd;
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
