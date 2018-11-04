//
// Created by xrf on 7/10/18.
//

#include "KeyTool.hpp"
#include "../LevelEditor.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include "../../../../Engine/InputManager.hpp"
#include "../../../../Tools.hpp"
#include "../../../../Engine/Engine.hpp"
#include "../../../../Engine/UI.hpp"
#include "../../../Entities/Key.hpp"

static SDL2pp::Texture* pTexure = nullptr;


void KeyTool::OnRender() {

    auto mpos = SnapToGrid(InputManager::GetMousePos(), 64);

    if (pTexure) {

        UI::DrawTexture(pTexure, mpos.first, mpos.second, 1.0f);
        UI::DrawRect(mpos.first, mpos.second, 64, 64, {255, 0, 0}, 2);
    }
}

void KeyTool::OnUpdate() {

    if(InputManager::MouseClicked(SDL_BUTTON_LEFT)){
        auto mpos = SnapToGrid(InputManager::GetMousePos(), 64);
        auto wpos = Camera::Instance().ScreenToWorldPoint(mpos);
        wpos = SnapToGrid(wpos,64);
        mpos = {(int)wpos.x, (int)wpos.y};
        pEditor->keyPosition = mpos;

        if(pKey)
            pKey->position = {mpos.first, mpos.second};
    }
}


void KeyTool::OnActivate() {

}

void KeyTool::OnDisable() {

}

void KeyTool::OnSave(const std::string& szMapDir) {


    std::ofstream fSaveMap(szMapDir + ".kdata",  std::ofstream::out);
    fSaveMap << pEditor->keyPosition.first << ' ' << pEditor->keyPosition.second  << '\n';
    std::cout << "Finished Door" << std::endl;
    fSaveMap.close();

}

void KeyTool::OnLoad(const std::string& szMapDir) {

    pTexure =  Engine::LoadTexture("assets/kagari.png");
    std::string lBuff;
    std::ifstream fileStream(szMapDir + ".kdata");

    if(!fileStream)
    {
        return;
    }

    std::getline(fileStream, lBuff);
    {
        std::stringstream line(lBuff);
        std::string buff;

        int data[2] = {0,0};

        for (int i = 0; i < 2; i++) {
            line >> buff;
            data[i] = std::stoi(buff);
        }

        int x = data[0];
        int y = data[1];

        pEditor->keyPosition = {x,y};
    }
    fileStream.close();

    if(pEditor->keyPosition.first != -1) {
        pKey = new Key();
        pKey->position = {pEditor->keyPosition.first, pEditor->keyPosition.second};
    }
}

KeyTool::KeyTool(LevelEditor *_pEditor) : pEditor(_pEditor){}



void KeyTool::ForceRender() {
    if(pTexure)
    UI::DrawTexture(pTexure, pEditor->keyPosition.first, pEditor->keyPosition.second, 1.0f);
}