#pragma once

#include <SDL.h>

class Enemy {
public:
    enum class State {
        Alive,
        Dying,
        Dead
    };

    Enemy(int x, int y, int w, int h);

    void Update();
    void Render(SDL_Renderer* renderer);

    void Hit();
    void Move(int x, int y);
    State GetState() const { return m_state; }
    const SDL_Rect* GetRect() const { return &m_rect; }

private:
    SDL_Rect m_rect;
    State m_state;
    int m_animationTimer;
};
