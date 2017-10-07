CC=gcc

CFLAGS=-g -Wall -D_GNU_SOURCE=1 -D_REENTRANT
IMAGE_FLAGS=-lSDL_image
AUDIO_FLAGS=-lSDL_mixer
FONT_FLAGS=-lSDL_ttf
LFLAGS=-L/usr/lib
IFLAGS=-I/usr/include/SDL



all: invader
#	mv -f invader ../invader


invader: invader.o 
	g++ -o invader invader.o $(CFLAGS) $(IMAGE_FLAGS) $(LFLAGS) $(IFLAGS) $(AUDIO_FLAGS) $(FONT_FLAGS)
	
#funcoes.o: funcoes.c
#	$(CC) funcoes.c -c  $(CFLAGS) $(LFLAGS) $(IFLAGS)

invader.o: invader.c
	$(CC) invader.c -c $(CFLAGS) $(LFLAGS) $(IFLAGS)

clean:
	rm -rf *.o invader core



#-I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT
#-L/usr/lib -lSDL
#gcc -o teste5 funcoes.o teste5.o $(CFLAGS) $(IMAGE_FLAGS) `sdl-config --cflags --libs`
