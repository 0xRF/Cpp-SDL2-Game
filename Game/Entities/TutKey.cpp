#include "TutKey.hpp"
#include "../../Engine/Engine.hpp"


static SDL2pp::Texture* aKey = nullptr;
static SDL2pp::Texture* dKey = nullptr;
static SDL2pp::Texture* spaceKey = nullptr;

void TutKey::Start() {

    aKey = Engine::LoadTexture("assets/key_a.png");
    dKey = Engine::LoadTexture("assets/key_d.png");
    spaceKey = Engine::LoadTexture("assets/key_space.png");


    if(key == 'a')
        AddComponent(ImageRenderer(aKey));
        else if(key == 'd')
        AddComponent(ImageRenderer(dKey));
            else if(key == 'm')
        AddComponent(ImageRenderer(spaceKey));
}

TutKey::TutKey(char zkey) {

    std::cout << "Placing key : " << zkey << std::endl;
    key = zkey;
}