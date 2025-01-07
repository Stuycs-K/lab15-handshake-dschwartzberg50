#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "pipe_networking.h"

static void handle_sigpipe(int signal) {
	;
}

int main() {
	signal(SIGPIPE, handle_sigpipe);
	
	int to_client, from_client;
  from_client = server_handshake(&to_client);
	
	int random_fd = open("/dev/urandom", O_RDONLY);
	unsigned int message;
	while (1) {
		if (read(random_fd, &message, sizeof(message)) == -1) { error("server: read from random_fd"); }
		
		message %= 100;
		
		int bytes = write(to_client, &message, sizeof(message));
		if (bytes == -1 && errno == EPIPE) {
			// restart the server handshake
			from_client = server_handshake(&to_client);
		} else if (bytes == -1) { error("server: write to client"); }
		
		sleep(1);
	}
	
}
