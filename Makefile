CC=gcc

all: game client server
game:game.c function.h
	$(CC) -o game game.c # -Wall -std=c99
client:client.c
	$(CC) -o client client.c # -Wall -std=c99
server:server.c
	$(CC) -o server server.c -lpthread -D_REENTRANT -DLinux # -Wall -std=c99
#game.h player.h: function.h:
#	@touch game.h player.h
clean:
	rm -rf game client server *.*~
