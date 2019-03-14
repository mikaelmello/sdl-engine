#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#include "SDL_include.h"

#include "Game.hpp"
#include <string>
#include <iostream>

void report_sdl_error(std::string error_message, int return_code) {
    std::cout << error_message << std::endl;
    std::cout << std::endl << SDL_GetError() << std::endl;
    exit(100);
}

Game::Game(std::string title, int width, int height) {
    int return_code;
    
    return_code = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);
    if (return_code != 0) {
        report_sdl_error("Erro ao inicializar a biblioteca SDL", return_code);    
    }

    return_code = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF);
    if (return_code != 0) {
        report_sdl_error("Erro ao inicializar a biblioteca SDL_Image", return_code);
    }

    return_code = Mix_Init(MIX_INIT_FLAC | MIX_INIT_MP3 | MIX_INIT_OGG);
    if (return_code != 0) {
        report_sdl_error("Erro ao inicializar a biblioteca SDL_Mixer", return_code);
    }

    return_code = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
    if (return_code != 0) {
        report_sdl_error("Erro ao iniciar a API de Audio", return_code);
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == nullptr) {
        report_sdl_error("Erro ao criar a janela", return_code);
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        report_sdl_error("Erro ao criar o renderizador", return_code);
    }

    state = new State();
}

Game::~Game() {
    delete state;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}

Game& Game::GetInstance() {
    if (instance == nullptr) {
        instance = new Game("Trabalho 1", 800, 800);
    }
    return *instance;
}

State& Game::GetState() {
    return *state;
}

SDL_Renderer* Game::GetRenderer() {
    return renderer;
}

void Game::Run() {
    while (!state->QuitRequested) {
        state->Update();
        state->Render();
        SDL_RenderPresent(renderer);
        SDL_Delay(33);
    }
}
