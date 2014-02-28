CC=gcc

all: game client server
game:game.c game.h function.h
	$(CC) -o game game.c # -Wall -std=c99
client:client.c
	$(CC) -o client client.c # -Wall -std=c99
server:client.c
	$(CC) -o server server.c # -Wall -std=c99
clean:
	rm -rf game client server *.*~
