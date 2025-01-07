#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pipe_networking.h"

int main() {
	int to_server, from_server;
	from_server = client_handshake(&to_server);
	
	char message[BUFFER_SIZE];
	char last_message[BUFFER_SIZE]; last_message[0] = '\0'; // bad solution 
	while (1) {
		if (read(from_server, &message, sizeof(message)) == -1) { error("client: read from server"); }
		
		if (strcmp(message, last_message) == 0) { exit(0); }
		
		printf("message: %s\n", message);
		strcpy(last_message, message);
	}
}
