#pragma once

#include <SDL2pp/Rect.hh>
#include "../../Engine/Entity.hpp"
#include "../Level.hpp"

class GameManager : public Entity<GameManager> {
public:
    void OnDestroy() override;

    void Start() override;

    Level* pCurrentLevel = nullptr;

    static GameManager* Instance();

    void GameEnd(bool bWin);

private:
    bool bEndMenuOpen = false;

};


