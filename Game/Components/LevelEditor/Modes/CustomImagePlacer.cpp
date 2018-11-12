#include <fstream>
#include <sstream>
#include "CustomImagePlacer.hpp"
#include "../../../../Engine/InputManager.hpp"
#include "../../../../Engine/Engine.hpp"
#include "../../../../Engine/UI.hpp"
#include "../../../Entities/TutKey.hpp"
#include "../../../../Engine/Camera.hpp"
#include <string>
int mde = 0;


static SDL2pp::Texture* aKey = nullptr;
static SDL2pp::Texture* dKey = nullptr;
static SDL2pp::Texture* spaceKey = nullptr;



void CustomImagePlacer::OnUpdate() {

    if(InputManager::MouseClicked(SDL_BUTTON_RIGHT)) {
        auto mpos = InputManager::GetMousePos();
        auto wpos = Camera::Instance().ScreenToWorldPoint(mpos);
        auto it = pEditor->mKeys.begin();
        while (it != pEditor->mKeys.end()) {

            auto rect = (*it)->GetComponent<ImageRenderer>()->srcrect;
            rect.x = (int) (*it)->position.x;
            rect.y = (int) (*it)->position.y;

            if (rect.Contains(wpos.x, wpos.y)) {
                (*it)->bDestroy = true;
                it = pEditor->mKeys.erase(it);
            } else
                it++;
        }
    }

    if(InputManager::MouseClicked(SDL_BUTTON_LEFT))
        PlaceKey();

    if(InputManager::KeyPressed(SDLK_k)) {
        if (mde < 2)
            mde++;
        else
            mde = 0;
    }

}

void CustomImagePlacer::OnRender() {

    if (mde == 0)
        UI::DrawTexture(aKey, InputManager::GetMousePos().first, InputManager::GetMousePos().second, 1.0f);
    else if (mde == 1)
        UI::DrawTexture(dKey, InputManager::GetMousePos().first, InputManager::GetMousePos().second, 1.0f);
    else if (mde == 2)
        UI::DrawTexture(spaceKey, InputManager::GetMousePos().first, InputManager::GetMousePos().second, 1.0f);


        auto mpos = InputManager::GetMousePos();
        auto wpos = Camera::Instance().ScreenToWorldPoint(mpos);
        auto it = pEditor->mKeys.begin();

        for(auto sKey : pEditor->mKeys){

            if(sKey->bNeedsStart)
                continue;

            auto rect = sKey->GetComponent<ImageRenderer>()->srcrect;
            rect.y = (int) sKey->position.y;
            rect.x = (int) sKey->position.x;

            auto spos = Camera::Instance().WorldToScreenPoint(sKey->position);

            if (rect.Contains(wpos.x, wpos.y)) {
                UI::DrawRect(spos.first, spos.second, rect.w, rect.h, {255,0,0}, 3);
            }
        }


}

void CustomImagePlacer::OnActivate() {

}

void CustomImagePlacer::OnDisable() {

}

void CustomImagePlacer::OnSave(const std::string& szMapDir) {

    std::ofstream fileStream(szMapDir + ".tdata",  std::ofstream::out);
    for(auto key : pEditor->mKeys) {
        fileStream << key->key << ' ' << (int)key->position.x << ' ' << (int)key->position.y << '\n';
    }
    fileStream.close();
    std::cout << "Finished Saving TutKeys: " << pEditor->mKeys.size() << std::endl;
}

CustomImagePlacer::CustomImagePlacer(LevelEditor *_pEditor) : pEditor(_pEditor) {

}

void CustomImagePlacer::OnLoad(const std::string& szMapDir) {

    aKey = Engine::LoadTexture("assets/key_a.png");
    dKey = Engine::LoadTexture("assets/key_d.png");
    spaceKey = Engine::LoadTexture("assets/key_space.png");


    std::string lBuff;
    std::ifstream fileStream(szMapDir + ".tdata");
    if(!fileStream)
        return;

    while(std::getline(fileStream, lBuff))
    {
        std::stringstream line(lBuff);
        std::string buff;

        char type = '5';
        int data[2] = {0,0};

        for (int i = 0; i < 3; i++) {
            line >> buff;
            if(i == 0){
                type = buff[0];
            }
            else
            data[i - 1] = std::stoi(buff);
        }

        int x = data[0];
        int y = data[1];

        std::cout << "Type " << type << std::endl;
        auto pKey = new TutKey(type);

        pKey->position.x = x;
        pKey->position.y = y;

        pEditor->mKeys.push_back(pKey);

    }
    fileStream.close();
}

void CustomImagePlacer::ForceRender() {}

void CustomImagePlacer::PlaceKey() {

    char ch = '5';

    if(mde == 0)
        ch = 'a';
    else if(mde == 1)
        ch = 'd';
    else if(mde == 2)
        ch = 'm';

   auto tkey = new TutKey(ch);
    pEditor->mKeys.push_back(tkey);
   auto mpos = InputManager::GetMousePos();
   auto wpo = Camera::Instance().ScreenToWorldPoint(mpos);
   tkey->position = wpo;

}
