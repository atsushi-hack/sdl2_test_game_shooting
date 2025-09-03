#include "Game.h"

int main(int argc, char* argv[]) {
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 1024;
    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    Game* game = new Game();
    game->Init("SDL2 Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

    Uint32 frameStart;
    int frameTime;

    while (game->IsRunning()) {
        frameStart = SDL_GetTicks();

        game->HandleEvents();
        game->Update();
        game->Render();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    game->Clean();
    delete game;

    return 0;
}
