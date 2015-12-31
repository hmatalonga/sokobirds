#include "sokobirds.h"

int initSDL(SDL_Window **gWindow, SDL_GLContext *gContext, Viewport view) {
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
    return 1;
}

int initGL(Viewport view, Scene scene, Camera camera, Light light) {
    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f );
    if(glGetError() != GL_NO_ERROR ) {
        printf( "Unable to initialize OpenGL!\n" );
        return 0;
    }
    glClearDepth(1.0); // Enables Clearing Of The Depth Buffer

    /* The Type Of Depth Test To Do
     * specify the value used for depth buffer comparisons
     * GL_LESS Passes if the incoming depth value is less than the stored depth value */
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    if(glGetError() != GL_NO_ERROR ) {
        printf( "Unable to initialize OpenGL!\n" );
        return 0;
    }

    //Coloração de Gouraud: interpola as cores entre os vértices (coloração por defeito)
    glShadeModel(GL_SMOOTH); // Enables Smooth Color Shading

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // Reset The Projection Matrix
    perspectiveGL(camera.fovy, (GLfloat) view.width / (GLfloat) view.height, camera.zNear, camera.zFar); // Calculate The Aspect Ratio Of The Window
    //gluPerspective(camera.fovy, (GLfloat) view.width / (GLfloat) view.height, camera.zNear, camera.zFar); // Calculate The Aspect Ratio Of The Window
    if(glGetError() != GL_NO_ERROR ) {
        printf( "Unable to initialize OpenGL!\n" );
        return 0;
    }

    glMatrixMode(GL_MODELVIEW);
    // set up light number 1.
    glLightfv(GL_LIGHT1, GL_AMBIENT, light.ambient); // add lighting. (ambient)
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light.diffuse); // add lighting. (diffuse).
    glLightfv(GL_LIGHT1, GL_POSITION, light.position); // set light position.
    glEnable(GL_LIGHT1); // turn light 1 on.
    if(glGetError() != GL_NO_ERROR ) {
        printf( "Unable to initialize OpenGL!\n" );
        return 0;
    }
    return 1;
}

Game initGame(Game game) {
    game.Map = loadMap(game.Map, &game.pos_x, &game.pos_y, &game.Score, "levels/level1.txt");
    return game;
}

void perspectiveGL( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar ) {
    // Replaces gluPerspective. Sets the frustum to perspective mode.
    // fovY     - Field of vision in degrees in the y direction
    // aspect   - Aspect ratio of the viewport
    // zNear    - The near clipping distance
    // zFar     - The far clipping distance
    // http://nehe.gamedev.net/article/replacement_for_gluperspective/21002/
    GLdouble fW, fH;
    fH = tan(fovY / 360 * PI) * zNear;
    fW = fH * aspect;

    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
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

void playMusic(Mix_Music **music, char *soundFile) {
    if (MIX_INIT_OGG != (Mix_Init(MIX_INIT_OGG))) {
        printf("Mix_Init: %s\n", Mix_GetError());
        return;
    }

    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
    *music = Mix_LoadMUS(soundFile);
    Mix_PlayMusic(*music, -1);
}

void closeSDL(SDL_Window **gWindow, Mix_Music **music) {
    SDL_DestroyWindow(*gWindow);
    *gWindow = NULL;
    Mix_FreeMusic(*music);
    *music = NULL;
    SDL_Quit();
}