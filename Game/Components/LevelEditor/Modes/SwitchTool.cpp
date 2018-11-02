//
// Created by xrf on 24/10/18.
//

#include "SwitchTool.hpp"
#include "../../../../Engine/Engine.hpp"
#include "../../../../Engine/UI.hpp"
#include "../../../Entities/Switch.hpp"
#include "../../../../Tools.hpp"
#include <sstream>
#include <SDL2/SDL.h>
#include <fstream>
#include <sys/stat.h>
#include "../../../../imgui/imgui.h"
static SDL2pp::Texture* pTexture = nullptr;

void SwitchTool::OnUpdate() {

    if (bEditSelection && activeSwitch != nullptr) {


        auto wPos = Camera::Instance().ScreenToWorldPoint(SnapToGrid(InputManager::GetMousePos(), 64));
        wPos = SnapToGrid(wPos, 64);



            if(InputManager::MouseDown(SDL_BUTTON_LEFT)) {
                activeSwitch->toggleZone.x = wPos.x;
                activeSwitch->toggleZone.y = wPos.y;
            }
           else if (InputManager::MouseHeld(SDL_BUTTON_LEFT)) {
                activeSwitch->toggleZone.w = wPos.x - activeSwitch->toggleZone.x;
                activeSwitch->toggleZone.h = wPos.y - activeSwitch->toggleZone.y;
            } else if (InputManager::MouseClicked(SDL_BUTTON_LEFT)) {
                if (activeSwitch->toggleZone.w != 0 && activeSwitch->toggleZone.h != 0) {
                    activeSwitch->toggleZone = NormalizeRect(activeSwitch->toggleZone);
                }
            }
            if (InputManager::MouseClicked(SDL_BUTTON_RIGHT)) {
                activeSwitch->toggleZone = {0, 0, 0, 0};

            }



        } else if (activeSwitch != nullptr) {

            if (InputManager::MouseClicked(SDL_BUTTON_LEFT)) {

                auto pos = InputManager::GetMousePos();
                pos = SnapToGrid(pos, 64);
                pos.second += 10;
                pos.first += 16 / 2;
                auto wpos = Camera::Instance().ScreenToWorldPoint(pos);
                wpos = SnapToGrid(wpos, 64);
                //      wpos.y += 0;
                wpos.x += 16 / 2;


                activeSwitch->position = {wpos.x, wpos.y};
            }
        }
    }


void SwitchTool::OnRender() {

    if(activeSwitch) {
        auto sp = Camera::Instance().WorldToScreenPoint(activeSwitch->position);
        UI::DrawRect(sp, 16 * 2, 27 * 2, {255, 0, 0});

        UI::DrawRect(WorldToScreen(activeSwitch->toggleZone), {255,0,0});

    }
    ImGui::Text("Switches");


    if(ImGui::Button("Add Switch")){


        auto swit = new Switch({0,0,0,0});
        allSwitches.push_back(swit);
        swit->position = Camera::Instance().ScreenToWorldPoint(1280/2, 720/2);
        activeSwitch = swit;
    }

    auto it = allSwitches.begin();

int id = 0;
    while(it != allSwitches.end()) {


        if (activeSwitch == (*it)) {


            if (bEditSelection) {


                if (ImGui::Button(("Switch " + std::to_string(id)).c_str())) {
                    activeSwitch = (*it);
                    bEditSelection = false;
                }
                ImGui::SameLine();
                ImGui::Text("Selected");

            } else {
                ImGui::Text("Selected");
                ImGui::SameLine();
                if(ImGui::Button(("Toggle Zone " + std::to_string(id)).c_str())){
                    activeSwitch = (*it);
                    bEditSelection = true;
                }

            }

            ImGui::SameLine();

            if(ImGui::Button(("Remove" + std::to_string(id)).c_str())) {
                (*it)->bDestroy = true;
                it = allSwitches.erase(it);
                activeSwitch = nullptr;
                break;
            }
            id++;
            ++it;
            continue;
        }


        if (ImGui::Button(("Switch " + std::to_string(id)).c_str())) {
            activeSwitch = (*it);
            bEditSelection = false;
        }
        ImGui::SameLine();
      if(ImGui::Button(("Toggle Zone " + std::to_string(id)).c_str())){
          activeSwitch = (*it);
          bEditSelection = true;
      }

      id++;
      ++it;
    }






}

void SwitchTool::OnActivate() {

}

void SwitchTool::OnDisable() {

}

void SwitchTool::OnSave(const std::string& szMapDir) {

    std::ofstream fSaveMap(szMapDir + ".zdata",  std::ofstream::out);

    for(auto swi : allSwitches){
        fSaveMap << (int)swi->position.x << ' ' << (int)swi->position.y  <<  swi->toggleZone.x << ' ' <<  swi->toggleZone.y << ' '<<  swi->toggleZone.w << ' ' <<  swi->toggleZone.h << '\n';
    }

    std::cout << "Finished Switch" << std::endl;
    fSaveMap.close();
}

SwitchTool::SwitchTool(LevelEditor *_pEditor) : pEditor(_pEditor){

}

void SwitchTool::OnLoad(const std::string& szMapDir) {

    pTexture = Engine::LoadTexture("assets/switch.png");

    std::string lBuff;
    std::ifstream fileStream(szMapDir + ".zdata");

    if (!fileStream) {
        return;
    }


    while (std::getline(fileStream, lBuff)) {
        std::stringstream line(lBuff);
        std::string buff;

        int data[6] = {0, 0, 0, 0, 0, 0};

        for (int i = 0; i < 7; i++) {
            line >> buff;
            data[i] = std::stoi(buff);
        }

        Vector2 pos = {data[0], data[1]};

        auto str = new Switch({data[2], data[3], data[4], data[5]});
        str->position = pos;
        allSwitches.push_back(str);
    }

    fileStream.close();


}

void SwitchTool::ForceRender() {

}
