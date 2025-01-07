#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "pipe_networking.h"

int main() {
	int to_server, from_server;
	from_server = client_handshake( &to_server );
	printf("client completed handshake\n");
	
	unsigned int message;
	while (1) {
		if (read(from_server, &message, sizeof(message)) == -1) { error("client: read from server"); }
		
		printf("message: %d\n", message);
	}
}
