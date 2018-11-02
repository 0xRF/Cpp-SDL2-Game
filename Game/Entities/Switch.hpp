#pragma once

#include "../../Engine/Entity.hpp"
#include "../Components/ImageRenderer.hpp"
#include "../Components/Collider.hpp"
#include "GameManager.hpp"
#include "../../Engine/Engine.hpp"

class Collider;

class Switch : public Entity<Switch>{

public:

    SDL2pp::Rect toggleZone = {0,0,0,0};

    std::vector<BaseEntity*> entitiesToToggle{};
    std::vector<Collider*> colldiersToToggle{};


    void OnDestroy() {

    }

    void Start();

    void OnUse();

    Switch(SDL2pp::Rect toggleZone) : toggleZone(toggleZone){
        std::cout << "hg\n";

    }
};
