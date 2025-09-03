#include "Enemy.h"

const int DEATH_ANIMATION_TIME = 30; // in frames

Enemy::Enemy(int x, int y, int w, int h) 
    : m_state(State::Alive), m_animationTimer(0) {
    m_rect.x = x;
    m_rect.y = y;
    m_rect.w = w;
    m_rect.h = h;
}

void Enemy::Update() {
    if (m_state == State::Dying) {
        m_animationTimer--;
        if (m_animationTimer <= 0) {
            m_state = State::Dead;
        }
    }
}

void Enemy::Render(SDL_Renderer* renderer) {
    if (m_state == State::Alive) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        SDL_RenderFillRect(renderer, &m_rect);
    } else if (m_state == State::Dying) {
        // Simple flashing animation
        if (m_animationTimer % 10 < 5) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
        }
        SDL_RenderFillRect(renderer, &m_rect);
    }
    // If Dead, do nothing (don't render)
}

void Enemy::Hit() {
    if (m_state == State::Alive) {
        m_state = State::Dying;
        m_animationTimer = DEATH_ANIMATION_TIME;
    }
}

void Enemy::Move(int x, int y) {
    m_rect.x += x;
    m_rect.y += y;
}
