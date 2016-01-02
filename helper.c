#include "sokobirds.h"

int initSDL(SDL_Window **gWindow, SDL_GLContext *gContext, Viewport view) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }
    // Initialize SDL Mixer
    if (MIX_INIT_OGG != (Mix_Init(MIX_INIT_OGG))) {
        printf("Mix_Init: %s\n", Mix_GetError());
        return 0;
    }
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
    //Use OpenGL 3.1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    //Create window
    *gWindow = SDL_CreateWindow(APP_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
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
    // Initialize clear color
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // White background color
    glClearDepth(1.0); // Enables Clearing Of The Depth Buffer
    if(glGetError() != GL_NO_ERROR ) {
        printf( "Unable to initialize OpenGL!\n" );
        return 0;
    }

    /* The Type Of Depth Test To Do
     * specify the value used for depth buffer comparisons
     * GL_LESS Passes if the incoming depth value is less than the stored depth value */
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    if(glGetError() != GL_NO_ERROR ) {
        printf( "Unable to initialize OpenGL!\n" );
        return 0;
    }

    // Coloração de Gouraud: interpola as cores entre os vértices (coloração por defeito)
    glShadeModel(GL_SMOOTH); // Enables Smooth Color Shading

    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Calculate The Aspect Ratio Of The Window
    perspectiveGL(camera.fovy, (GLfloat) view.width / (GLfloat) view.height, camera.zNear, camera.zFar);
    if(glGetError() != GL_NO_ERROR ) {
        printf( "Unable to initialize OpenGL!\n" );
        return 0;
    }

    // Initialize ModelView Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); // LoadIdentity ???

    // Set up light number 1.
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

    float distance = 1.2f;

    glFrustum( -fW * distance, fW * distance, -fH * distance, fH * distance, zNear, zFar );
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
    *music = Mix_LoadMUS(soundFile);
    Mix_PlayMusic(*music, SOUND_LOOP);
}

void playSound(char *soundFile) {
    Mix_Chunk *sound = NULL;
    sound = Mix_LoadWAV(soundFile);
    Mix_PlayChannel(-1, sound, SOUND_NO_LOOP);
    Mix_FreeChunk(sound);
    sound = NULL;
}

int loadGLTexture(GLuint *textureContent, int textureID, char *textureFile) {
    // You should probably use CSurface::OnLoad ... ;)
    //-- and make sure the Surface pointer is good!
    SDL_Surface* Surface = IMG_Load(textureFile);

    glGenTextures(1, &textureContent[textureID]);
    glBindTexture(GL_TEXTURE_2D, textureContent[textureID]);

    int Mode = GL_RGB;

    if(Surface->format->BytesPerPixel == 4) {
        Mode = GL_RGBA;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, Mode, Surface->w, Surface->h, 0, Mode, GL_UNSIGNED_BYTE, Surface->pixels);
    glGetError();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Free up any memory we may have used
    //if ( TextureImage[0] )
    //    SDL_FreeSurface( TextureImage[0] );
    return 1;
}

void closeSDL(SDL_Window **gWindow, Mix_Music **music) {
    SDL_DestroyWindow(*gWindow);
    *gWindow = NULL;
    Mix_FreeMusic(*music);
    *music = NULL;
    Mix_CloseAudio();
    SDL_Quit();
}


void GLAPIENTRY gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez,
      GLdouble centerx, GLdouble centery, GLdouble centerz,
      GLdouble upx, GLdouble upy, GLdouble upz) {
   GLdouble m[16];
   GLdouble x[3], y[3], z[3];
   GLdouble mag;

   /* Make rotation matrix */

   /* Z vector */
   z[0] = eyex - centerx;
   z[1] = eyey - centery;
   z[2] = eyez - centerz;
   mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
   if (mag) {           /* mpichler, 19950515 */
      z[0] /= mag;
      z[1] /= mag;
      z[2] /= mag;
   }

   /* Y vector */
   y[0] = upx;
   y[1] = upy;
   y[2] = upz;

   /* X vector = Y cross Z */
   x[0] = y[1] * z[2] - y[2] * z[1];
   x[1] = -y[0] * z[2] + y[2] * z[0];
   x[2] = y[0] * z[1] - y[1] * z[0];

   /* Recompute Y = Z cross X */
   y[0] = z[1] * x[2] - z[2] * x[1];
   y[1] = -z[0] * x[2] + z[2] * x[0];
   y[2] = z[0] * x[1] - z[1] * x[0];

   /* mpichler, 19950515 */
   /* cross product gives area of parallelogram, which is < 1.0 for
    * non-perpendicular unit-length vectors; so normalize x, y here
    */

   mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
   if (mag) {
      x[0] /= mag;
      x[1] /= mag;
      x[2] /= mag;
   }

   mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
   if (mag) {
      y[0] /= mag;
      y[1] /= mag;
      y[2] /= mag;
   }

#define M(row,col)  m[col*4+row]
   M(0, 0) = x[0];
   M(0, 1) = x[1];
   M(0, 2) = x[2];
   M(0, 3) = 0.0;
   M(1, 0) = y[0];
   M(1, 1) = y[1];
   M(1, 2) = y[2];
   M(1, 3) = 0.0;
   M(2, 0) = z[0];
   M(2, 1) = z[1];
   M(2, 2) = z[2];
   M(2, 3) = 0.0;
   M(3, 0) = 0.0;
   M(3, 1) = 0.0;
   M(3, 2) = 0.0;
   M(3, 3) = 1.0;
#undef M
   glMultMatrixd(m);

   /* Translate Eye to Origin */
   glTranslated(-eyex, -eyey, -eyez);
}
