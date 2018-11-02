#include "SelectionTool.hpp"
#include "../../../../Engine/InputManager.hpp"
#include "../../../../Tools.hpp"
#include "../../../../Engine/UI.hpp"
#include "../LevelEditor.hpp"
#include "../../../../Engine/Entity.hpp"
#include "../../ImageRenderer.hpp"
#include <algorithm>

void SelectionTool::OnActivate(){

}

void SelectionTool::OnRender(){
  UI::DrawRect((selrect), {255,0,0}, 2);
}

void SelectionTool::OnUpdate() {

    auto mousePos = InputManager::GetMousePos();
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
        if (selrect.w != 0 && selrect.h != 0) {
            selrect = NormalizeRect(selrect);
            GrabTiles();
            selrect = {0, 0, 0, 0};
        }
    }
}

void SelectionTool::OnDisable(){
  
}

//to check over
void SelectionTool::GrabTiles() {
    auto worldRect = ScreenToWorld(selrect);

    std::vector<Empty *> pvecs = std::vector<Empty *>();

    std::vector<std::pair<int,int>> srcrects{};

    for(int i = selrect.x; i < selrect.x + selrect.w; i+=64){
        for(int j = selrect.y; j < selrect.y + selrect.h; j+=64){

            auto pEnt = GetEntity({i,j});

            if(!pEnt){
                srcrects.push_back({-1,-1});
            }else
            {
                auto src = pEnt->GetComponent<ImageRenderer>()->srcrect;

                srcrects.push_back({src.x*2, src.y*2});
            }


        }
    }
    pEditor->tilesCopyBuffer = {{selrect.w/64, selrect.h/64}, srcrects};

}

Empty* SelectionTool::GetEntity(std::pair<int, int> pos) {

    for(auto ent : pEditor->worldTileList)
    {
        if((int)ent->position.x == pos.first && (int)ent->position.y == pos.second)
            return ent;
    }

    return nullptr;
}

void SelectionTool::OnSave(const std::string& szMapDir) {

}

void SelectionTool::OnLoad(const std::string& szMapDir) {

}

void SelectionTool::ForceRender() {

}


