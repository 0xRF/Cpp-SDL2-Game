#include "GameManager.hpp"

#include "../../Engine/UI.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Engine/Button.hpp"
#include <SDL2pp/Rect.hh>
#include "Player.hpp"
#include "../Components/MainMenu.hpp"

GameManager* inst = nullptr;
MainMenu* pMenu = nullptr;

void GameManager::OnDestroy() {
    Engine::Stop();
}

void GameManager::Start() {
    inst = this;
   pMenu =  AddComponent<MainMenu>(MainMenu());
}

GameManager *GameManager::Instance() {
    return inst;
}

void GameManager::GameEnd(bool bWin) {
    pMenu->OnGameEnd(bWin);
}
