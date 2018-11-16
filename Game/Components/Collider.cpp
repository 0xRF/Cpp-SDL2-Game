//
// Created by rowan fimmano on 21/7/18.
//

#include "Collider.hpp"
#include "../../Engine/Entity.hpp"
#include "ImageRenderer.hpp"
#include "../../Engine/UI.hpp"
#include "../../Engine/Camera.hpp"
#include "../Entities/GameManager.hpp"
void Collider::Update(const float &deltaTime) {

}

void Collider::Render() {
   auto a = Camera::Instance().WorldToScreenPoint(entity->position);

   UI::DrawRect(a.first, a.second, static_cast<int>(GetBounds().w), static_cast<int>(GetBounds().h), {0,0,255},5);



}



void Collider::Start() {
    if(bdone){
        std::cout << "WTF\n";
    }

    if(GameManager::Instance()->pCurrentLevel)
        GameManager::Instance()->pCurrentLevel->worldColliders.push_back(this);

    bdone = true;
}

void Collider::OnDestroy() {

}

Collider::Collider(const int &w, const int &h) : width(w), height(h), offsets({0,0}) {
}

 Collider::Collider(SDL2pp::Rect bounds) : width(bounds.w), height(bounds.h), offsets(bounds.x, bounds.y){

}

Collider::Collider(ImageRenderer* rend) : width(rend->srcrect.w), height(rend->srcrect.h) {


}

bool Collider::Collides(const SDL2pp::Rect &other) {
    return GetBounds().Intersects(other);
}

bool Collider::Collides(const int &x, const int &y) {
    return GetBounds().Contains(x,y);
}

bool Collider::Collides(const SDL2pp::Rect &other, const Vector2 &velocity) {

    return GetBounds().Intersects({(int)(other.x + velocity.x), (int)(other.y + velocity.y), other.w, other.h});
}

bool Collider::Collides(const int &x, const int &y, const Vector2 &velocity) {
    return GetBounds().Contains((int)(x + velocity.x), (int)(y + velocity.y));
}


bool Collider::Collides(const float &x, const float &y, const Vector2 &velocity) {
    return GetBounds().Contains((int)(x + velocity.x), (int)(y + velocity.y));
}

bool Collider::Collides(const Vector2 &other, const Vector2 &velocity) {
    return GetBounds().Contains((int)(other.x + velocity.x), (int)(other.y + velocity.y));
}

const SDL2pp::Rect Collider::GetBounds() const {
    return {
            (int)entity->position.x + offsets.first,
            (int)entity->position.y + offsets.second,
            (int)(abs(entity->scale) * width),
            (int)(abs(entity->scale) * height)
    };
}


