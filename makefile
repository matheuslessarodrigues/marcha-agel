CC=gcc

CFLAGS=-g -Wall
SDL_FLAGS=-lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer -lSDL2_image
#SDL_FLAGS=-lSDL2main -lSDL2
IFLAGS=-IC:\tools\clibs\SDL\include -IC:\Workspace\C\marcha-agel\include
LFLAGS=-LC:\tools\clibs\SDL\lib -LC:\Workspace\C\marcha-agel\lib

all: jogo

jogo: jogo.c
	$(CC) -o jogo jogo.c $(CFLAGS) $(IFLAGS) $(LFLAGS) -lmingw32 $(SDL_FLAGS)

clean:
	rm -rf *.o jogo.exe main.exe

main: main.c
	$(CC) -o main main.c $(CFLAGS) $(IFLAGS) $(LFLAGS) -lmingw32 $(SDL_FLAGS)

#-I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
#-L/usr/lib -lSDL
#gcc -o teste5 funcoes.o teste5.o $(CFLAGS) $(IMAGE_FLAGS) `sdl-config --cflags --libs`