#include "sokobirds.h"

int main(int argc, char const *argv[])
{
    SDL_Window *gWindow = NULL;
    SDL_GLContext gContext;
    GLuint textureContent[NUM_TEXTURES];
    Mix_Music *music = NULL;
    SDL_Event e;
    Scene scene = {
        -DISPLAY_WIDTH/100, DISPLAY_HEIGHT/100,
        -30.0f, 45.0f,
        0, 0, 0, 0, 0
    };
    Camera camera = {45.0f, 0.1f, 100.0f};
    Light sun = {
        {1.0f, 1.0f, 1.0f, 1.0f}, //diffuse
        {0.5f, 0.5f, 0.5f, 1.0f}, //ambient
        {0.0f, 0.0f, 2.0f, 1.0f}, //position
    };
    Game game = {1, 0, 0, 0, 1, CFLOOR, 0, 0, 0, 0, NULL};
    Viewport viewport = {DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0, 0.5f, 1};
    unsigned char quit = 0;

    initSDL(&gWindow, &gContext, viewport);
    initGL(viewport, scene, camera, sun);
    game = initGame(game);

    loadGLTexture(textureContent, TEXTURE_MAINMENU, "assets/Textures/intro.jpg");
    loadGLTexture(textureContent, TEXTURE_SKYBOX_TOP, "assets/Textures/top2.jpg");
    loadGLTexture(textureContent, TEXTURE_SKYBOX_BOTTOM, "assets/Textures/bottom2.jpg");
    loadGLTexture(textureContent, TEXTURE_SKYBOX_SIDE, "assets/Textures/side2.jpg");
    loadGLTexture(textureContent, TEXTURE_PLAYER_FRONT, "assets/Textures/front_face.jpg");
    loadGLTexture(textureContent, TEXTURE_PLAYER_OTHER, "assets/Textures/other_face.jpg");
    loadGLTexture(textureContent, TEXTURE_BOX, "assets/Textures/crate.jpg");
    loadGLTexture(textureContent, TEXTURE_WALL, "assets/Textures/wall.jpg");
    loadGLTexture(textureContent, TEXTURE_FLOOR, "assets/Textures/grass.jpg");
    loadGLTexture(textureContent, TEXTURE_HOLE, "assets/Textures/hole.jpg");

    playMusic(&music, "assets/Sounds/NightOwl.ogg");

    while (!quit) {
        while (SDL_PollEvent(&e) != 0)
            if (e.type == SDL_QUIT)
                quit = 1;
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        quit = 1;
                        break;
                    case SDLK_SPACE:
                        game.MenuOpened = 0;
                        break;
                    case SDLK_UP:
                        game.Map = playerMove(game.Map, &game.pos_x, &game.pos_y, &game.LastStep, &game.CheckPoint, &game.Score, 'w');
                        break;
                    case SDLK_LEFT:
                        game.Map = playerMove(game.Map, &game.pos_x, &game.pos_y, &game.LastStep, &game.CheckPoint, &game.Score, 'a');
                        break;
                    case SDLK_DOWN:
                        game.Map = playerMove(game.Map, &game.pos_x, &game.pos_y, &game.LastStep, &game.CheckPoint, &game.Score, 's');
                        break;
                    case SDLK_RIGHT:
                        game.Map = playerMove(game.Map, &game.pos_x, &game.pos_y, &game.LastStep, &game.CheckPoint, &game.Score, 'd');
                        break;
                    default:
                        break;
                }
            }
        updateGame(&game);
        renderGame(game, scene, textureContent);
        //Update screen
        SDL_GL_SwapWindow(gWindow);
    }

    closeSDL(&gWindow, &music);
    return 0;
}
