#include "pipe_networking.h"

char error_string[256];
void error(char *message) {
  sprintf(error_string, "%s: errno %d", message, errno);
  perror(error_string);
  exit(1);
}

int server_setup() {
	if (mkfifo(WKP, 0666) == -1) { error("server_setup: mkfifo WKP"); }
	
  int from_client = open(WKP, O_RDONLY);
	if (from_client == -1) { error("server_setup: open WKP"); }
	
	if (remove(WKP) != 0) { error("server_setup: remove WKP"); }

  return from_client;
}

int server_handshake(int *to_client) {
  int from_client = server_setup();

	int syn;
	if (read(from_client, &syn, sizeof(syn)) == -1) { error("server_handshake: read from_client"); }
	
  char private_pipe_name[BUFFER_SIZE];
  sprintf(private_pipe_name, "%d", syn);

	int random_fd = open("/dev/urandom", O_RDONLY);
	if (random_fd == -1) { error("server_handshake: open /dev/urandom"); }

	unsigned int syn_ack, ack;
	if (read(random_fd, &syn_ack, sizeof(syn_ack)) == -1) { error("server_handshake: read random_fd"); }

  *to_client = open(private_pipe_name, O_WRONLY);
	if (*to_client == -1) { error("server_handshake: open private pipe"); }
	
	if (write(*to_client, &syn_ack, sizeof(syn_ack)) == -1) { error("server_handshake: write to_client"); }
	
	if (read(from_client, &ack, sizeof(ack)) == -1) { error("server_handshake: read from_client"); }

	if (ack != syn_ack + 1) {
		printf("client send back syn_ack=%d, expected ack=%d + 1", syn_ack, ack);
    exit(1);
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
  *to_server = open(WKP, O_WRONLY);
	if (*to_server == -1) { error("client_handshake open to_server"); }
	printf("client opened to_server: %d\n", *to_server);
		
  int pid = getpid();

  char private_pipe_name[BUFFER_SIZE];
  sprintf(private_pipe_name, "%d", pid);
  if (mkfifo(private_pipe_name, 0666) == -1) { error("mkfifo private pipe"); }
	
	printf("client about to send pid to to_server\n");
	// send pid to server
	if (write(*to_server, &pid, sizeof(pid)) == -1) { error("client write to private_pipe_fd"); }
  	
	printf("client about to open from_server\n");
	int from_server = open(private_pipe_name, O_RDONLY);
	if (from_server == -1) { error("client_handshake open from_server"); }
	
	printf("client about to remove %s\n", private_pipe_name);
	if (remove(private_pipe_name) != 0) { error("client_handshake: remove private pipe"); }

  unsigned int x;
	if (read(from_server, &x, sizeof(x)) == -1) { error("client_handshake read from from_server"); }
	
  x++;
	if (write(*to_server, &x, sizeof(x)) == -1) { error("client_handshake write to to_server"); }

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
