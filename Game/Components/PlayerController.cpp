
#include "PlayerController.hpp"
#include "../../Engine/InputManager.hpp"
#include "../../Engine/Entity.hpp"
#include "../../Engine/UI.hpp"
#include "../../Engine/Engine.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_keycode.h>
#include "../../Engine/Vector2.hpp"
#include "ImageRenderer.hpp"
#include "../../Engine/Camera.hpp"
#include "AnimatedRenderer.hpp"
#include "../Background.hpp"
#include "Collider.hpp"
#include <SDL2/SDL.h>
#include <map>

static AnimatedRenderer* pAnimController = nullptr;
void PlayerController::Render() {

    SDL_Color col = {255,0,0};
    UI::DrawString(localPlayer->velocity.x, 20,20, 3, {255,0,0});
   UI::DrawString(localPlayer->velocity.y, 20,50, 3, {255,0,0});

   static SDL2pp::Texture* heart = Engine::LoadTexture("assets/heart.png");
    static SDL2pp::Texture* nheart = Engine::LoadTexture("assets/nheart.png");
   for(int i = 0; i < 3; i++){

       if(i <= localPlayer->lives)
       UI::DrawTexture(heart, Camera::Instance().viewport.w - 40 - (heart->GetWidth()*2) - ((heart->GetWidth()*2) * i + i*6), 40, 2.0f);
        else
           UI::DrawTexture(nheart, Camera::Instance().viewport.w - 40 - (heart->GetWidth()*2) - ((heart->GetWidth()*2) * i + i*6), 40, 2.0f);
   }

    Camera::Instance().viewport.x = static_cast<int>(localPlayer->position.x) - 640 - 44;
    Camera::Instance().viewport.y = static_cast<int>(localPlayer->position.y) - 360  - 22;

}

void PlayerController::Start() {
    pAnimController = entity->GetComponent<AnimatedRenderer>();
    std::cout << Empty::EID() << std::endl;
}



void PlayerController::Update(const float &deltaTime) {

    if(InputManager::KeyDown(SDLK_0)) {
        localPlayer->position = {60.0f, 320.0f};
        localPlayer->velocity = Vector2::Zero;
    }
    int dir = ApplyHorizontalAcceleration(deltaTime);

    ApplyHorizontalFriction(dir, deltaTime);

    if(localPlayer->velocity.y < maxFallSpeed)
        localPlayer->velocity.y = maxFallSpeed;

    if(localPlayer->velocity.x == 0 && localPlayer->velocity.y == 0)
        pAnimController->AdjustSpritePosition(0);
    else {
        pAnimController->AdjustSpritePosition(64);
        if(localPlayer->velocity.x == maxMoveSpeed)
        pAnimController->AdjustAnimSpeed(200);
        else
            pAnimController->AdjustAnimSpeed(350);
    }

    if(SolveCollision(deltaTime)) {

        if (localPlayer->velocity.y == 0 && InputManager::KeyHeld(SDLK_SPACE)) {
            localPlayer->velocity.y = jumpPower;
        }
    } else
        localPlayer->velocity.y -= gravity;

    localPlayer->position.y -= (localPlayer->velocity.y);
    localPlayer->position.x += (localPlayer->velocity.x);

    BG::bounds.x -= (int)localPlayer->velocity.x;
    BG::bounds.y += (int)localPlayer->velocity.y;
}

void PlayerController::OnDestroy() {

}

void PlayerController::ApplyHorizontalFriction(const int& dir, const float& deltaTime) {
    if(dir == 0){
        if(localPlayer->velocity.x > 0){
            localPlayer->velocity.x -= friction * deltaTime;
            localPlayer->velocity.x = std::clamp(localPlayer->velocity.x, 0.0f, abs(localPlayer->velocity.x));
        }
        else if(localPlayer->velocity.x < 0){
            localPlayer->velocity.x += friction * deltaTime;
            localPlayer->velocity.x = std::clamp(localPlayer->velocity.x, -abs(localPlayer->velocity.x), 0.0f);
        }
    }
}

int PlayerController::ApplyHorizontalAcceleration(const float& deltaTime) {

    int dir = 0;

    if(InputManager::KeyHeld(SDLK_a))
        dir -= 1;
    else if(InputManager::KeyHeld(SDLK_d))
        dir += 1;

    switch (dir) {
        case -1 :
            localPlayer->scale = -abs(localPlayer->scale);
            localPlayer->velocity.x -= acceleration * deltaTime;
            break;

        case 1:
            localPlayer->scale = abs(localPlayer->scale);
            localPlayer->velocity.x += acceleration * deltaTime;
            break;
    }

    localPlayer->velocity.x = std::clamp(localPlayer->velocity.x, -maxMoveSpeed, maxMoveSpeed);

    return dir;
}




bool PlayerController::SolveCollision(const float& deltaTime) {

    SDL2pp::Rect future = localPlayer->GetFutureBounds(deltaTime);
    auto player = localPlayer->GetBounds();
//    UI::DrawRect(Camera::Instance().WorldToScreenPoint(player.x, player.y), player.w, player.h, {255,0,0}, 2);

    auto hits = localPlayer->GetCollisions(deltaTime);

    bool bGrounded = false;

    for(auto hit : hits) {

       auto right = abs(hit.x + hit.w - player.x);
       auto left = abs(player.x + player.w - hit.x);
       auto bottom = abs(hit.y + hit.h - player.y);
       auto top = abs(player.y + player.h - hit.y);

        if(left < right && left < top && left && left < bottom){ //hit right side??
            localPlayer->position.x = hit.x - player.w - 32;
            localPlayer->velocity.x = 0;

        }else if(right < left && right < top && right < bottom){
            localPlayer->position.x = hit.x + hit.w - 32;//hit left side
            localPlayer->velocity.x = 0;
        }else if(top < left && top < right && top < bottom){//ground
            localPlayer->position.y = hit.y - player.h + 1;
            localPlayer->velocity.y = 0;
            bGrounded = true;
        }else if(bottom < left && bottom < right && bottom < top){//Hit head
            localPlayer->position.y = hit.y + hit.h;
            localPlayer->velocity.y = 0;
        }
    }

    return bGrounded;
}

