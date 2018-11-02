#pragma once

#include "../../Engine/Component.hpp"
#include "../Entities/Player.hpp"
class PlayerController  : public Component<PlayerController>{

    public:

    Player* localPlayer = nullptr;

    float maxMoveSpeed = 2.4f;
    float maxAirSped = 2.0f;
    float acceleration = 3.5f;
    float maxFallSpeed = -32.0f;
    float jumpPower = 3.2f;
    float friction = 2.3f;
    float gravity = 0.04f;


    explicit PlayerController(Player* player) : localPlayer(player){}

    void Update(const float &deltaTime) override;

    void Render() override;

    void Start() override;

    void OnDestroy() override;

    void ApplyHorizontalFriction(const int& dir, const float& deltaTime);
    int ApplyHorizontalAcceleration(const float& deltaTime);

    bool SolveCollision(const float& deltaTime);

};

