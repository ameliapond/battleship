CC=gcc

game:game.c game.h function.h
	$(CC) -o game game.c
clean:
	rm -rf game client serveur game.c~ game.h~
