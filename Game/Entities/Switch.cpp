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

    std::cout << entitiesToToggle.size() << "a \n";
    std::cout << colldiersToToggle.size() << "b \n";

    for (auto ent : entitiesToToggle) {
        ent->bEnabled = !ent->bEnabled;
    }

    for(auto pCol : colldiersToToggle) {
        pCol->bEnabled = !pCol->bEnabled;
    }
}

