//
// Created by xrf on 27/07/18.
//

#include "Entity.hpp"

#if __linux__
#include <cstring>
#else
#include <string>
#endif

#include "Engine.hpp"

//add entity when init,
//set all the pointers the components to 0, rather than what every memeory was there previously
BaseEntity::BaseEntity() : scale(1.0f), position({0.0f, 0.0f}), bDestroy(false), bNeedsStart(true){
    memset(components, 0, sizeof(components));
    Engine::AddEntity(this);
}

BaseEntity::BaseEntity(const float& scale)  : scale(scale), position({0.0f, 0.0f}), bDestroy(false), bNeedsStart(true){
    memset(components, 0, sizeof(components));
    Engine::AddEntity(this);
}

BaseEntity::BaseEntity(const float& scale, const Vector2& startingPos)  : scale(scale), position(startingPos), bDestroy(false), bNeedsStart(true){
    memset(components, 0, sizeof(components));
    Engine::AddEntity(this);
}

void Empty::OnDestroy() {

}

void Empty::Start() {

}
