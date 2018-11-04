//
// Created by xrf on 4/10/18.
//

#include "ColliderTool.hpp"
#include "../../../../Engine/InputManager.hpp"
#include "../../../../Tools.hpp"
#include "../../../../Engine/UI.hpp"
#include <fstream>
#include <sstream>
#include <vector>

void ColliderTool::OnUpdate() {
    auto mousePos = InputManager::GetMousePos();

    if(InputManager::MouseClicked(SDL_BUTTON_RIGHT)){

        auto wPos = Camera::Instance().ScreenToWorldPoint(mousePos);

        auto it = pEditor->colliderList.begin();
        while(pEditor->colliderList.end() != it){

            if((*it).Contains((int)wPos.x, (int)wPos.y))
                it = pEditor->colliderList.erase(it);
            else ++it;
        }

        return;
    }


    mousePos = SnapToGrid(mousePos, 64);

    if (InputManager::MouseDown(SDL_BUTTON_LEFT)) {

        if (ZeroRect(selrect)) {
            selrect.x = mousePos.first;
            selrect.y = mousePos.second;
        }
    } else if (InputManager::MouseHeld(SDL_BUTTON_LEFT)) {
        selrect.w = mousePos.first - selrect.x;
        selrect.h = mousePos.second - selrect.y;

    } else if (InputManager::MouseClicked(SDL_BUTTON_LEFT)) {

        if(selrect.w == 0 || selrect.h == 0)
            selrect = {0,0,0,0};

        if (selrect.w != 0 && selrect.h != 0) {
            selrect = NormalizeRect(selrect);
            PlaceCollider();
            selrect = {0, 0, 0, 0};
        }
    }
}

void ColliderTool::OnRender() {

    UI::DrawRect((selrect), {255,0,0}, 2);
    auto mousePos = InputManager::GetMousePos();
    auto wPos = Camera::Instance().ScreenToWorldPoint(mousePos);



}

void ColliderTool::OnActivate() {

}

void ColliderTool::OnDisable() {

}

ColliderTool::ColliderTool(LevelEditor *_pEditor) : pEditor(_pEditor) {

}

void ColliderTool::PlaceCollider() {
    auto world = ScreenToWorld(selrect);
    pEditor->colliderList.push_back(world);
}

void ColliderTool::OnSave(const std::string& szMapDir) {

    std::ofstream fileStream(szMapDir + ".cdata",  std::ofstream::out);
    for(auto col : pEditor->colliderList) {
        fileStream << col.x << ' ' << col.y << ' ' << col.w << ' ' << col.h << '\n';
    }
    fileStream.close();
    std::cout << "Finished Saving Colliders: " << pEditor->colliderList.size() << std::endl;


}

void ColliderTool::OnLoad(const std::string& szMapDir) {
    std::string lBuff;
    std::ifstream fileStream(szMapDir + ".cdata");
    while(std::getline(fileStream, lBuff))
    {
        std::stringstream line(lBuff);
        std::string buff;

        int data[4] = {0,0,0,0};

        for (int i = 0; i < 4; i++) {
            line >> buff;
            data[i] = std::stoi(buff);
        }

        int x = data[0];
        int y = data[1];

        int w = data[2];
        int h = data[3];

        pEditor->colliderList.push_back({x,y,w,h});
    }
    fileStream.close();
}

void ColliderTool::ForceRender() {

}
