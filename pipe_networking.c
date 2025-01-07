#include "pipe_networking.h"

void error(char *message) {
	char error_string[256];
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
	// gets the fd for the WKP and removes it
  int from_client = server_setup();
	
	// receive the name of the private pipe, the client's pid
	int syn;
	if (read(from_client, &syn, sizeof(syn)) == -1) { error("server_handshake: read from_client"); }
  char private_pipe_name[BUFFER_SIZE];
  sprintf(private_pipe_name, "%d", syn);
	
	// generate a random integer to send to client
	int random_fd = open("/dev/urandom", O_RDONLY);
	if (random_fd == -1) { error("server_handshake: open /dev/urandom"); }
	unsigned int syn_ack, ack;
	if (read(random_fd, &syn_ack, sizeof(syn_ack)) == -1) { error("server_handshake: read random_fd"); }
	
	// send SYN_ACK (random int)
  *to_client = open(private_pipe_name, O_WRONLY);
	if (*to_client == -1) { error("server_handshake: open private pipe"); }
	if (write(*to_client, &syn_ack, sizeof(syn_ack)) == -1) { error("server_handshake: write to_client"); }
	
	// ensure that ACK == SYN_ACK + 1
	if (read(from_client, &ack, sizeof(ack)) == -1) { error("server_handshake: read from_client"); }
	if (ack != syn_ack + 1) {
		printf("client send back syn_ack=%d, expected ack=%d + 1", syn_ack, ack);
    exit(1);
  }

  return from_client;
}

int client_handshake(int *to_server) {
	// open the WKP
  *to_server = open(WKP, O_WRONLY);
	if (*to_server == -1) { error("client_handshake: open WKP"); }
	
	// create the private pipe, based on the client's pid
  int pid = getpid();
	char private_pipe_name[BUFFER_SIZE];
  sprintf(private_pipe_name, "%d", pid);
	if (mkfifo(private_pipe_name, 0666) == -1) { error("client_handshake: mkfifo private pipe"); }
	
	// tell the server about the name of the private pipe
	if (write(*to_server, &pid, sizeof(pid)) == -1) { error("client_handshake: write to_server"); }
  	
	// open the private pipe, waiting for server to open it in write mode, and then remove it after that
	int from_server = open(private_pipe_name, O_RDONLY);
	if (from_server == -1) { error("client_handshake: open from_server"); }
	if (remove(private_pipe_name) != 0) { error("client_handshake: remove private pipe"); }
	
	// receive a random int from the server
	unsigned int syn_ack;
	if (read(from_server, &syn_ack, sizeof(syn_ack)) == -1) { error("client_handshake: read from_server"); }
	
	// increase it, send it back for final confirmation 
	unsigned int ack = syn_ack + 1;
	if (write(*to_server, &ack, sizeof(ack)) == -1) { error("client_handshake: write to_server"); }

  return from_server;
}

// copied from server_handshake except removed server_setup
void server_handshake_half(int *to_client, int from_client) {
	// receive the name of the private pipe, the client's pid
	int syn;
	if (read(from_client, &syn, sizeof(syn)) == -1) { error("server_handshake: read from_client"); }
  char private_pipe_name[BUFFER_SIZE];
  sprintf(private_pipe_name, "%d", syn);
	
	// generate a random integer to send to client
	int random_fd = open("/dev/urandom", O_RDONLY);
	if (random_fd == -1) { error("server_handshake: open /dev/urandom"); }
	unsigned int syn_ack, ack;
	if (read(random_fd, &syn_ack, sizeof(syn_ack)) == -1) { error("server_handshake: read random_fd"); }
	
	// send SYN_ACK (random int)
  *to_client = open(private_pipe_name, O_WRONLY);
	if (*to_client == -1) { error("server_handshake: open private pipe"); }
	if (write(*to_client, &syn_ack, sizeof(syn_ack)) == -1) { error("server_handshake: write to_client"); }
	
	// ensure that ACK == SYN_ACK + 1
	if (read(from_client, &ack, sizeof(ack)) == -1) { error("server_handshake: read from_client"); }
	if (ack != syn_ack + 1) {
		printf("client send back syn_ack=%d, expected ack=%d + 1", syn_ack, ack);
    exit(1);
  }
}