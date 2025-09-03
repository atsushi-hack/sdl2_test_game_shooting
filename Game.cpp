#include "Game.h"
#include <iostream>
#include <algorithm> // For std::remove_if
#include <cstdlib>   // For rand() and srand()
#include <ctime>     // For time()

// Frame rate control
const int FPS = 60;
const int frameDelay = 1000 / FPS;

// Enemy settings
const int ENEMY_MOVE_INTERVAL = 1000; // ms

// Star settings
const int NUM_STARS = 200;

// Font settings
const char* FONT_PATH = "/usr/local/share/fonts/mplus-ipa/fonts/ipagp.ttf";

Game::Game() : m_isRunning(false), m_window(nullptr), m_renderer(nullptr), m_player(nullptr), m_screenWidth(0), m_screenHeight(0), m_enemyMoveTimer(0), m_gameState(GameState::PLAYING), m_score(0), m_font(nullptr) {}

Game::~Game() {
    Clean();
}

void Game::Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
    m_screenWidth = width;
    m_screenHeight = height;

    int flags = 0;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        srand(time(0)); // Seed random number generator

        std::cout << "SDL initialized..." << std::endl;
        m_window = SDL_CreateWindow(title, xpos, ypos, width, height, flags | SDL_WINDOW_SHOWN);
        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

        if(TTF_Init() == -1) {
            std::cerr << "Could not initialize SDL_ttf: " << TTF_GetError() << std::endl;
            m_isRunning = false;
            return;
        }
        m_font = TTF_OpenFont(FONT_PATH, 28);
        if (!m_font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            m_isRunning = false;
            return;
        }

        m_isRunning = true;
        m_gameState = GameState::PLAYING;
        m_score = 0;

        // Create player, enemies, stars...
        m_player = new Player((width / 2) - 25, height - 60, 50, 50, width, height);
        for (int i = 0; i < 5; ++i) { // 5 columns
            for (int j = 0; j < 3; ++j) { // 3 rows
                m_enemies.emplace_back(50 + i * 100, 50 + j * 60, 40, 40);
            }
        }
        for (int i = 0; i < NUM_STARS; ++i) {
            m_stars.push_back({(float)(rand() % m_screenWidth), (float)(rand() % m_screenHeight), (float)(rand() % 5 + 1) / 2.0f, rand() % 3 + 1});
        }
        m_enemyMoveTimer = SDL_GetTicks();

    } else {
        m_isRunning = false;
        std::cerr << "Could not initialize SDL: " << SDL_GetError() << std::endl;
    }
}

void Game::HandleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_isRunning = false;
        }
        if (m_gameState == GameState::PLAYING && event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE && event.key.repeat == 0) {
                m_player->Fire(m_missiles);
            }
        }
    }

    if (m_gameState == GameState::PLAYING) {
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        m_player->HandleInput(currentKeyStates);
    }
}

void Game::Update() {
    if (m_gameState != GameState::PLAYING) return;

    // Update stars, player, enemies, missiles...
    for (auto& star : m_stars) {
        star.y += star.speed;
        if (star.y > m_screenHeight) { star.y = 0; star.x = rand() % m_screenWidth; }
    }
    m_player->Update();
    if (SDL_GetTicks() - m_enemyMoveTimer > ENEMY_MOVE_INTERVAL) {
        for (auto& enemy : m_enemies) { enemy.Move(0, 20); }
        m_enemyMoveTimer = SDL_GetTicks();
    }

    // Missile-enemy collision
    for (auto it_m = m_missiles.begin(); it_m != m_missiles.end(); ) {
        it_m->rect.y -= 15;
        bool missile_removed = false;
        for (auto& enemy : m_enemies) {
            if (enemy.GetState() == Enemy::State::Alive && SDL_HasIntersection(&it_m->rect, enemy.GetRect())) {
                enemy.Hit();
                m_score += 100;
                it_m = m_missiles.erase(it_m);
                missile_removed = true;
                break;
            }
        }
        if (missile_removed) continue;
        if (it_m->rect.y < -it_m->rect.h) { it_m = m_missiles.erase(it_m); } else { ++it_m; }
    }

    for (auto& enemy : m_enemies) { enemy.Update(); }
    m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(), [](const Enemy& e) { return e.GetState() == Enemy::State::Dead; }), m_enemies.end());

    // Game state checks
    if (m_enemies.empty()) {
        m_gameState = GameState::CLEAR;
        return;
    }
    for (const auto& enemy : m_enemies) {
        if (enemy.GetState() == Enemy::State::Alive && (enemy.GetRect()->y + enemy.GetRect()->h >= m_screenHeight)) {
            m_gameState = GameState::GAME_OVER;
            return;
        }
    }
    const SDL_Rect* playerRect = m_player->GetRect();
    for (const auto& enemy : m_enemies) {
        if (enemy.GetState() == Enemy::State::Alive && SDL_HasIntersection(playerRect, enemy.GetRect())) {
            m_gameState = GameState::GAME_OVER;
            return;
        }
    }
}

void Game::RenderText(const std::string& message, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(m_font, message.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(m_renderer, texture, NULL, &dstRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Game::Render() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    for (const auto& star : m_stars) {
        if (star.size > 2) SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        else if (star.size > 1) SDL_SetRenderDrawColor(m_renderer, 200, 200, 200, 255);
        else SDL_SetRenderDrawColor(m_renderer, 150, 150, 150, 255);
        SDL_Rect starRect = { (int)star.x, (int)star.y, star.size, star.size };
        SDL_RenderFillRect(m_renderer, &starRect);
    }

    m_player->Render(m_renderer);
    for (const auto& missile : m_missiles) {
        SDL_SetRenderDrawColor(m_renderer, 255, 255, 0, 255);
        SDL_RenderFillRect(m_renderer, &missile.rect);
    }
    for (auto& enemy : m_enemies) { enemy.Render(m_renderer); }

    // Render score and messages
    std::string score_text = "Score: " + std::to_string(m_score);
    RenderText(score_text, 10, 10, {255, 255, 255, 255});

    if (m_gameState == GameState::CLEAR) {
        RenderText("Game Clear!", m_screenWidth / 2 - 100, m_screenHeight / 2 - 50, {0, 255, 0, 255});
    } else if (m_gameState == GameState::GAME_OVER) {
        RenderText("Game Over", m_screenWidth / 2 - 100, m_screenHeight / 2 - 50, {255, 0, 0, 255});
    }

    SDL_RenderPresent(m_renderer);
}

void Game::Clean() {
    TTF_CloseFont(m_font);
    TTF_Quit();
    delete m_player;
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
    std::cout << "Game cleaned..." << std::endl;
}
