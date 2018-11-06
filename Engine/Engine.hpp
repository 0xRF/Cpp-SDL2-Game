#pragma once

#include <vector>
#include <map>
#include <SDL2pp/SDL2pp.hh>
#include "InputManager.hpp"

class Collider;
class BaseEntity;

class Engine {

public:
    static Engine& Instance();

    static const bool Start();
    static void Stop();
    InputManager* inputManager = nullptr;

    static SDL2pp::Texture* LoadTexture(const char* szFileName);

    SDL2pp::Rect windowRect = {0,0, 1280, 720};

    static BaseEntity* FindObjectOfType(const std::size_t& id);

    void Update();

    Engine(Engine const&) = delete;
    Engine&operator=(Engine const&) = delete;

    SDL2pp::Window* g_pWindow  = nullptr;
    SDL2pp::Renderer* g_pRenderer = nullptr;
    SDL2pp::Mixer* g_pMixer = nullptr;
    std::vector<BaseEntity*> entityList = std::vector<BaseEntity*>();

    static void PlaySound(const SDL2pp::Chunk* pSound);
    static void PlaySound(const SDL2pp::Music* pSound, bool bLoop = false);
    static void AddEntity(BaseEntity* pEnt);

private:

    bool bShutdown = 0;

    Engine(){}
    ~Engine(){ std::cout << "Deleting Engine\n";};

    void Render();
    static int RenderThread(void* data);
};

