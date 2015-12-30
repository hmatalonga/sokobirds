#include "sokobirds.h"

int main(int argc, char const *argv[])
{
    SDL_Window* gWindow;
    SDL_GLContext gContext;
    SDL_Event e;
    Game game = {1, 0, 0, 0, 1, CFLOOR, 0, 0, 0, 0, NULL};
    Viewport viewport = {DISPLAY_WIDTH, DISPLAY_HEIGHT, 0, 0, 0.5f, 1};
    unsigned char quit = 0;

    if (DEBUG)
        printf("Before init\n");
    init(&gWindow, &gContext, viewport);
    if (DEBUG)
        printf("Before initGame\n");
    game = initGame(game);
    //playMusic("assets/Sounds/NightOwl.ogg");

    while (!quit) {
        while (SDL_PollEvent(&e) != 0)
            if (e.type == SDL_QUIT)
                quit = 1;
        //Render quad
        printf("Before render\n");
        render();
        //Update screen
        SDL_GL_SwapWindow(gWindow);
    }

    close(&gWindow);
    return 0;
}
