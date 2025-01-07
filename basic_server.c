#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pipe_networking.h"

int main() {
	int to_client, from_client;
  from_client = server_handshake( &to_client );
	printf("server completed handshake\n");
	
	int random_fd = open("/dev/urandom", O_RDONLY);
	unsigned int message;
	while (1) {
		if (read(random_fd, &message, sizeof(message)) == -1) { error("server: read from random_fd"); }
		
		message %= 101;
		if (write(to_client, &message, sizeof(message)) == -1) { error("server: write to client"); }
		sleep(1);
	}
	
}
