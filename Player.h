#pragma once

#include <SDL.h>
#include <vector>

// Forward declaration
struct Missile;

class Player {
public:
    Player(int x, int y, int w, int h, int screenWidth, int screenHeight);

    void HandleInput(const Uint8* keyState);
    void Update();
    void Render(SDL_Renderer* renderer);

    void Fire(std::vector<Missile>& missiles);

    const SDL_Rect* GetRect() const { return &m_rect; }

private:
    SDL_Rect m_rect;
    int m_speed;
    int m_screenWidth;
    int m_screenHeight;
};
