/*
 * Autores : Matheus Lessa Rodrigues
 *		     Emanuelle Schiavon
 *           Rafael Orosco
 *
 * Jogo : Marcha do Agel
 * Trabalho de comp!
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

//Definições Gerais :
#define ERRO -17
#define TICKS_TIME 15
#define ANIME_POR_VOLTAS 4
#define PONTOS_POR_COMIDA 5

#define JANELA_TITULO "Marcha do Agel"
#define JANELA_ICONE "Marcha do Agel"

//Definições de Grade :
#define GRADE 32
#define VEL 8

//Definições de Imagem :
#define INTRO_SPRITE "imagem/intro.jpg"
#define TITULO_SPRITE "imagem/titulo.jpg"
#define FUNDO_SPRITE "imagem/fundo.jpg"
#define CREDITOS_SPRITE "imagem/creditos.jpg"

#define PINGUIM1_SPRITE_NOME "imagem/pinguim1.png"
#define PINGUIM2_SPRITE_NOME "imagem/pinguim2.png"
#define PINGUIM_SPRITE_W 32
#define PINGUIM_SPRITE_H 48
#define PINGUIM_ANIME_MAX 4

#define COMIDA_SPRITE_NOME "imagem/calca.png"
#define COMIDA_SPRITE_W 32
#define COMIDA_SPRITE_H 32
#define COMIDA_ANIME_MAX 4

#define BROTA1_SPRITE_NOME "imagem/brota1.png"
#define BROTA2_SPRITE_NOME "imagem/brota2.png"
#define BROTA_ANIME_MAX 4

#define OBSTACULO_SPRITE_NOME "imagem/obstaculos.png"
#define OBSTACULO_SPRITE_W 32
#define OBSTACULO_SPRITE_H 32
#define OBSTACULO_ANIME_MAX 4

//Definições de Som :
#define MUSICA_MENU "som/menu.mp3"
#define MUSICA_CREDITOS "som/baboon.mp3"
#define MUSICA_JOGO "som/tele.mp3"

#define CANAL_EFEITOS 0

//Definições de Fonte :
#define FONTE_NOME "fonte/Action.ttf"
#define FONTE_TAM 32

//Definições de Tela :
#define PLACAR_H (GRADE * 2)
#define PLACAR_TXT_COR 0xFFFFFF
#define PLACAR_TXT_MAX 51

#define SCREEN_W (GRADE * 25)
#define SCREEN_H (GRADE * 18 + PLACAR_H)

//Definições de Telas de Jogo :
#define TELA_INTRO 1
#define TELA_TITULO 2
#define TELA_OPCOES 3
#define TELA_RECORDES 4
#define TELA_GAMEOVER 5
#define TELA_ARCADE 6
#define TELA_BATALHA 7
#define TELA_CREDITOS 8
#define TELA_TUTORIAL 9

//Definições de Texto de Menu de Opções :
#define MENU_COR_TXT 0xFFFFFF
#define MENU_COR_VAR 0x001010
#define MENU_COR_MAX 10
#define MENU_SUP_MAX 7

#define TITULO_MAX 4 //5 linha 1175
#define TITULO_TXT_MAX 21
#define TITULO_X (3 * GRADE)
#define TITULO_Y (13 * GRADE)
#define TITULO_ARCADE "Arcade"
#define TITULO_BATALHA "Multiplayer"
#define TITULO_OPCOES "Opcoes"
#define TITULO_RECORDES "Recordes"
#define TITULO_SAIR "Sair"

#define OPCOES_MAX 7
#define OPCOES_TXT_MAX 51
#define OPCOES_X (2 * GRADE)
#define OPCOES_Y (2 * GRADE)
#define OPCOES_SOM "Permitir som"
#define OPCOES_TECLAS1 "Configurar teclas do jogador 1"
#define OPCOES_TECLAS2 "Configurar teclas do jogador 2"
#define OPCOES_FASE "Escolher a fase para o modo multiplayer"
#define OPCOES_TUTORIAL "Tutorial de como jogar esse jogo"
#define OPCOES_CREDITOS "Saiba quem foram os loucos que fizeram esse jogo!"
#define OPCOES_SAIR "Voltar para a tela titulo.."

#define FASE1_SPRITE "imagem/tela1mini.jpg"
#define FASE2_SPRITE "imagem/tela2mini.jpg"
#define FASE3_SPRITE "imagem/tela3mini.jpg"
#define FASE4_SPRITE "imagem/tela4mini.jpg"
#define FASE5_SPRITE "imagem/tela5mini.jpg"

#define TUTORIAL_SPRITE "imagem/tutorial.jpg"

//Definições de Jogador :
#define MULTIPLAYER_MAX 2

#define MAX 30
#define N_INI 1
#define VIDAS_INI 3
#define NIVEL_MAX 5

#define POS1_X 2
#define POS1_Y 4
#define POS2_X 22
#define POS2_Y 17

//Macros :
#define NA_GRADE(obj) ((((obj).x % GRADE == 0) && ((obj).y % GRADE == 0)) ? 1 : 0)

#define DIR_INVERSA(dir) (((dir) == 2) ? 8 : (((dir) == 4) ? 6 : (((dir) == 6) ? 4 : (((dir) == 8) ? 2 : 0))))

#define ZERAR_TICKS          \
	_ticks = SDL_GetTicks(); \
	_ticks_ini = _ticks

#define unless(x) if (!(x))

//ESTRUTURAS======================================================================================================================

typedef struct _PASSAVEL
{
	int baixo;
	int esquerda;
	int direita;
	int cima;
} PASSAVEL;

typedef struct _COMIDA
{
	SDL_Rect rect;
	Uint32 cor;
	SDL_Texture *sprite;
	int sprite_anime;
} COMIDA;

typedef struct _OBJETO
{
	signed int x;
	signed int y;
	int dir;
	int lider;
	int sprite_anime;
} OBJETO;

typedef struct _CONJUNTO
{
	OBJETO *obj;
	int start;
	SDL_Texture *sprite;
} CONJUNTO;

typedef struct _OPCAO
{
	unsigned int cor;
	int fade;
} OPCAO;

typedef struct _MENU
{
	OPCAO opc[MENU_SUP_MAX];
	SDL_Rect rect;
	int cursor;
} MENU;

//VARIÁVEIS GLOBAIS===============================================================================================================

int fullscreen = 0;
SDL_Window *_window;
SDL_Renderer *_renderer;
int _input_dir[MULTIPLAYER_MAX];
SDL_Keycode _teclas1[] = {SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_UP};
SDL_Keycode _teclas2[] = {SDLK_s, SDLK_a, SDLK_d, SDLK_w};
int _vidas[MULTIPLAYER_MAX];
int _pontos[MULTIPLAYER_MAX];
int _nivel = 1;
int _pause;
int _som = 1;
Mix_Music *_musica_fundo;
Mix_Chunk *_efeito_som;
unsigned int _ticks, _ticks_ini;
TTF_Font *_fonte;

//FUNÇÕES=========================================================================================================================

unsigned int COR(unsigned int r, unsigned int g, unsigned int b)
{
	unsigned int cor = 0;
	cor += (r * 0x10000) & 0xFF0000;
	cor += (g * 0x100) & 0x00FF00;
	cor += b & 0x0000FF;

	return cor;
}

SDL_Texture *LoadTextureWithColorKey(const char *fileName, unsigned int r, unsigned int g, unsigned int b)
{
	SDL_Surface *surface = IMG_Load(fileName);
	if (surface == NULL)
	{
		printf("Nao foi possivel carregar a imagem %s\n", fileName);
		exit(ERRO);
	}

	if (SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, r, g, b)) < 0)
	{
		printf("Nao foi possivel escolher a color key para a imagem %s\n", fileName);
		exit(ERRO);
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);
	if (texture == NULL)
	{
		printf("Nao foi possivel converter a imagem %s em textura\n", fileName);
		exit(ERRO);
	}

	SDL_FreeSurface(surface);

	return texture;
}

void FillRect(const SDL_Rect *rect, unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
	SDL_SetRenderDrawColor(_renderer, r, g, b, a);
	if (SDL_RenderFillRect(_renderer, rect) < 0)
	{
		printf("Nao consegui preencher retangulo. %s", SDL_GetError());
		exit(ERRO);
	}
}

//Inicialização---------------------------------------------------------------------------------------

void inicializar_obj(OBJETO *obj, int x, int y)
{
	obj->x = x;
	obj->y = y;
	obj->dir = 0;
	obj->lider = 0;
	obj->sprite_anime = rand() % PINGUIM_ANIME_MAX;
}

void inicializar_com(COMIDA *com, int x, int y)
{
	com->rect.x = x;
	com->rect.y = y;
	com->rect.w = GRADE;
	com->rect.h = GRADE;
	com->cor = COR(200, 200, 0);

	com->sprite = LoadTextureWithColorKey(COMIDA_SPRITE_NOME, 0, 255, 0);
	com->sprite_anime = 0;
}

void inicializar_conj(CONJUNTO *conj, int jogadores)
{
	int i;

	for (jogadores--; jogadores >= 0; jogadores--)
	{
		conj[jogadores].obj = (OBJETO *)malloc(MAX * sizeof(OBJETO));
		if (!conj[jogadores].obj)
		{
			printf("Nao consegui alocar memoria para o vetor de objetos.\n");
			exit(ERRO);
		}

		conj[jogadores].start = MAX - N_INI;

		if (jogadores == 0)
		{
			for (i = conj[0].start; i < MAX; i++)
			{
				inicializar_obj(&conj[0].obj[i], (POS1_X + i - conj[0].start) * GRADE, POS1_Y * GRADE);
			}
		}
		else if (jogadores == 1)
		{
			for (i = conj[1].start; i < MAX; i++)
			{
				inicializar_obj(&conj[1].obj[i], (POS2_X + i - conj[1].start) * GRADE, POS2_Y * GRADE);
			}
		}

		conj[jogadores].obj[conj[jogadores].start].lider = 1;
		conj[jogadores].obj[conj[jogadores].start].sprite_anime = 0;

		if (jogadores == 0)
			conj[jogadores].sprite = LoadTextureWithColorKey(PINGUIM1_SPRITE_NOME, 0, 255, 0);
		else if (jogadores == 1)
			conj[jogadores].sprite = LoadTextureWithColorKey(PINGUIM2_SPRITE_NOME, 0, 255, 0);
	}
}

void inicializar_obst(CONJUNTO *obst, int tam)
{
	int i;

	obst->obj = (OBJETO *)malloc(tam * sizeof(OBJETO));
	if (!obst->obj)
	{
		printf("Nao consegui alocar memoria para o vetor de obstaculos.\n");
		exit(ERRO);
	}

	obst->start = tam;

	for (i = 0; i < tam; i++)
	{
		inicializar_obj(&(obst->obj[i]), 0, 0);
	}

	obst->sprite = LoadTextureWithColorKey(OBSTACULO_SPRITE_NOME, 0, 255, 0);
}

void liberar_obj(CONJUNTO *conj, int n)
{
	int i;
	for (i = 0; i < n; i++)
	{
		free(conj[i].obj);
		SDL_DestroyTexture(conj[i].sprite);
	}
}

void inicializar_fnt(char *fonte_nome, int fonte_tam)
{
	_fonte = TTF_OpenFont(fonte_nome, fonte_tam);
	if (!_fonte)
	{
		printf("Nao foi possivel inicializar a fonte: %s\n", TTF_GetError());
		exit(ERRO);
	}
}

void gerar_fase(int nivel, CONJUNTO *obst)
{
	int i, j;
	int num_obst = 0;
	char *fase[SCREEN_W / GRADE + 1];

	//Aqui são desenhadas as fases
	if (nivel == 1)
	{
		char mapa[][SCREEN_W / GRADE + 1] =
			{
				"xxxxx.xxxxxxxxxxxxx.xxxxx",
				"x.......................x",
				"x.......................x",
				"x.......................x",
				"x.......................x",
				".........................",
				"x.......................x",
				"x.......................x",
				"x.......................x",
				"x.......................x",
				"x.......................x",
				"x.......................x",
				".........................",
				"x.......................x",
				"x.......................x",
				"x.......................x",
				"x.......................x",
				"xxxxx.xxxxxxxxxxxxx.xxxxx"};
		num_obst = 74; //Número de X's
		for (i = 0; i < (SCREEN_H - PLACAR_H) / GRADE; i++)
		{
			fase[i] = mapa[i];
		}
	}
	else if (nivel == 2)
	{
		char mapa[][SCREEN_W / GRADE + 1] =
			{
				"xxxxxxxxxxxx.xxxxxxxxxxxx",
				"x.......................x",
				"x.......................x",
				"x.......................x",
				"x...........x...........x",
				"x...........x...........x",
				"x...........x...........x",
				"x.......................x",
				"........xxxxxxxxx........",
				"x.......................x",
				"x...........x...........x",
				"x...........x...........x",
				"x...........x...........x",
				"x...........x...........x",
				"x.......................x",
				"x.......................x",
				"x.......................x",
				"xxxxxxxxxxxx.xxxxxxxxxxxx"};
		num_obst = 94; //Número de X's
		for (i = 0; i < (SCREEN_H - PLACAR_H) / GRADE; i++)
		{
			fase[i] = mapa[i];
		}
	}
	else if (nivel == 3)
	{
		char mapa[][SCREEN_W / GRADE + 1] =
			{
				"xxxxxxxxxxxxxxxxxxxxxxxxx",
				"x.......................x",
				"x............xx.........x",
				"x............xx.........x",
				"x...xx..................x",
				"x...xx..................x",
				"x.......................x",
				"x...............xx......x",
				"x...............xx......x",
				"x.........xx............x",
				"x.........xx............x",
				"x.......................x",
				"x.......................x",
				"x.....xx................x",
				"x.....xx..........xx....x",
				"x.................xx....x",
				"x.......................x",
				"xxxxxxxxxxxxxxxxxxxxxxxxx"};
		num_obst = 106; //Número de X's
		for (i = 0; i < (SCREEN_H - PLACAR_H) / GRADE; i++)
		{
			fase[i] = mapa[i];
		}
	}
	else if (nivel == 4)
	{
		char mapa[][SCREEN_W / GRADE + 1] =
			{
				"xxxxxxxxxxxxxxxxxxxxxxxxx",
				"x.......................x",
				"x...........x...........x",
				"x..xx.......x........x..x",
				"x...........x........x..x",
				"x.......................x",
				"x.x.....xxxxxxx.x.....x.x",
				"x.x.............x.....x.x",
				"..x.....x.......x.....x..",
				"..x.....x.......x.....x..",
				"x.x.....x.............x.x",
				"x.x.....x.xxxxxxx.....x.x",
				"x.......................x",
				"x..x........x...........x",
				"x..x........x.......xx..x",
				"x...........x...........x",
				"x.......................x",
				"xxxxxxxxxxxxxxxxxxxxxxxxx"};
		num_obst = 126; //Número de X's
		for (i = 0; i < (SCREEN_H - PLACAR_H) / GRADE; i++)
		{
			fase[i] = mapa[i];
		}
	}
	else if (nivel == 5)
	{
		char mapa[][SCREEN_W / GRADE + 1] =
			{
				"xxxxxxxxxxxxxxxxxxxxxxxxx",
				"x.......................x",
				"x...xxxxxxxxxxxxxxxxxx..x",
				"x.......................x",
				"x..xxxxxxxxxxxxxxxxxxxx.x",
				"x.......................x",
				"x.......................x",
				"x.xxxxxxxxxxxxxxxxxxxx..x",
				"x.......................x",
				"x.......................x",
				"x..xxxxxxxxxxxxxxxxxxxx.x",
				"x.......................x",
				"x.......................x",
				"x.xxxxxxxxxxxxxxxxxxxx..x",
				"x.......................x",
				"x..xxxxxxxxxxxxxxxxxx...x",
				"x.......................x",
				"xxxxxxxxxxxxxxxxxxxxxxxxx"};
		num_obst = 198; //Número de X's
		for (i = 0; i < (SCREEN_H - PLACAR_H) / GRADE; i++)
		{
			fase[i] = mapa[i];
		}
	}
	else
	{
		//Por "default" o mapa não tem obstáculos
		char mapa[(SCREEN_H - PLACAR_H) / GRADE][SCREEN_W / GRADE + 1];
		for (i = 0; i < (SCREEN_H - PLACAR_H) / GRADE; i++)
		{
			for (j = 0; j < SCREEN_W / GRADE; j++)
			{
				mapa[i][j] = '.';
			}
		}
		mapa[SCREEN_H / GRADE - 1][SCREEN_W / GRADE] = '\0';
		for (i = 0; i < (SCREEN_H - PLACAR_H) / GRADE; i++)
		{
			fase[i] = mapa[i];
		}
	}

	inicializar_obst(obst, num_obst);
	for (i = 0; i < (SCREEN_H - PLACAR_H) / GRADE; i++)
	{
		for (j = 0; j < SCREEN_W / GRADE; j++)
		{
			if (fase[i][j] == 'x')
			{
				num_obst--;
				obst->obj[num_obst].x = j * GRADE;
				obst->obj[num_obst].y = i * GRADE + PLACAR_H;
			}
		}
	}
}

//Entrada---------------------------------------------------------------------------------------------

int scan_input(SDL_Event *event)
{
	int i;

	if (event->type == SDL_KEYDOWN)
	{
		for (i = 0; i < 4; i++)
		{
			if (event->key.keysym.sym == _teclas1[i])
			{
				unless(_pause) _input_dir[0] = (i + 1) * 2;
			}
			else if (event->key.keysym.sym == _teclas2[i])
			{
				unless(_pause) _input_dir[1] = (i + 1) * 2;
			}
		}

		switch (event->key.keysym.sym)
		{
		case SDLK_SPACE:
			if (_pause)
				_pause = 0;
			else
				_pause = 1;
			break;

		case SDLK_F11:
			fullscreen = !fullscreen;
			if (SDL_SetWindowFullscreen(_window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) < 0)
			{
				printf("Nao consegui mudar o fullscreen. %s\n", SDL_GetError());
				exit(ERRO);
			}
			break;

		case SDLK_ESCAPE:
			if (_pause)
				return 1;
			break;

		default:
			break;
		}
	}

	return 0;
}

//Colisão---------------------------------------------------------------------------------------------

void colisao(OBJETO *obj1, OBJETO *obj2, PASSAVEL *dir)
{
	if (obj1->y == obj2->y)
	{
		if (obj1->x == obj2->x + GRADE)
			dir->esquerda = 0;
		if (obj1->x + GRADE == obj2->x)
			dir->direita = 0;

		if ((obj1->x == 0 || obj1->x == SCREEN_W) && obj2->x == SCREEN_W - GRADE)
			dir->esquerda = 0;
		if ((obj1->x == SCREEN_W - GRADE || obj1->x == 0 - GRADE) && obj2->x == 0)
			dir->direita = 0;
	}
	if (obj1->x == obj2->x)
	{
		if (obj1->y == obj2->y + GRADE)
			dir->cima = 0;
		if (obj1->y + GRADE == obj2->y)
			dir->baixo = 0;

		if ((obj1->y == PLACAR_H || obj1->y == SCREEN_H) && obj2->y == SCREEN_H - GRADE)
			dir->cima = 0;
		if ((obj1->y == SCREEN_H - GRADE || obj1->y == PLACAR_H - GRADE) && obj2->y == PLACAR_H)
			dir->baixo = 0;
	}
}

PASSAVEL dir_passaveis(CONJUNTO *conj, CONJUNTO *obst, int indice, int jogadores)
{
	PASSAVEL dir = {1, 1, 1, 1};
	int i;

	//testar direção de colisão com outros objetos
	for (jogadores--; jogadores >= 0; jogadores--)
	{
		for (i = MAX - 1; i >= conj[jogadores].start; i--)
		{
			colisao(&(conj[indice].obj[conj[indice].start]), &(conj[jogadores].obj[i]), &dir);
		}
	}

	for (i = 0; i < obst->start; i++)
	{
		colisao(&(conj[indice].obj[conj[indice].start]), &(obst->obj[i]), &dir);
	}

	return dir;
}

PASSAVEL dir_comida(OBJETO *obj, COMIDA *com)
{
	PASSAVEL dir = {0, 0, 0, 0};

	if (obj->y == com->rect.y)
	{
		if (obj->x == com->rect.x + GRADE)
			dir.esquerda = 1;
		if (obj->x + GRADE == com->rect.x)
			dir.direita = 1;
	}
	if (obj->x == com->rect.x)
	{
		if (obj->y == com->rect.y + GRADE)
			dir.cima = 1;
		if (obj->y + GRADE == com->rect.y)
			dir.baixo = 1;
	}

	return dir;
}

//Comida----------------------------------------------------------------------------------------------

void criar_comida(COMIDA *com, CONJUNTO *conj, CONJUNTO *obst, int jogadores)
{
	int x, y;
	int i, continuar;
	int num_jogadores = jogadores;

	while (1)
	{
		continuar = 0;
		x = rand() % (SCREEN_W / GRADE - 4) + 2;
		y = rand() % ((SCREEN_H - PLACAR_H) / GRADE - 4) + 2;
		x *= GRADE;
		y *= GRADE;
		y += PLACAR_H;

		for (jogadores = num_jogadores - 1; jogadores >= 0 && !continuar; jogadores--)
		{
			for (i = conj[jogadores].start; i < MAX; i++)
			{
				if (x == conj[jogadores].obj[i].x && y == conj[jogadores].obj[i].y)
				{
					continuar = 1;
					break;
				}
			}
		}
		for (i = 0; i < obst->start; i++)
		{
			if (x == obst->obj[i].x && y == obst->obj[i].y)
			{
				continuar = 1;
				break;
			}
		}

		if (!continuar)
			break;
	}
	inicializar_com(com, x, y);
}

int aumentar_conj(CONJUNTO *conj, int jogadores)
{
	if (conj->start == 0)
		return 1; // PASSAR DE NÍVEL POIS O LIMITE FOI ATINGIDO

	int x = conj->obj[conj->start].x;
	int y = conj->obj[conj->start].y;

	switch (conj->obj[conj->start].dir)
	{
	case 2:
		y += GRADE;
		break;
	case 4:
		x -= GRADE;
		break;
	case 6:
		x += GRADE;
		break;
	case 8:
		y -= GRADE;
		break;
	}

	ZERAR_TICKS;
	int quit = 0;
	int anime = 0;
	SDL_Texture *brota;
	if (jogadores == 0)
		brota = LoadTextureWithColorKey(BROTA1_SPRITE_NOME, 0, 255, 0);
	else if (jogadores == 1)
		brota = LoadTextureWithColorKey(BROTA2_SPRITE_NOME, 0, 255, 0);

	SDL_Rect recorte = {0, 0, PINGUIM_SPRITE_W, PINGUIM_SPRITE_H};
	SDL_Rect destrect = {x, y - (PINGUIM_SPRITE_H - GRADE), 0, 0};

	while (!quit)
	{
		recorte.x = anime * PINGUIM_SPRITE_W;

		if (SDL_RenderCopy(_renderer, brota, &recorte, &destrect) < 0)
		{
			printf("Nao consegui desenhar a imagem do pinguim brotando.\n");
			exit(ERRO);
		}

		SDL_RenderPresent(_renderer);

		_ticks = SDL_GetTicks();
		if (_ticks - _ticks_ini > 100)
		{
			_ticks_ini = _ticks;
			anime++;
			if (anime == BROTA_ANIME_MAX)
				quit = 1;
		}
	}
	SDL_DestroyTexture(brota);

	inicializar_obj(&(conj->obj[conj->start - 1]), x, y);
	conj->obj[conj->start - 1].lider = 1;
	conj->obj[conj->start].lider = 0;
	conj->obj[conj->start - 1].dir = conj->obj[conj->start].dir;
	conj->start -= 1;

	return 0;
}

//Movimento-------------------------------------------------------------------------------------------

void mudar_pos(OBJETO *obj)
{
	unless(obj->lider)
	{
		if (NA_GRADE(*obj))
		{
			if (obj->y == (obj - 1)->y)
			{
				if (obj->x == 0 && (obj - 1)->x == SCREEN_W - GRADE)
					obj->dir = 4;
				else if (obj->x == SCREEN_W - GRADE && (obj - 1)->x == 0)
					obj->dir = 6;
				else
				{
					if (obj->x < (obj - 1)->x)
						obj->dir = 6;
					else
						obj->dir = 4;
				}
			}
			if (obj->x == (obj - 1)->x)
			{
				if (obj->y == PLACAR_H && (obj - 1)->y == SCREEN_H - GRADE)
					obj->dir = 8;
				else if (obj->y == SCREEN_H - GRADE && (obj - 1)->y == PLACAR_H)
					obj->dir = 2;
				else
				{
					if (obj->y < (obj - 1)->y)
						obj->dir = 2;
					else
						obj->dir = 8;
				}
			}
		}
	}

	switch (obj->dir)
	{
	case 2:
		obj->y += VEL;
		break;
	case 4:
		obj->x -= VEL;
		break;
	case 6:
		obj->x += VEL;
		break;
	case 8:
		obj->y -= VEL;
		break;
	}

	if (obj->x == 0 - GRADE)
		obj->x = SCREEN_W - GRADE;
	else if (obj->x == SCREEN_W)
		obj->x = 0;
	else if (obj->y == PLACAR_H - GRADE)
		obj->y = SCREEN_H - GRADE;
	else if (obj->y == SCREEN_H)
		obj->y = PLACAR_H;
}

int atualizar_pos(CONJUNTO *conj, COMIDA *com, CONJUNTO *obst, int jogadores)
{
	int num_jogadores = jogadores;
	int i, alinhados = 0;

	for (i = 0; i < num_jogadores; i++)
	{
		if (NA_GRADE(conj[i].obj[conj[i].start]))
			alinhados += 1;
	}

	PASSAVEL dir_com, dir_pas;

	for (jogadores = num_jogadores - 1; jogadores >= 0; jogadores--)
	{
		unless(_input_dir[jogadores]) continue;

		//Se todos estiverem alilhados na grade
		if (alinhados == num_jogadores)
		{
			if (MAX - conj[jogadores].start == 1 || _input_dir[jogadores] != DIR_INVERSA(conj[jogadores].obj[conj[jogadores].start].dir))
				conj[jogadores].obj[conj[jogadores].start].dir = _input_dir[jogadores];

			dir_com = dir_comida(&(conj[jogadores].obj[conj[jogadores].start]), com);
			dir_pas = dir_passaveis(conj, obst, jogadores, num_jogadores);
			switch (conj[jogadores].obj[conj[jogadores].start].dir)
			{
			case 2:
				unless(dir_pas.baixo) return jogadores + 1;
				if (dir_com.baixo)
				{
					_pontos[jogadores] += PONTOS_POR_COMIDA;
					if (aumentar_conj(&conj[jogadores], jogadores))
						return -1;
					criar_comida(com, conj, obst, num_jogadores);
					dir_pas = dir_passaveis(conj, obst, jogadores, num_jogadores);
					unless(dir_pas.baixo) return jogadores + 1;
				}
				break;
			case 4:
				unless(dir_pas.esquerda) return jogadores + 1;
				if (dir_com.esquerda)
				{
					_pontos[jogadores] += PONTOS_POR_COMIDA;
					if (aumentar_conj(&conj[jogadores], jogadores))
						return -1;
					criar_comida(com, conj, obst, num_jogadores);
					dir_pas = dir_passaveis(conj, obst, jogadores, num_jogadores);
					unless(dir_pas.esquerda) return jogadores + 1;
				}
				break;
			case 6:
				unless(dir_pas.direita) return jogadores + 1;
				if (dir_com.direita)
				{
					_pontos[jogadores] += PONTOS_POR_COMIDA;
					if (aumentar_conj(&conj[jogadores], jogadores))
						return -1;
					criar_comida(com, conj, obst, num_jogadores);
					dir_pas = dir_passaveis(conj, obst, jogadores, num_jogadores);
					unless(dir_pas.direita) return jogadores + 1;
				}
				break;
			case 8:
				unless(dir_pas.cima) return jogadores + 1;
				if (dir_com.cima)
				{
					_pontos[jogadores] += PONTOS_POR_COMIDA;
					if (aumentar_conj(&conj[jogadores], jogadores))
						return -1;
					criar_comida(com, conj, obst, num_jogadores);
					dir_pas = dir_passaveis(conj, obst, jogadores, num_jogadores);
					unless(dir_pas.cima) return jogadores + 1;
				}
				break;
			}
		}
		else
		{
			//Teste complementar de colisão * anti-bug *
			for (i = jogadores + 1; i < num_jogadores; i++)
			{
				if (conj[jogadores].obj[conj[jogadores].start].x == conj[i].obj[conj[i].start].x &&
					conj[jogadores].obj[conj[jogadores].start].y == conj[i].obj[conj[i].start].y)
					return jogadores + 1;
			}
		}
	}

	for (jogadores = num_jogadores - 1; jogadores >= 0; jogadores--)
	{
		unless(conj[jogadores].obj[conj[jogadores].start].dir) continue;

		for (i = MAX - 1; i >= conj[jogadores].start; i--)
		{
			mudar_pos(&(conj[jogadores].obj[i]));
		}
	}

	return 0;
}

//Texto-----------------------------------------------------------------------------------------------

void printt(char *string, int x, int y, unsigned int c)
{
	SDL_Color cor = {(0xFF0000 & c) / 0x10000, (0x00FF00 & c) / 0x100, 0x0000FF & c};
	SDL_Surface *surf = TTF_RenderText_Blended(_fonte, string, cor);
	if (surf == NULL)
	{
		printf("Nao consegui renderizar texto. %s\n", TTF_GetError());
		exit(ERRO);
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surf);
	if (texture == NULL)
	{
		printf("Nao consegui renderizar texto. %s\n", TTF_GetError());
		exit(ERRO);
	}
	SDL_FreeSurface(surf);

	SDL_Rect dstrct = {x, y, 0, 0};
	if (SDL_RenderCopy(_renderer, texture, NULL, &dstrct) < 0)
	{
		printf("Nao consegui fazer blit na superficie. %s\n", TTF_GetError());
		exit(ERRO);
	}
	SDL_DestroyTexture(texture);
}

//Fade------------------------------------------------------------------------------------------------

void fade(int *inout, int *alfa, int vel, int min, int max)
{
	SDL_Rect destrect = {0, 0, 0, 0};

	if (*alfa > max)
	{
		*alfa = max;
		*inout = 0;
	}
	else if (*alfa < min)
	{
		*alfa = min;
		*inout = 0;
	}

	FillRect(&destrect, 0, 0, 0, *alfa);

	if (*inout > 0)
		*alfa -= vel;
	else if (*inout < 0)
		*alfa += vel;
}

//Desenho---------------------------------------------------------------------------------------------

void desenhar_objs(CONJUNTO *conj, int anime_ligado, int jogadores)
{
	int i, num_jogadores = jogadores;
	SDL_Rect recorte = {0, 0, PINGUIM_SPRITE_W, PINGUIM_SPRITE_H};
	SDL_Rect destrect;
	int maior_z = PLACAR_H - GRADE;

	while (maior_z < SCREEN_H + GRADE)
	{
		for (jogadores = num_jogadores - 1; jogadores >= 0; jogadores--)
		{
			for (i = MAX - 1; i >= conj[jogadores].start; i--)
			{
				if (conj[jogadores].obj[i].y != maior_z)
					continue;

				recorte.x = conj[jogadores].obj[i].sprite_anime * PINGUIM_SPRITE_W;
				if (conj[jogadores].obj[i].dir)
					recorte.y = (conj[jogadores].obj[i].dir / 2 - 1) * PINGUIM_SPRITE_H;
				else
					recorte.y = 0;
				destrect.x = conj[jogadores].obj[i].x - (PINGUIM_SPRITE_W - GRADE);
				destrect.y = conj[jogadores].obj[i].y - (PINGUIM_SPRITE_H - GRADE);

				//Calcula e Desenha a outra metade do sprite no outro lado da tela..
				if (conj[jogadores].obj[i].x < 0)
				{
					SDL_Rect temprect = {SCREEN_W + destrect.x, destrect.y, PINGUIM_SPRITE_W, PINGUIM_SPRITE_H};

					if (SDL_RenderCopy(_renderer, conj[jogadores].sprite, &recorte, &temprect) < 0)
					{
						printf("Nao desenhei sprite %d \"objeto.sprite\". SDL erro: %s\n", i, SDL_GetError());
						exit(ERRO);
					}
				}
				else if (conj[jogadores].obj[i].x > SCREEN_W - GRADE)
				{
					SDL_Rect temprect = {destrect.x - SCREEN_W, destrect.y, PINGUIM_SPRITE_W, PINGUIM_SPRITE_H};

					if (SDL_RenderCopy(_renderer, conj[jogadores].sprite, &recorte, &temprect) < 0)
					{
						printf("Nao desenhei sprite %d \"objeto.sprite\". SDL erro: %s\n", i, SDL_GetError());
						exit(ERRO);
					}
				}
				else if (conj[jogadores].obj[i].y < PLACAR_H)
				{
					SDL_Rect temprect = {destrect.x, SCREEN_H - PLACAR_H + destrect.y, PINGUIM_SPRITE_W, PINGUIM_SPRITE_H};

					if (SDL_RenderCopy(_renderer, conj[jogadores].sprite, &recorte, &temprect) < 0)
					{
						printf("Nao desenhei sprite %d \"objeto.sprite\". SDL erro: %s\n", i, SDL_GetError());
						exit(ERRO);
					}
				}
				else if (conj[jogadores].obj[i].y > SCREEN_H - GRADE)
				{
					SDL_Rect temprect = {destrect.x, destrect.y - SCREEN_H + PLACAR_H, PINGUIM_SPRITE_W, PINGUIM_SPRITE_H};

					if (SDL_RenderCopy(_renderer, conj[jogadores].sprite, &recorte, &temprect) < 0)
					{
						printf("Nao desenhei sprite %d \"objeto.sprite\". SDL erro: %s\n", i, SDL_GetError());
						exit(ERRO);
					}
				}

				//Desenha o sprite na tela
				if (SDL_RenderCopy(_renderer, conj[jogadores].sprite, &recorte, &destrect) < 0)
				{
					printf("Nao desenhei sprite %d \"objeto.sprite\". SDL erro: %s\n", i, SDL_GetError());
					exit(ERRO);
				}

				if (_input_dir[jogadores] && anime_ligado)
				{
					conj[jogadores].obj[i].sprite_anime++;
					conj[jogadores].obj[i].sprite_anime %= PINGUIM_ANIME_MAX;
				}
			}
		}

		maior_z += VEL;
	}
}

void desenhar_comida(COMIDA *com, int anime_ligado)
{
	SDL_Rect recorte = {com->sprite_anime * COMIDA_SPRITE_W, 0, COMIDA_SPRITE_W, COMIDA_SPRITE_H};

	if (SDL_RenderCopy(_renderer, com->sprite, &recorte, &com->rect) < 0)
	{
		printf("Nao consegui desenhar a calca do pinguim.\n");
		exit(ERRO);
	}
	if (anime_ligado)
	{
		com->sprite_anime++;
		com->sprite_anime %= COMIDA_ANIME_MAX;
	}
}

void desenhar_obstaculos(CONJUNTO *obst)
{
	int i;
	SDL_Rect recorte = {0, 0, PINGUIM_SPRITE_W, PINGUIM_SPRITE_H};
	SDL_Rect destrect;

	for (i = 0; i < obst->start; i++)
	{
		recorte.x = obst->obj[i].sprite_anime * GRADE;
		destrect.x = obst->obj[i].x;
		destrect.y = obst->obj[i].y;

		if (SDL_RenderCopy(_renderer, obst->sprite, &recorte, &destrect) < 0)
		{
			printf("Nao desenhei sprite %d \"obstaculo.sprite\". SDL erro: %s\n", i, SDL_GetError());
			exit(ERRO);
		}
	}
}

void desenhar_placar(CONJUNTO *conj, int jogadores)
{
	int i;
	SDL_Rect recorte = {0, 0, PINGUIM_SPRITE_W, PINGUIM_SPRITE_H};
	SDL_Rect destrect;
	destrect.y = PLACAR_H - PINGUIM_SPRITE_H - 8;

	SDL_Rect placar_rect = {0, 0, SCREEN_W, PLACAR_H};

	FillRect(&placar_rect, 50, 50, 100, 255);

	char string[PLACAR_TXT_MAX];
	sprintf(string, "PONTOS : %04d", _pontos[0]);
	//Desenhar os pontos
	printt(string, placar_rect.x + 16, placar_rect.y + 16, PLACAR_TXT_COR);
	//Desenhar as vidas
	for (i = 0; i < _vidas[0]; i++)
	{
		destrect.x = placar_rect.x + (7 + i) * GRADE;

		if (SDL_RenderCopy(_renderer, conj[0].sprite, &recorte, &destrect) < 0)
		{
			printf("Nao desenhei vida %d \"objeto.sprite\". SDL erro: %s\n", i, SDL_GetError());
			exit(ERRO);
		}
	}

	if (jogadores > 1)
	{
		sprintf(string, "PONTOS : %04d", _pontos[1]);

		//Desenhar os pontos
		printt(string, placar_rect.x + SCREEN_W - 11 * GRADE + 16, placar_rect.y + 16, PLACAR_TXT_COR);
		//Desenhar as vidas
		for (i = 0; i < _vidas[1]; i++)
		{
			destrect.x = placar_rect.x + SCREEN_W + (i - 4) * GRADE;

			if (SDL_RenderCopy(_renderer, conj[1].sprite, &recorte, &destrect) < 0)
			{
				printf("Nao desenhei vida %d \"objeto.sprite\". SDL erro: %s\n", i, SDL_GetError());
				exit(ERRO);
			}
		}
	}
}

void desenhar_fundo(SDL_Texture *fundo)
{
	SDL_Rect destrect = {0, 0, 0, 0};
	if (SDL_RenderCopy(_renderer, fundo, NULL, &destrect) < 0)
	{
		printf("Nao consegui desenhar o fundo da tela. %s", SDL_GetError());
		exit(ERRO);
	}
}

void limpar_tela(unsigned int c)
{
	SDL_SetRenderDrawColor(_renderer, 0xFF0000 & c, 0x00FF00 & c, 0x0000FF & c, 255);
	if (SDL_RenderClear(_renderer) < 0)
	{
		printf("Nao consegui limpar a tela. %s", SDL_GetError());
		exit(ERRO);
	}
}

//TELA INTRO======================================================================================================================

int tela_intro(void)
{
	ZERAR_TICKS;
	int fadeio = 1;
	int alfa = 255;

	//Tocar Musica de fundo :
	_musica_fundo = Mix_LoadMUS(MUSICA_MENU);
	Mix_PlayMusic(_musica_fundo, -1);

	SDL_Texture *splash;
	SDL_Rect destrect = {0, 0, 0, 0};
	splash = IMG_LoadTexture(_renderer, INTRO_SPRITE);
	if (!splash)
	{
		printf("Nao consegui carregar a imagem de inicio \"%s\"\n", INTRO_SPRITE);
		exit(ERRO);
	}

	FillRect(&destrect, 0, 0, 0, alfa);

	while (_ticks - _ticks_ini < 5000)
	{
		_ticks = SDL_GetTicks();

		if (_ticks - _ticks_ini > 4000)
			fadeio = -1;

		if (SDL_RenderCopy(_renderer, splash, NULL, &destrect) < 0)
		{
			printf("Nao consegui desenhar a imagem de inicio \"%s\"\n", INTRO_SPRITE);
			exit(ERRO);
		}

		fade(&fadeio, &alfa, 50, 0, 255);

		SDL_RenderPresent(_renderer);
	}

	return TELA_TITULO;
}

//TELA TITULO=====================================================================================================================

int tela_titulo(void)
{
	//Inicializações----------------------------------------------------------------
	int i;

	//Coisas de Texto
	inicializar_fnt(FONTE_NOME, FONTE_TAM);
	char txt[][TITULO_TXT_MAX] = {TITULO_ARCADE, TITULO_BATALHA, TITULO_OPCOES, TITULO_SAIR};
	MENU menu;
	for (i = 0; i < TITULO_MAX; i++)
	{
		menu.opc[i].cor = MENU_COR_TXT;
		menu.opc[i].fade = 0;
	}
	menu.rect.x = TITULO_X - 8;
	menu.rect.y = TITULO_Y - 8;
	menu.rect.w = GRADE * 6 + 16;
	menu.rect.h = GRADE * (TITULO_MAX) + 16;
	menu.cursor = 0;

	//Coisas do Fundo da Tela
	SDL_Event event;
	SDL_Texture *fundo_titulo = IMG_LoadTexture(_renderer, TITULO_SPRITE);
	if (!fundo_titulo)
	{
		printf("Nao consegui iniciar superficie \"fundo\"\n");
		exit(ERRO);
	}
	//------------------------------------------------------------------------------

	//Loop principal
	while (1)
	{
		//Atualiza eventos
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				return 0;
			if (event.type == SDL_KEYDOWN)
				switch (event.key.keysym.sym)
				{
				case SDLK_SPACE:
				case SDLK_RETURN:
					switch (menu.cursor)
					{
					case 0:
						_nivel = 1;
						_vidas[0] = VIDAS_INI;
						_pontos[0] = 0;
						for (i = 1; i < MULTIPLAYER_MAX; i++)
						{
							_vidas[i] = -1;
							_pontos[i] = -1;
						}
						return TELA_ARCADE;
					case 1:
						unless(_nivel) _nivel = 1;
						for (i = 0; i < MULTIPLAYER_MAX; i++)
						{
							_vidas[i] = VIDAS_INI;
							_pontos[i] = 0;
						}
						return TELA_BATALHA;
					case 2:
						return TELA_OPCOES;
					case 3:
						return TELA_RECORDES;
					case 4:
						return 0;
					default:
						break;
					}
					break;
				case SDLK_UP:
					menu.cursor--;
					break;
				case SDLK_DOWN:
					menu.cursor++;
					break;

				case SDLK_F11:
					fullscreen = !fullscreen;
					if (SDL_SetWindowFullscreen(_window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) < 0)
					{
						printf("Nao consegui mudar o fullscreen. %s\n", SDL_GetError());
						exit(ERRO);
					}
					break;

				default:
					break;
				}
		}

		//Atualizações--------------------------------------------------------------
		if (menu.cursor == -1)
			menu.cursor = TITULO_MAX - 1;
		else if (menu.cursor == TITULO_MAX)
			menu.cursor = 0;

		for (i = 0; i < TITULO_MAX; i++)
		{
			if (menu.opc[i].cor == MENU_COR_TXT)
			{
				if (i == menu.cursor)
					menu.opc[i].fade = -1;
				else
					menu.opc[i].fade = 0;
			}
			else if (menu.opc[i].cor == MENU_COR_TXT - MENU_COR_VAR * MENU_COR_MAX)
			{
				if (i == menu.cursor)
					menu.opc[i].fade = 0;
				else
					menu.opc[i].fade = 1;
			}

			if (menu.opc[i].fade > 0)
				menu.opc[i].cor += MENU_COR_VAR;
			else if (menu.opc[i].fade < 0)
				menu.opc[i].cor -= MENU_COR_VAR;
		}

		//Limpar tela---------------------------------------------------------------
		desenhar_fundo(fundo_titulo);

		//Desenhar Texto de Opções--------------------------------------------------
		//-------
		FillRect(&menu.rect, 0, 0, 0, 255);
		//-------
		for (i = 0; i < TITULO_MAX; i++)
		{
			printt(txt[i], TITULO_X, TITULO_Y + i * GRADE, menu.opc[i].cor);
		}

		//Atualizar tela------------------------------------------------------------
		SDL_RenderPresent(_renderer);
	}

	return 0;
}

//TELA OPÇÕES=====================================================================================================================

int tela_opcoes(void)
{
	//Inicializações----------------------------------------------------------------
	ZERAR_TICKS;

	int i;
	int teclas_config = 0;
	int tecla_edit = 0;

	//Coisas de Texto
	inicializar_fnt(FONTE_NOME, FONTE_TAM);
	char txt[][OPCOES_TXT_MAX] = {OPCOES_SOM, OPCOES_TECLAS1, OPCOES_TECLAS2, OPCOES_FASE, OPCOES_TUTORIAL, OPCOES_CREDITOS, OPCOES_SAIR};
	MENU menu;
	for (i = 0; i < OPCOES_MAX; i++)
	{
		menu.opc[i].cor = MENU_COR_TXT;
		menu.opc[i].fade = 0;
	}
	menu.rect.x = OPCOES_X - 8;
	menu.rect.y = OPCOES_Y - 8;
	menu.rect.w = GRADE * 10 + 16;
	menu.rect.h = GRADE * 2 * (OPCOES_MAX) + 16;
	menu.cursor = 0;

	SDL_Event event;

	//Detalhes do menu de opções
	SDL_Texture *pinguim;
	pinguim = LoadTextureWithColorKey(PINGUIM1_SPRITE_NOME, 0, 255, 0);

	SDL_Texture *minitela;
	switch (_nivel)
	{
	case 1:
		minitela = IMG_LoadTexture(_renderer, FASE1_SPRITE);
		break;
	case 2:
		minitela = IMG_LoadTexture(_renderer, FASE2_SPRITE);
		break;
	case 3:
		minitela = IMG_LoadTexture(_renderer, FASE3_SPRITE);
		break;
	case 4:
		minitela = IMG_LoadTexture(_renderer, FASE4_SPRITE);
		break;
	case 5:
		minitela = IMG_LoadTexture(_renderer, FASE5_SPRITE);
		break;
	}
	SDL_Rect dest_mini_rect = {17 * GRADE, 14 * GRADE, 0, 0};
	SDL_Rect dest_pinguim_rect = {8 * GRADE, 2 * GRADE - (PINGUIM_SPRITE_H - GRADE), 0, 0};
	SDL_Rect recorte_pinguim = {0, 0, PINGUIM_SPRITE_W, PINGUIM_SPRITE_H};
	SDL_Rect teclas_rect = {5 * GRADE, 8 * GRADE, 11 * GRADE, 3 * GRADE + GRADE / 2};
	char nivel_string[3];
	sprintf(nivel_string, "%d", _nivel);

	//------------------------------------------------------------------------------

	//Loop principal
	while (1)
	{
		//Atualiza eventos
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				return 0;
			if (event.type == SDL_KEYDOWN)
			{
				if (teclas_config == 1)
				{
					_teclas1[tecla_edit - 1] = event.key.keysym.sym;
					tecla_edit++;
					if (tecla_edit > 4)
						teclas_config = 0;
				}
				else if (teclas_config == 2)
				{
					_teclas2[tecla_edit - 1] = event.key.keysym.sym;
					tecla_edit++;
					if (tecla_edit > 4)
						teclas_config = 0;
				}
				else
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_SPACE:
					case SDLK_RETURN:
						unless(teclas_config)
						{
							switch (menu.cursor)
							{
							case 0:
								if (_som)
								{
									_som = 0;
									Mix_PauseMusic();
								}
								else
								{
									_som = 1;
									Mix_ResumeMusic();
								}
								break;
							case 1:
								teclas_config = 1;
								tecla_edit = 1;
								break;
							case 2:
								teclas_config = 2;
								tecla_edit = 1;
								break;
							case 3:
								_nivel %= NIVEL_MAX;
								_nivel++;
								sprintf(nivel_string, "%d", _nivel);

								SDL_DestroyTexture(minitela);
								switch (_nivel)
								{
								case 1:
									minitela = IMG_LoadTexture(_renderer, FASE1_SPRITE);
									break;
								case 2:
									minitela = IMG_LoadTexture(_renderer, FASE2_SPRITE);
									break;
								case 3:
									minitela = IMG_LoadTexture(_renderer, FASE3_SPRITE);
									break;
								case 4:
									minitela = IMG_LoadTexture(_renderer, FASE4_SPRITE);
									break;
								case 5:
									minitela = IMG_LoadTexture(_renderer, FASE5_SPRITE);
									break;
								}
								break;
							case 4:
								return TELA_TUTORIAL;
							case 5:
								return TELA_CREDITOS;
							case 6:
								return TELA_TITULO;
							default:
								break;
							}
						}
						break;
					case SDLK_ESCAPE:
						return TELA_TITULO;
					case SDLK_UP:
						menu.cursor--;
						break;
					case SDLK_DOWN:
						menu.cursor++;
						break;

					case SDLK_F11:
						fullscreen = !fullscreen;
						if (SDL_SetWindowFullscreen(_window, fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) < 0)
						{
							printf("Nao consegui mudar o fullscreen. %s\n", SDL_GetError());
							exit(ERRO);
						}
						break;

					default:
						break;
					}
				}
			}
		}

		//Atualizações--------------------------------------------------------------
		if (menu.cursor == -1)
			menu.cursor = OPCOES_MAX - 1;
		else if (menu.cursor == OPCOES_MAX)
			menu.cursor = 0;

		for (i = 0; i < OPCOES_MAX; i++)
		{
			if (menu.opc[i].cor == MENU_COR_TXT)
			{
				if (i == menu.cursor)
					menu.opc[i].fade = -1;
				else
					menu.opc[i].fade = 0;
			}
			else if (menu.opc[i].cor == MENU_COR_TXT - MENU_COR_VAR * MENU_COR_MAX)
			{
				if (i == menu.cursor)
					menu.opc[i].fade = 0;
				else
					menu.opc[i].fade = 1;
			}

			if (menu.opc[i].fade > 0)
				menu.opc[i].cor += MENU_COR_VAR;
			else if (menu.opc[i].fade < 0)
				menu.opc[i].cor -= MENU_COR_VAR;
		}

		//Limpar tela---------------------------------------------------------------
		limpar_tela(0x2A4A4F);

		//Desenhar Texto de Opções--------------------------------------------------
		for (i = 0; i < OPCOES_MAX; i++)
		{
			printt(txt[i], OPCOES_X, OPCOES_Y + i * GRADE * 2, menu.opc[i].cor);
		}

		//Desenhar a escolha de som-------------------------------------------------
		if (_som)
		{
			if (SDL_RenderCopy(_renderer, pinguim, &recorte_pinguim, &dest_pinguim_rect) < 0)
			{
				printf("Nao consegui desenhar marcador de som.\n");
				exit(ERRO);
			}

			_ticks = SDL_GetTicks();
			if (_ticks - _ticks_ini > 220)
			{
				_ticks_ini = SDL_GetTicks();
				recorte_pinguim.x += PINGUIM_SPRITE_W;
				recorte_pinguim.x %= 4 * PINGUIM_SPRITE_W;
			}
		}
		else
		{
			recorte_pinguim.x = 0;
			if (SDL_RenderCopy(_renderer, pinguim, &recorte_pinguim, &dest_pinguim_rect) < 0)
			{
				printf("Nao consegui desenhar marcador de som.\n");
				exit(ERRO);
			}
		}

		//Desenhar o nivel e a fase atual a serem escolhidos------------------------
		printt(nivel_string, 21 * GRADE, 8 * GRADE, MENU_COR_TXT);
		if (SDL_RenderCopy(_renderer, minitela, NULL, &dest_mini_rect) < 0)
		{
			printf("Nao consegui desenhar a mini tela de selecao de nivel.\n");
			exit(ERRO);
		}

		//Desenhar a configuração de teclas-----------------------------------------
		if (teclas_config)
		{
			FillRect(&teclas_rect, 0, 0, 0, 255);

			printt("Aperte uma tecla para", teclas_rect.x + 8, teclas_rect.y + 8, MENU_COR_TXT);
			printt("configurar a direcao:", teclas_rect.x + 8, teclas_rect.y + 8 + GRADE, MENU_COR_TXT);

			switch (tecla_edit)
			{
			case 1:
				printt("BAIXO...", teclas_rect.x + 8, teclas_rect.y + 8 + 2 * GRADE, MENU_COR_TXT);
				break;
			case 2:
				printt("ESQUERDA...", teclas_rect.x + 8, teclas_rect.y + 8 + 2 * GRADE, MENU_COR_TXT);
				break;
			case 3:
				printt("DIREITA...", teclas_rect.x + 8, teclas_rect.y + 8 + 2 * GRADE, MENU_COR_TXT);
				break;
			case 4:
				printt("CIMA...", teclas_rect.x + 8, teclas_rect.y + 8 + 2 * GRADE, MENU_COR_TXT);
				break;
			default:
				break;
			}
		}

		//Atualizar tela------------------------------------------------------------
		SDL_RenderPresent(_renderer);
	}

	return 0;
}

//TELA GAMEOVER===================================================================================================================

int tela_gameover(void)
{
	int i;
	int quit = 0;
	int jogadores = MULTIPLAYER_MAX;

	//Carrega a musica
	if (_som)
		Mix_HaltMusic();
	Mix_FreeMusic(_musica_fundo);
	_musica_fundo = Mix_LoadMUS(MUSICA_MENU);
	Mix_PlayMusic(_musica_fundo, -1);
	Mix_PauseMusic();

	//Reseta as direções em "buffer"
	for (i = 0; i < MULTIPLAYER_MAX; i++)
	{
		_input_dir[i] = 0;
	}

	//Checar se estava jogando em multiplayer ou não
	if (_pontos[1] < 0)
		jogadores = 1;

	//Coisas da SDL
	SDL_Event event;
	char *string;
	string = (char *)malloc(31 * sizeof(char));

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				return 0;
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
				case SDLK_RETURN:
				case SDLK_SPACE:
					quit = 1;
					break;
				default:
					break;
				}
			}
		}

		//Limpa a tela
		limpar_tela(0x2A4A4F);

		//Desenha os resultados dos jogadores
		printt("Resultados do jogo:", 3 * GRADE, 2 * GRADE, MENU_COR_TXT);
		if (jogadores == 1)
		{
			sprintf(string, "Voce chegou ate a fase: %d", _nivel);
			printt(string, 3 * GRADE, 11 * GRADE, MENU_COR_TXT);
			sprintf(string, "Pontos adquiridos: %04d", _pontos[0]);
			printt(string, 3 * GRADE, 5 * GRADE, MENU_COR_TXT);
			if (_vidas[0] > 0)
				sprintf(string, "E ainda restaram %d vidas!", _vidas[0]);
			else
				sprintf(string, "GAME OVER!!");
			printt(string, 3 * GRADE, 8 * GRADE, MENU_COR_TXT);
		}
		else
		{
			sprintf(string, "Voces jogaram a fase: %d", _nivel);
			printt(string, 3 * GRADE, 11 * GRADE, MENU_COR_TXT);
			sprintf(string, "Pontos do jogador 1: %04d", _pontos[0]);
			printt(string, 3 * GRADE, 5 * GRADE, MENU_COR_TXT);
			sprintf(string, "Pontos do jogador 2: %04d", _pontos[1]);
			printt(string, 3 * GRADE, 6 * GRADE, MENU_COR_TXT);
			sprintf(string, "Vidas restantes do jogador 1: %d", _vidas[0]);
			printt(string, 3 * GRADE, 8 * GRADE, MENU_COR_TXT);
			sprintf(string, "Vidas restantes do jogador 2: %d", _vidas[1]);
			printt(string, 3 * GRADE, 9 * GRADE, MENU_COR_TXT);

			if (_pontos[0] > _pontos[1])
				sprintf(string, "O jogador 1 venceu!");
			else if (_pontos[1] > _pontos[0])
				sprintf(string, "O jogador 2 venceu!");
			else
				sprintf(string, "Empate...");
			printt(string, 3 * GRADE, 13 * GRADE, MENU_COR_TXT);
		}
		printt("Bom Jogo! Aperte ENTER para continuar..", 3 * GRADE, 17 * GRADE, MENU_COR_TXT);
		string[0] = '\0';

		SDL_RenderPresent(_renderer);
	}

	free(string);
	if (_som)
		Mix_ResumeMusic();
	return TELA_TITULO;
}

//TELA TUTORIAL===================================================================================================================

int tela_tutorial(void)
{
	int quit = 0;

	//Coisas da SDL
	SDL_Event event;
	SDL_Texture *tutorial;
	tutorial = IMG_LoadTexture(_renderer, TUTORIAL_SPRITE);
	if (!tutorial)
	{
		printf("Nao consegui carregar a imagem do tutorial");
		exit(ERRO);
	}
	SDL_Rect rect = {0, 0, 0, 0};

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				return 0;
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
				case SDLK_RETURN:
				case SDLK_SPACE:
					quit = 1;
					break;
				default:
					break;
				}
			}
		}

		if (SDL_RenderCopy(_renderer, tutorial, NULL, &rect) < 0)
		{
			printf("Nao consegui desenhar a imagem do tutorial.\n");
			exit(ERRO);
		}

		SDL_RenderPresent(_renderer);
	}

	return TELA_OPCOES;
}

//TELA CREDITOS===================================================================================================================

int tela_creditos(void)
{
	//Carregar a imagem dos creditos
	SDL_Texture *creditos;
	creditos = IMG_LoadTexture(_renderer, CREDITOS_SPRITE);
	if (!creditos)
	{
		printf("Nao consegui carregar a imagem \"%s\".\n", CREDITOS_SPRITE);
		exit(ERRO);
	}
	SDL_Event event;
	SDL_Rect destrect = {0, 0, 0, 0};
	int x = destrect.x, y = destrect.y;
	int quit = 0;

	//Carregar a musica dos creditos
	if (_som)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(_musica_fundo);
		_musica_fundo = Mix_LoadMUS(MUSICA_CREDITOS);
		Mix_PlayMusic(_musica_fundo, -1);
	}

	ZERAR_TICKS;
	limpar_tela(0x000000);

	while (!quit)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				return 0;
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
				case SDLK_RETURN:
				case SDLK_SPACE:
					quit = 1;
					break;
				default:
					break;
				}
			}
		}

		if (y > (-2) * SCREEN_H)
		{
			_ticks = SDL_GetTicks();
			if (_ticks - _ticks_ini > 50)
			{
				_ticks_ini = _ticks;
				y -= 1;
			}
		}

		destrect.x = x;
		destrect.y = y;

		if (SDL_RenderCopy(_renderer, creditos, NULL, &destrect) < 0)
		{
			printf("Nao consegui desenhar a imagem dos creditos.\n");
			exit(ERRO);
		}

		SDL_RenderPresent(_renderer);
	}

	//Carrega a musica dos menus
	if (_som)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(_musica_fundo);
		_musica_fundo = Mix_LoadMUS(MUSICA_MENU);
		Mix_PlayMusic(_musica_fundo, -1);
	}
	SDL_DestroyTexture(creditos);
	return TELA_OPCOES;
}

//TELA JOGO=======================================================================================================================

int tela_jogo(int jogadores)
{
	//Inicializações----------------------------------------------------------------
	_pause = 0;
	int fadeio;
	int alfa;
	int i;
	for (i = 0; i < MULTIPLAYER_MAX; i++)
	{
		_input_dir[i] = 0;
	}

	ZERAR_TICKS;

	//Carregar Música
	if (_som)
	{
		Mix_HaltMusic();
		Mix_FreeMusic(_musica_fundo);
		_musica_fundo = Mix_LoadMUS(MUSICA_JOGO);
		Mix_PlayMusic(_musica_fundo, -1);
	}

	SDL_Event event;
	unsigned int voltas = 0;
	int evento_pos;

	COMIDA comida;
	CONJUNTO pinguins[MULTIPLAYER_MAX];
	CONJUNTO obstaculos;

	inicializar_conj(pinguins, jogadores);
	gerar_fase(_nivel, &obstaculos);
	criar_comida(&comida, pinguins, &obstaculos, jogadores);

	//Coisas de Texto
	inicializar_fnt(FONTE_NOME, FONTE_TAM);

	//Coisas do Fundo da Tela
	SDL_Texture *fundo;
	fundo = IMG_LoadTexture(_renderer, FUNDO_SPRITE);
	if (!fundo)
	{
		printf("Nao consegui iniciar superficie \"fundo\"\n");
		exit(ERRO);
	}
	//------------------------------------------------------------------------------

	//Loop principal
	while (1)
	{
		_ticks = SDL_GetTicks();
		//Atualiza eventos
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				return 0;
			if (scan_input(&event))
			{
				if (_som)
				{
					Mix_HaltMusic();
					Mix_FreeMusic(_musica_fundo);
					_musica_fundo = Mix_LoadMUS(MUSICA_MENU);
					Mix_PlayMusic(_musica_fundo, -1);
				}
				return TELA_TITULO;
			}
		}

		//Atualizações--------------------------------------------------------------
		if (_ticks - _ticks_ini >= TICKS_TIME && !_pause)
		{
			_ticks_ini = _ticks;
			evento_pos = atualizar_pos(pinguins, &comida, &obstaculos, jogadores);
			switch (evento_pos)
			{
			case 0: // CONTINUA..
				break;
			case -1: // PRÓXIMO NÍVEL!!
				liberar_obj(pinguins, jogadores);
				liberar_obj(&obstaculos, 1);
				for (i = 0; i < MULTIPLAYER_MAX; i++)
				{
					_pontos[i] += 150 * (_vidas[i] - 1);
				}
				ZERAR_TICKS;
				fadeio = -1;
				alfa = 0;
				while (_ticks - _ticks_ini < 1000)
				{
					_ticks = SDL_GetTicks();
					fade(&fadeio, &alfa, 50, 0, 10);
					SDL_RenderPresent(_renderer);
				}
				if (jogadores > 1)
					return TELA_GAMEOVER;
				if (_nivel == NIVEL_MAX)
					return TELA_GAMEOVER;
				_nivel++;
				return TELA_ARCADE;
				break;
			default:
				liberar_obj(pinguins, jogadores);
				liberar_obj(&obstaculos, 1);
				if (_som)
				{
					Mix_HaltMusic();
					Mix_FreeMusic(_musica_fundo);
					_musica_fundo = Mix_LoadMUS(MUSICA_MENU);
					Mix_PlayMusic(_musica_fundo, -1);
				}
				_vidas[evento_pos - 1]--;
				if (_vidas[evento_pos - 1] == 0)
				{
					ZERAR_TICKS;
					fadeio = -1;
					alfa = 0;
					while (_ticks - _ticks_ini < 1000)
					{
						_ticks = SDL_GetTicks();
						fade(&fadeio, &alfa, 50, 0, 10);
						SDL_RenderPresent(_renderer);
					}
					return TELA_GAMEOVER; // GAME OVER!!
				}
				else
				{
					ZERAR_TICKS;
					fadeio = -1;
					alfa = 0;
					while (_ticks - _ticks_ini < 1000)
					{
						_ticks = SDL_GetTicks();
						fade(&fadeio, &alfa, 50, 0, 10);
						SDL_RenderPresent(_renderer);
					}
					if (jogadores == 1)
						return TELA_ARCADE; // CONTINUA COM MENOS UMA VIDA..
					else
						return TELA_BATALHA; // CONTINUA COM MENOS UMA VIDA..
				}
				break;
			}
		}

		//Limpar tela---------------------------------------------------------------
		desenhar_fundo(fundo);

		//Desenhar objetos na tela--------------------------------------------------
		desenhar_comida(&comida, voltas % ANIME_POR_VOLTAS == 0 && !_pause);
		desenhar_obstaculos(&obstaculos);
		desenhar_objs(pinguins, voltas % ANIME_POR_VOLTAS == 0 && !_pause, jogadores);

		//Desenhar placar-----------------------------------------------------------
		desenhar_placar(pinguins, jogadores);

		//Fade do Pause-------------------------------------------------------------
		if (_pause)
		{
			fadeio = -1;
			fade(&fadeio, &alfa, 50, 0, 128);
		}
		else
		{
			fadeio = 0;
			alfa = 0;
		}

		//Atualizar tela------------------------------------------------------------
		SDL_RenderPresent(_renderer);

		voltas++;
	}

	return 0;
}

//FUNÇÃO MAIN=====================================================================================================================

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

	SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_H, SDL_WINDOW_SHOWN, &_window, &_renderer);
	if (_window == NULL || _renderer == NULL)
	{
		printf("Nao pude iniciar video mode: %s\n", SDL_GetError());
		exit(ERRO);
	}

	if (TTF_Init() < 0)
	{
		printf("Nao consegui inicializar SDL_ttf: %s\n", TTF_GetError());
		exit(ERRO);
	}
	//------------------------------------------------------------------------------

	//Alterar Titulo da janela do jogo
	SDL_SetWindowTitle(_window, JANELA_TITULO);

	//Ocultar cursor
	SDL_ShowCursor(SDL_DISABLE);

	//Randomizar raiz de números aleatórios
	srand(time(NULL));

	//Gerenciador de telas
	int tela = TELA_INTRO;
	SDL_Event event;

	while (tela)
	{
		while (SDL_PollEvent(&event))
			continue; //Remove os eventos armazenados no "buffer"

		switch (tela)
		{
		case TELA_INTRO:
			tela = tela_intro();
			break;
		case TELA_TITULO:
			tela = tela_titulo();
			break;
		case TELA_OPCOES:
			tela = tela_opcoes();
			break;
		case TELA_RECORDES:
			tela = 0;
			break;
		case TELA_GAMEOVER:
			tela = tela_gameover();
			break;
		case TELA_ARCADE:
			tela = tela_jogo(1);
			break;
		case TELA_BATALHA:
			tela = tela_jogo(MULTIPLAYER_MAX);
			break;
		case TELA_CREDITOS:
			tela = tela_creditos();
			break;
		case TELA_TUTORIAL:
			tela = tela_tutorial();
			break;
		default:
			break;
		}
	}

	printf("\n\n ===OBRIGADO POR JOGAR!!=== \n\n");

	Mix_FreeMusic(_musica_fundo);
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
	return 0;
}
