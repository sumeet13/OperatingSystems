all: server client
server: server.c 
	gcc -c server.c -o server
	gcc server -o server.o

client: client.c
	gcc -c client.c -o client
	gcc client -o client.c
clean :
	-rm *.o $(objects) 