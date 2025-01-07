#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "pipe_networking.h"

static void handle_sigpipe(int signal) {
	;
}

static void handle_sigint(int signal) {
	if (remove(WKP) != 0) { error("server: remove WKP"); }
	exit(0);
}

int main() {
	signal(SIGPIPE, handle_sigpipe);
	signal(SIGINT, handle_sigint);
	
	int to_client, from_client;
	int random_fd = open("/dev/urandom", O_RDONLY);
	unsigned int x;
	char message[BUFFER_SIZE];
	
	while (1) {
		from_client = server_setup();
		
		int p = fork();
		if (p == -1) { error("server: fork"); }
		else if (p == 0) { break; } // child
		else { continue; } // parent
	}
	
	// only the forked children can be here
	server_handshake_half(&to_client, from_client);
	while (1) {
		if (read(random_fd, &x, sizeof(x)) == -1) { error("subserver: read from random_fd"); }
		x %= 100;
		
		sprintf(message, "this is a message from subserver pid=%d: %u", getpid(), x);
		
		int bytes = write(to_client, &message, sizeof(message));
		if (bytes == -1 && errno == EPIPE) {
			exit(0);
		} else if (bytes == -1) { error("subserver: write to client"); }
		
		sleep(1);
	}
	
}
