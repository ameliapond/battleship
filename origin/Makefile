CC=gcc

all:client server
client:client.c
	$(CC) -o client client.c # -Wall -std=c99
server:client.c
	$(CC) -o server server.c # -Wall -std=c99
clean:
	rm -rf client server *.*~
