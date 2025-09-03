#include "Player.h"
#include "Missile.h"

const int PLAYER_SPEED = 5;

Player::Player(int x, int y, int w, int h, int screenWidth, int screenHeight) 
    : m_speed(PLAYER_SPEED), m_screenWidth(screenWidth), m_screenHeight(screenHeight) {
    m_rect.x = x;
    m_rect.y = y;
    m_rect.w = w;
    m_rect.h = h;
}

void Player::HandleInput(const Uint8* keyState) {
    if (keyState[SDL_SCANCODE_LEFT]) {
        m_rect.x -= m_speed;
    }
    if (keyState[SDL_SCANCODE_RIGHT]) {
        m_rect.x += m_speed;
    }
    if (keyState[SDL_SCANCODE_UP]) {
        m_rect.y -= m_speed;
    }
    if (keyState[SDL_SCANCODE_DOWN]) {
        m_rect.y += m_speed;
    }
}

void Player::Update() {
    // Boundary checks
    if (m_rect.x < 0) {
        m_rect.x = 0;
    }
    if (m_rect.x + m_rect.w > m_screenWidth) {
        m_rect.x = m_screenWidth - m_rect.w;
    }
    if (m_rect.y < 0) {
        m_rect.y = 0;
    }
    if (m_rect.y + m_rect.h > m_screenHeight) {
        m_rect.y = m_screenHeight - m_rect.h;
    }
}

void Player::Render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
    SDL_RenderFillRect(renderer, &m_rect);
}

void Player::Fire(std::vector<Missile>& missiles) {
    Missile newMissile;
    newMissile.rect.w = 5;
    newMissile.rect.h = 15;
    newMissile.rect.x = m_rect.x + (m_rect.w / 2) - (newMissile.rect.w / 2);
    newMissile.rect.y = m_rect.y;
    missiles.push_back(newMissile);
}
