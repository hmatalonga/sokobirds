#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_opengl.h>

#define APP_NAME "Sokobirds"
#define DEBUG 1
#define DATA_FILE "data.bin"
#define DISPLAY_WIDTH 800
#define DISPLAY_HEIGHT 480
#ifndef PI
#define PI (3.141592653589793)
#endif

#define WORLD_MAX_WIDTH 32 //Tamanho max da largura da matrix
#define WORLD_MAX_HEIGHT 32 //Tamanho max da altura da matrix

#define CWALL 'X' //Char para Parede
#define CFLOOR ' ' //Char para Chão
#define CBOX '*' //Char para Caixa
#define CPLAYER '@' //Char para Player
#define CHOLE '.' //Char para Buraco
#define CDONE 'D' //Char para Objectivo
#define CEMPTY 'N' //Char para Vazio

#define TEXTURE_PLAYER_FRONT 0 //Textura da Face do Jogador
#define TEXTURE_PLAYER_OTHER 1 //Textura do Corpo do Jogador
#define TEXTURE_BOX 2 //Textura da Caixa
#define TEXTURE_FLOOR 3 //Textura do Chão
#define TEXTURE_WALL 4 //Textura da Parede
#define TEXTURE_HOLE 5 //Textura do Buraco
#define TEXTURE_SKYBOX_TOP 6
#define TEXTURE_SKYBOX_BOTTOM 7
#define TEXTURE_SKYBOX_SIDE 8
#define TEXTURE_MAINMENU 9
#define NUM_TEXTURES 10

#define SOUND_LOOP -1 //Música em reprodução em loop
#define SOUND_NO_LOOP 0 //Música reprodução uma vez apenas

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

typedef struct Scene {
    GLfloat x; // X eixo de deslocção de posicionamento da Cena -DISPLAY_WIDTH/100
    GLfloat y; // Y eixo de deslocção de posicionamento da Cena DISPLAY_HEIGHT/100
    GLfloat z; //profundidade do ecran. -35.0f
    GLfloat visionAngle; //ângulo de rotação -45º
    GLfloat xrot; //x rotation 0
    GLfloat yrot; //y rotation 0
    GLfloat zrot; //z rotation 0
    GLfloat xspeed; //x rotation speed
    GLfloat yspeed; //y rotation speed
} Scene;

typedef struct Camera { // 45. 0.1, 100
    GLdouble fovy; //ângulo de visão de perspectiva
    GLdouble zNear; //Ponto de proximidade da camera
    GLdouble zFar; //Distância de visão da camera
} Camera;

typedef struct Light {
    GLfloat diffuse[4]; // {1.0f, 1.0f, 1.0f, 1.0f}
    GLfloat ambient[4]; // {0.5f, 0.5f, 0.5f, 1.0f}
    GLfloat position[4]; // {0.0f, 0.0f, 2.0f, 1.0f}
} Light;

// Functions definitions
// helper.c
int initSDL(SDL_Window **gWindow, SDL_GLContext *gContext, Viewport view);
int initGL(Viewport view, Scene scene, Camera camera, Light light);
Game initGame(Game game);
void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar);
char **loadMap(char **map, int *pos_x, int *pos_y, int *score, char *fname);
void listMap(char **map);
int loadGLTexture(GLuint *textureContent, int textureID, char *textureFile);
void playMusic(Mix_Music **music, char *soundFile);
void playSound(char *soundFile);
void closeSDL(SDL_Window **gWindow, Mix_Music **music);
// draw.c
void renderGame(Game game, Scene scene, GLuint *textureContent);
void drawMainMenu(GLuint *textureContent);
void drawSkyBox(GLuint *textureContent);
void drawScene(Game game, Scene scene, GLuint *textureContent);
void drawObject(GLuint *textureContent, char *objName);
// logic.c
char **playerMove(char **Map, int *pos_x, int *pos_y, char *lastStep, int *CheckPoint, int *score, char dir);
void updateGame(Game *game);

// glulookat deprecated!!!
void GLAPIENTRY gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez,
      GLdouble centerx, GLdouble centery, GLdouble centerz,
      GLdouble upx, GLdouble upy, GLdouble upz);
// -----------------------
