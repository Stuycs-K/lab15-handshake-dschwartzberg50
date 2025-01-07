#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pipe_networking.h"

int main() {
	int to_client, from_client;
  from_client = server_handshake( &to_client );
	printf("server completed handshake\n");
	
	int message = 12345;
	if (write(to_client, &message, sizeof(message)) == -1) {
		error("server: write to client");
		exit(1);
	}
	
}
