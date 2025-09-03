#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <vector>
#include <string>
#include "Player.h"
#include "Missile.h"
#include "Enemy.h"
#include "Star.h"

class Game {
public:
    enum class GameState { PLAYING, GAME_OVER, CLEAR };

    Game();
    ~Game();

    void Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
    
    void HandleEvents();
    void Update();
    void Render();
    void Clean();

    bool IsRunning() const { return m_isRunning; }

private:
    void RenderText(const std::string& message, int x, int y, SDL_Color color);

    bool m_isRunning;
    int m_screenWidth;
    int m_screenHeight;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    Player* m_player;
    std::vector<Missile> m_missiles;
    std::vector<Enemy> m_enemies;
    std::vector<Star> m_stars;

    GameState m_gameState;
    int m_score;
    TTF_Font* m_font;

    Uint32 m_frameStart;
    int m_frameTime;
    Uint32 m_enemyMoveTimer;
};
