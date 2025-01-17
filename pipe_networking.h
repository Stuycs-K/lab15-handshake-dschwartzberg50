#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#ifndef NETWORKING_H
#define NETWORKING_H
#define WKP "mario"

#define BUFFER_SIZE 256

void error(char *message);

int server_setup();
int server_handshake(int *to_client);
int client_handshake(int *to_server);

void server_handshake_half(int *to_client, int from_client);

#endif
