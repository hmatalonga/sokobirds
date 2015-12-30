#include "sokobirds.h"

int init(SDL_Window **gWindow, SDL_GLContext *gContext, Viewport view) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    //Use OpenGL 3.1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    //Create window
    *gWindow = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        view.width, view.height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if(*gWindow == NULL) {
        printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
        return 0;
    }
    //Create context
    *gContext = SDL_GL_CreateContext(*gWindow);
    if(*gContext == NULL ) {
        printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
        return 0;
    }
    //Use Vsync
    if( SDL_GL_SetSwapInterval( 1 ) < 0 )
        printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    //Initialize OpenGL
    if(!initGL()) {
        printf( "Unable to initialize OpenGL!\n" );
        return 0;
    }
    return 1;
}

int initGL() {
    GLenum error = GL_NO_ERROR;
    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
        return 0;
    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
        return 0;
    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );
    //Check for error
    error = glGetError();
    if( error != GL_NO_ERROR )
        return 0;
    return 1;
}

Game initGame(Game game) {
    game.Map = loadMap(game.Map, &game.pos_x, &game.pos_y, &game.Score, "levels/level1.txt");
    return game;
}

char **loadMap(char **map, int *pos_x, int *pos_y, int *score, char *fname) {
    FILE *fp = fopen(fname, "r");
    char ch;
    unsigned char i = 0, j = 0;
    if (fp != NULL) {
        map = (char **) malloc(WORLD_MAX_WIDTH * sizeof (char *));
        if (map == NULL) {
            fprintf(stderr, "out of memory\n");
            return NULL;
        }
        for (i = 0; i < WORLD_MAX_WIDTH; i++) {
            map[i] = (char *) malloc(WORLD_MAX_HEIGHT * sizeof (char));
            if (map[i] == NULL) {
                fprintf(stderr, "out of memory\n");
                return NULL;
            } else
                for (j = 0; j < WORLD_MAX_HEIGHT; j++)
                    map[i][j] = 'N';
        }
        i = 0;
        j = 0;
        while ((ch = getc(fp)) != EOF) {
            if (ch == CWALL || ch == CFLOOR || ch == CBOX ||
                    ch == CPLAYER || ch == CHOLE) {
                map[i][j] = ch;
                if (ch == CPLAYER) {
                    *pos_x = i;
                    *pos_y = j;
                } else if (ch == CHOLE)
                    (*score)++;
                j++;
            } else if (ch == 10 || ch == 13) {
                i++;
                j = 0;
            }
        }
        fclose(fp);
        return map;
    }
    return NULL;
}

void listMap(char **map) {
    int i = 0, j = 0;
    for (i = 0; i < WORLD_MAX_WIDTH; i++) {
        for (j = 0; j < WORLD_MAX_HEIGHT; j++)
            printf("%c", map[i][j]);
        printf("\n");
    }
}

void playMusic(char *soundFile) {
    if (MIX_INIT_OGG != (Mix_Init(MIX_INIT_OGG))) {
        printf("Mix_Init: %s\n", Mix_GetError());
        exit(1);
    }

    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
    Mix_Music *music = Mix_LoadMUS(soundFile);
    Mix_PlayMusic(music, -1);

    while (!SDL_QuitRequested())
        SDL_Delay(250);

    Mix_FreeMusic(music);
}

void close(SDL_Window **gWindow) {
    SDL_DestroyWindow(*gWindow);
    *gWindow = NULL;
    SDL_Quit();
}