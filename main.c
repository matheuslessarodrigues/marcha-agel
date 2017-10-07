#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#define ERRO -17

SDL_Window *_screen;
SDL_Renderer *_renderer;
SDL_Surface *_fade;

int main(int argc, char **argv)
{
	//Chamar SDL--------------------------------------------------------------------
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_SWSURFACE) < 0)
	{
		printf("Nao pude inicializar SDL: %s\n", SDL_GetError());
		exit(ERRO);
	}

	if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096) < 0)
	{
		printf("Nao pude inicializar SDL_mixer.\n");
		exit(ERRO);
	}

	SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_SHOWN, &_screen, &_renderer);
	if (_screen == NULL || _renderer == NULL)
	{
		printf("Nao pude iniciar video mode: %s\n", SDL_GetError());
		exit(ERRO);
	}

	_fade = SDL_CreateRGBSurface(0, 640, 480, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF);
	if (_fade == NULL)
	{
		printf("Nao pude iniciar fade superficie: %s\n", SDL_GetError());
		exit(ERRO);
	}

	if (TTF_Init() < 0)
	{
		printf("Nao consegui inicializar SDL_ttf: %s\n", TTF_GetError());
		exit(ERRO);
	}
	//------------------------------------------------------------------------------

	//Ocultar cursor
	SDL_ShowCursor(SDL_DISABLE);

	//Randomizar raiz de números aleatórios
	srand(time(NULL));

	SDL_Delay(3000);

	printf("\n\n ===OBRIGADO POR JOGAR!!=== \n\n");

	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
	return 0;
}