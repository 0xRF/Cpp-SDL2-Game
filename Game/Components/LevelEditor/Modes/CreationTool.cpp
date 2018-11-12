
#include "CreationTool.hpp"
#include "../LevelEditor.hpp"
#include "../../../../Engine/UI.hpp"
#include "../../../../Engine/InputManager.hpp"
#include "../../../../Tools.hpp"
#include "../../../../Engine/Entity.hpp"
#include "../../ImageRenderer.hpp"



Empty* CreationTool::GetEntity(std::pair<int, int> pos) {

    for(auto ent : pEditor->worldTileList)
    {
        if((int)ent->position.x == pos.first && (int)ent->position.y == pos.second)
            return ent;
    }

    return nullptr;
}

void CreationTool::OnUpdate(){

//auto mousePos = InputManager::GetMousePos();
    if(InputManager::MouseClicked(SDL_BUTTON_LEFT)){


        auto mousePos = SnapToGrid(InputManager::GetMousePos(), 64);

        int index = 0;

        for (int i = 0; i < pEditor->tilesCopyBuffer.first.first; i++) {
            for (int j = 0; j < pEditor->tilesCopyBuffer.first.second; j++) {

                auto tile = pEditor->tilesCopyBuffer.second.at(index);

                if(!(tile.first == -1 || tile.second == -1)) {

                    auto empty = new Empty;
                    empty->AddComponent(ImageRenderer(pEditor->pTileMap,
                                                      {tile.first / 2, tile.second / 2, 32, 32}));

                    std::pair<int, int> sp = {mousePos.first + 64 * i, mousePos.second + 64 * j};
                    sp = SnapToGrid(sp, 64);

                    auto pos = SnapToGrid(Camera::Instance().ScreenToWorldPoint(sp), 64);;

                    auto ent = GetEntity({(int)pos.x, (int)pos.y});
                    if(ent){
                        pEditor->RemoveTile(ent);

                    }


                    empty->position = pos;

                    empty->scale = 2.0f;
                    pEditor->worldTileList.push_back(empty);
                }

               index++;
            }
        }
    }

}

void CreationTool::OnRender() {


    auto mpos = SnapToGrid(InputManager::GetMousePos(), 64);


    int index = 0;

    for (int i = 0; i < pEditor->tilesCopyBuffer.first.first; i++) {
        for (int j = 0; j < pEditor->tilesCopyBuffer.first.second; j++) {

            auto tile = pEditor->tilesCopyBuffer.second.at(index);

            if(!(tile.first == -1 || tile.second == -1)) {
                UI::DrawTexture
                        (pEditor->pTileMap, {tile.first / 2, tile.second / 2, 32, 32}, mpos.first + 64 * i,
                         mpos.second + 64 * j, 2.0f);
            }
            index++;
        }
    }
}

void CreationTool::OnDisable(){}

void CreationTool::OnActivate(){}



void CreationTool::OnLoad(const std::string& szMapDir) {

}


void CreationTool::ForceRender() {

}

void CreationTool::OnSave(const std::string &szMapDir) {

}
