//
// Created by xrf on 4/10/18.
//

#include "DoorTool.hpp"
#include "../../../../Engine/InputManager.hpp"
#include "../../../../Tools.hpp"
#include "../../../../Engine/UI.hpp"
#include "../LevelEditor.hpp"
#include <fstream>
#include <sstream>
#include "../../../Entities/Door.hpp"
#include "../../../../Engine/Engine.hpp"
void DoorTool::OnUpdate() {
    auto mousePos = InputManager::GetMousePos();
    mousePos = SnapToGrid(mousePos, 64);

    if (InputManager::MouseDown(SDL_BUTTON_LEFT)) {
       Place();
    }
}

void DoorTool::OnRender() {

    auto pos = InputManager::GetMousePos();
    pos = SnapToGrid(pos,64);

    auto sp = (Camera::Instance().WorldToScreenPoint(pEditor->doorPosition));

    UI::DrawRect({pos.first, pos.second + 32, 64, 96}, {0,255,0}, 2);

}

void DoorTool::OnActivate() {

}

void DoorTool::OnDisable() {

}

void DoorTool::OnSave(const std::string& szMapDir) {

    std::ofstream fSaveMap(szMapDir + ".ddata",  std::ofstream::out);
        fSaveMap << pEditor->doorPosition.first << ' ' << pEditor->doorPosition.second  << '\n';
    std::cout << "Finished Door" << std::endl;
    fSaveMap.close();



}

void DoorTool::OnLoad(const std::string& szMapDir) {

    std::string lBuff;
    std::ifstream fileStream(szMapDir + ".ddata");

    if(!fileStream)
        return;

    std::getline(fileStream, lBuff);
    {
        std::stringstream line(lBuff);
        std::string buff;

        int data[2] = {0, 0};

        for (int i = 0; i < 2; i++) {
            line >> buff;
            data[i] = std::stoi(buff);
        }

        int x = data[0];

        int y = data[1];

        pEditor->doorPosition = {x, y};
    }

    fileStream.close();
}

DoorTool::DoorTool(LevelEditor *_pEditor) : pEditor(_pEditor) {

}

void DoorTool::Place() {

    auto wpos = SnapToGrid(Camera::Instance().ScreenToWorldPoint(SnapToGrid(InputManager::GetMousePos(), 64)), 64);
    pEditor->doorPosition = std::pair<int, int>{(int) wpos.x, (int) wpos.y};
};

void DoorTool::ForceRender() {
    auto sp = (Camera::Instance().WorldToScreenPoint(pEditor->doorPosition));

    static SDL2pp::Texture* pTexture = Engine::LoadTexture("assets/duntiles.png");

UI::DrawTexture(pTexture, {0,0, 32, 64}, sp.first, sp.second, 2);
}
