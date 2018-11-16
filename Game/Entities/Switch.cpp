#include "Switch.hpp"

void Switch::Start()
{

    AddComponent(ImageRenderer("assets/switch.png", {0, 0, 16, 27}));
    scale = 2.0f;
    AddComponent(Collider(GetComponent<ImageRenderer>()));

    for(auto ent : Engine::Instance().entityList){

        std::pair<int,int> pos = {ent->position.x, ent->position.y};

        if(ent->GetComponent<Collider>())
            continue;

        if(toggleZone.Contains(pos.first, pos.second)){
            entitiesToToggle.push_back(ent);
        }
    }

    if(GameManager::Instance()->pCurrentLevel) {

        for (auto pCol : GameManager::Instance()->pCurrentLevel->worldColliders) {

            if (pCol->GetBounds().Intersects(toggleZone))
                colldiersToToggle.push_back(pCol);
        }

    }

    std::cout << colldiersToToggle.size() << " SIZE\n";
}

void Switch::OnUse() {

    bUsed = !bUsed;

    if(bUsed)
        scale = -2.0f;
    else
        scale =2.0f;

    std::cout << entitiesToToggle.size() << "entties \n";
    std::cout << colldiersToToggle.size() << "colldiers \n";

    for (auto ent : entitiesToToggle) {
        ent->bEnabled = !bUsed;
    }

    if(colldiersToToggle.size() == 0){
        for (auto pCol : GameManager::Instance()->pCurrentLevel->worldColliders) {

            if (pCol->GetBounds().Intersects(toggleZone))
                colldiersToToggle.push_back(pCol);
        }
    }

    for(auto pCol : colldiersToToggle) {
        std::cout <<";:;\n";
        pCol->bEnabled = !bUsed;
    }
}

