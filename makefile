.PHONY: compile server client clean
server: server.out
	@./server.out
client: client.out
	@./client.out
compile server.out client.out: basic_client.o persistent_server.o pipe_networking.o
	@gcc -o server.out persistent_server.o pipe_networking.o -Wall
	@gcc -o client.out basic_client.o pipe_networking.o -Wall
persistent_server.o: persistent_server.c pipe_networking.h
	@gcc -c persistent_server.c -Wall
basic_client.o: basic_client.c pipe_networking.h
	@gcc -c basic_client.c -Wall
pipe_networking.o: pipe_networking.h
	@gcc -c pipe_networking.c -Wall
clean:
	rm -f *.o mario