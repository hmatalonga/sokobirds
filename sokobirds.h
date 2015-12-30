#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_opengl.h>

#define APP_NAME "Sokobirds"
#define DEBUG 1
#define DATA_FILE "data.bin"
#define DISPLAY_WIDTH 800
#define DISPLAY_HEIGHT 480

#define WORLD_MAX_WIDTH 32 //Tamanho max da largura da matrix
#define WORLD_MAX_HEIGHT 32 //Tamanho max da altura da matrix

#define CWALL 'X' //Char para Parede
#define CFLOOR ' ' //Char para Chão
#define CBOX '*' //Char para Caixa
#define CPLAYER '@' //Char para Player
#define CHOLE '.' //Char para Buraco
#define CDONE 'D' //Char para Objectivo
#define CEMPTY 'N' //Char para Vazio

typedef struct Game {
    int MenuOpened; //Var controlo de menu (visivel)
    int pos_x, pos_y; //Pos_x e Pos_y actuais do Player
    int Score; //Pontuação do Jogo
    int Level; //Num do Nivel actual do jogo
    char LastStep; //Guardar o tipo da última posição onde esteve o Player
    int CheckPoint; //Guardar o tipo da última posição onde esteve a última Caixa movida
    int ValidMove; //Regista se o último movimento foi válido
    int GameOver; //Determine se Jogo chegou ao fim
    int Help;
    char **Map; //Array para Mapa do Jogo
} Game;

typedef struct Viewport {
    int width; //Largura da Viewport
    int height; //Altura da Viewport
    float pos_x, pos_y; //Pos_x e Pos_y da Viewport (Origem)
    GLfloat rotation; //Valor do ângulo de rotação da Viewport
    int fullscreen; //Var controlo de Viewport Ecran Inteiro
} Viewport;

// Functions definitions
// helper.c
int init(SDL_Window **gWindow, SDL_GLContext *gContext, Viewport view);
int initGL();
Game initGame(Game game);
char **loadMap(char **map, int *pos_x, int *pos_y, int *score, char *fname);
void listMap(char **map);
void playMusic(char *soundFile);
void playSound(char *soundFile);
void close(SDL_Window **gWindow);
// draw.c
void render();