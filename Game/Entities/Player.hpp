#pragma once

#include <SDL2pp/Rect.hh>
#include "../../Engine/Entity.hpp"
class Key;
class Player : public Entity<Player>{

public:

    Vector2 velocity = {0.0f, 0.0f};

    Player() : Entity(3.0f, {300.0f, 300.0f}), velocity(0.0f, 0.0f){}

    void Start() override;

    void OnDestroy() override;

    bool Collides(SDL2pp::Rect* pHit);

    int lives = 3;

    std::vector<SDL2pp::Rect> GetCollisions(const float& deltaTime);

    SDL2pp::Rect GetFutureBounds(const float& deltaTime);
    SDL2pp::Rect GetBounds();


    void HurtMe();
private:

    void CollectedKey(Key* pKey);

};
