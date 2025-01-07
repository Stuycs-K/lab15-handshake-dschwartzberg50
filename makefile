.PHONY: compile server client clean
server: server.out
	@./server.out
client: client.out
	@./client.out
compile server.out client.out: basic_client.o basic_server.o pipe_networking.o
	@gcc -o server.out basic_server.o pipe_networking.o
	@gcc -o client.out basic_client.o pipe_networking.o
basic_server.o: basic_server.c pipe_networking.h
	@gcc -c basic_server.c
basic_client.o: basic_client.c pipe_networking.h
	@gcc -c basic_client.c
pipe_networking.o: pipe_networking.h
	@gcc -c pipe_networking.c
clean:
	rm -f *.o mario