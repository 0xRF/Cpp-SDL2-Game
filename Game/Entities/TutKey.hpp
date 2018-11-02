#pragma once

#include "../../Engine/Entity.hpp"
#include "../Components/ImageRenderer.hpp"

class TutKey : public Entity<TutKey>{

public:

    char key = '5';

    void OnDestroy() {};

    static SDL2pp::Texture* pSpace;

    void Start();

    TutKey(char zkey);
};
