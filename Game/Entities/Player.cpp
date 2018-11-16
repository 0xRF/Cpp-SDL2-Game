#include "Player.hpp"
#include "../Components/PlayerController.hpp"
#include "../Components/ImageRenderer.hpp"
#include "../../Engine/Engine.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Editor.hpp"

#include "../../Engine/UI.hpp"
#include "../../Engine/Camera.hpp"
#include "../Components/AnimatedRenderer.hpp"
#include <algorithm>
#include "../Components/LevelEditor/LevelEditor.hpp"
#include "../Entities/Spike.hpp"
#include "../Entities/Key.hpp"
#include "../Entities/Door.hpp"
#include "../Entities/GameManager.hpp"
#include "../Entities/Switch.hpp"
static Collider* pCollider = nullptr;
void Player::Start() {


    AddComponent(ImageRenderer("assets/Assassin-1.png", {8,64,32,32}));
    AddComponent(AnimatedRenderer(GetComponent<ImageRenderer>(), 10, 230));
    AddComponent(PlayerController(this));
   // AddComponent(Editor());
   pCollider = AddComponent(Collider({32,0,12, 32}));

}

void Player::OnDestroy() {

}

bool Player::Collides(SDL2pp::Rect* pHit) {

    static std::vector<Collider*>* colliders = &GameManager::Instance()->pCurrentLevel->worldColliders;

    for (auto &pCol : *colliders) {

        if (!pCol)continue;

        if(pCol->entity == this)
            continue;

        if(!pCol->bEnabled)
            continue;

        if (pCol->Collides(pCollider->GetBounds())) {

            if(pHit) {
                pHit->x = (int) pCol->entity->position.x;
                pHit->y = (int) pCol->entity->position.y;
                pHit->w = pCol->GetBounds().w;
                pHit->h = pCol->GetBounds().h;
            }
            return true;
        }
    }

    for (auto &pCol : GetComponent<Editor>()->colliderList) {

        //Red Start
      //  UI::DrawRect(Camera::Instance().WorldToScreenPoint(pCol.x, pCol.y), 32,32, {255,0,0}, 1);

        //Green Ending
       // UI::DrawRect(Camera::Instance().WorldToScreenPoint(pCol.x + pCol.w - 32, pCol.y + pCol.h - 32), 32,32, {0,255,0}, 1);
      //  UI::DrawRect(Camera::Instance().WorldToScreenPoint(pCol.x, pCol.y), pCol.w,pCol.h, {0,0,255}, 1);

        if (pCol.Intersects(pCollider->GetBounds())) {

            if(pHit)
                *pHit = pCol;

            return true;
        }
    }

    return false;
}



std::vector<SDL2pp::Rect> Player::GetCollisions(const float& deltaTime) {

    std::vector<Collider *>* allColls = &GameManager::Instance()->pCurrentLevel->worldColliders;


//    UI::DrawString((std::to_string(GameManager::Instance()->pCurrentLevel->bGrabbedKey) + " Collected Key").c_str(), 100,100, 3, {255,0,0});


    auto ret = std::vector<SDL2pp::Rect>();

    if(GetComponent<LevelEditor>()) {
        for (auto col : GetComponent<LevelEditor>()->colliderList) {

            //      if(FIntersects(col, position, GetBounds())){
            if (col.Intersects(GetBounds())) {
                UI::DrawRect(Camera::Instance().WorldToScreenPoint(col.x, col.y), col.w, col.h, {255, 0, 0}, 2);
                ret.push_back(col);
            }
        }

    }else {


        auto it = allColls->begin();
        while (it != allColls->end()) {

            if ((!(*it)->GetBounds().Intersects(GetBounds())) || (*it)->entity == this) {
                ++it;
                continue;
            }

            if((*it)->entity->ID() == Switch::EID()) {

                UI::DrawString("F to use", 1280/2, 50, 3, {0,255,99});

                if (InputManager::KeyDown(SDLK_f)) {

                    Switch *sw = (Switch *) ((*it))->entity;
                    sw->OnUse();
                }
                ++it;
                continue;
            }

            if(!(*it)->bEnabled) {
                ++it;
                continue;
            }
       //     std::cout << Door::EID() << " DOOR \n";
         //   std::cout << Empty::EID() << " Empty \n";
          //  std::cout << (*it)->entity->ID() << " ID \n";

            if ((*it)->entity->ID() == Spike::EID())
                HurtMe();
            else if ((*it)->entity->ID() == Key::EID() && !GameManager::Instance()->pCurrentLevel->bGrabbedKey) {
                GameManager::Instance()->pCurrentLevel->bGrabbedKey = true;
                (*it)->entity->bDestroy = true;
                it = allColls->erase(it);
                continue;

            } else if ((*it)->entity->ID() == Door::EID()) {

                if (GameManager::Instance()->pCurrentLevel->bGrabbedKey) {
                    //Level complete

                    GameManager::Instance()->GameEnd(true);
                }

            }
            else {
                ret.push_back((*it)->GetBounds());
            }

            ++it;
        }
    }



    return ret;
}

SDL2pp::Rect Player::GetFutureBounds(const float &deltaTime) {

    auto bounds = pCollider->GetBounds();

    return {static_cast<int>(bounds.x + velocity.x), static_cast<int>(bounds.y + velocity.y), bounds.w, bounds.h};
}

SDL2pp::Rect Player::GetBounds() {
    return pCollider->GetBounds();
}

void Player::HurtMe() {

    static SDL2pp::Chunk *pSound = new SDL2pp::Chunk("assets/hurt.mp3");
    Engine::PlaySound(pSound);

    lives--;

    if (lives == 0) {
        GameManager::Instance()->GameEnd(false);
        bDestroy = true;
    } else {
        position = {300.0f, 300.0f};
        velocity = Vector2::Zero;
    }
}

void Player::CollectedKey(Key* pKey) {

}

