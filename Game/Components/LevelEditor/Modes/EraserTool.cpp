//
// Created by xrf on 4/10/18.
//

#include "EraserTool.hpp"
#include "../../../../Engine/InputManager.hpp"
#include "../../../../Tools.hpp"
#include "../../../../Engine/UI.hpp"
#include "../LevelEditor.hpp"

void EraserTool::OnUpdate() {

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

        if(selrect.w == 0 || selrect.h == 0)
            selrect = {0,0,0,0};

        if (selrect.w != 0 && selrect.h != 0) {
            selrect = NormalizeRect(selrect);

            Delete();

            selrect = {0, 0, 0, 0};
        }
    }

}

void EraserTool::OnRender() {
    UI::DrawRect((selrect), {255,0,0}, 2);
}

void EraserTool::OnActivate() {

}

void EraserTool::OnDisable() {

}

void EraserTool::OnSave(const std::string& szMapDir) {

}

void EraserTool::OnLoad(const std::string& szMapDir) {

}

EraserTool::EraserTool(LevelEditor *_pEditor) : pEditor(_pEditor){

}

void EraserTool::Delete() {

    selrect = ScreenToWorld(selrect);

    for (int i = selrect.x; i < selrect.x + selrect.w; i += 64) {
        for (int j = selrect.y; j < selrect.y + selrect.h; j += 64) {

            auto ent = pEditor->GetEntity(i,j);

            if (ent)
                pEditor->RemoveTile(ent);
        }
    }


}

void EraserTool::ForceRender() {

}
