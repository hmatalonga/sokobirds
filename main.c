#include "sokobirds.h"

int main(int argc, char const *argv[])
{
    SDL_Window *gWindow = NULL;
    //SDL_Renderer *gRenderer = NULL;
    SDL_GLContext gContext;
    Mix_Music *music = NULL;
    SDL_Event e;
    Scene scene = {
        -DISPLAY_WIDTH/100, DISPLAY_HEIGHT/100,
        -35.0f, 45.0f,
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
    playMusic(&music, "assets/Sounds/NightOwl.ogg");

    while (!quit) {
        while (SDL_PollEvent(&e) != 0)
            if (e.type == SDL_QUIT)
                quit = 1;

        renderGame(game);
        //Update screen
        SDL_GL_SwapWindow(gWindow);
    }

    closeSDL(&gWindow, &music);
    return 0;
}
