//
// Created by xrf on 5/10/18.
//

#include "SpikeTool.hpp"

#include "../../../../Engine/InputManager.hpp"
#include "../../../../Tools.hpp"
#include "../../../Entities/Spike.hpp"
#include "../../../../Engine/UI.hpp"
#include <sstream>
#include <fstream>

Spike *SpikeTool::GetSpike(const int &x, const int &y) {

    for(auto spike : pEditor->worldSpikes){

        if((int)spike->position.x == x && (int)spike->position.y == y)
            return spike;
    }

    return nullptr;
}



void SpikeTool::OnUpdate() {

    auto mpos = InputManager::GetMousePos();
    mpos = SnapToGrid(mpos, 64);
    auto bpos = Camera::Instance().ScreenToWorldPoint(mpos);
    bpos = SnapToGrid(bpos, 64);

    mpos.first = (int)bpos.x;
    mpos.second = (int)bpos.y;

    auto pSpike = GetSpike(mpos.first, mpos.second + 32);

    if(InputManager::MouseClicked(SDL_BUTTON_LEFT)) {
        if (!pSpike) {
            pSpike = new Spike();
            pSpike->position.x = mpos.first;
            pSpike->position.y = mpos.second + 32;

            pEditor->worldSpikes.push_back(pSpike);

        }
    }else if(InputManager::MouseClicked(SDL_BUTTON_RIGHT)) {
        if (pSpike) {
            auto it = pEditor->worldSpikes.begin();

            while (it != pEditor->worldSpikes.end()) {

                if ((*it) == pSpike) {
                    (*it)->bDestroy = true;
                    it = pEditor->worldSpikes.erase(it);
                } else
                    ++it;
            }

        }
    }


}

void SpikeTool::OnRender() {

    auto mpos = InputManager::GetMousePos();
    mpos = SnapToGrid(mpos, 64);

    if(GetSpike(mpos.first, mpos.second + 32))
    UI::DrawRect({mpos.first, mpos.second + 32, 64, 32}, {255,0,0});
    else
        UI::DrawRect({mpos.first, mpos.second + 32, 64, 32}, {0,255,0});
}

void SpikeTool::OnActivate() {

}

void SpikeTool::OnDisable() {

}



SpikeTool::SpikeTool(LevelEditor *_pEditor) : pEditor(_pEditor){}

void SpikeTool::OnLoad(const std::string& szMapDir) {


    std::string lBuff;
    std::ifstream fileStream(szMapDir + ".sdata");
    while(std::getline(fileStream, lBuff))
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

        auto pSpike = new Spike();

        pSpike->position.x = x;
        pSpike->position.y = y;

        pEditor->worldSpikes.push_back(pSpike);

    }
    fileStream.close();

}

void SpikeTool::OnSave(const std::string& szMapDir) {


    std::ofstream fileStream(szMapDir + ".sdata",  std::ofstream::out);
    for(auto spike : pEditor->worldSpikes) {
        fileStream << (int)spike->position.x << ' ' << (int)spike->position.y << '\n';
    }
    fileStream.close();
    std::cout << "Finished Saving Spikes: " << pEditor->worldSpikes.size() << std::endl;

}

void SpikeTool::ForceRender() {

   for(auto pSpike : pEditor->worldSpikes){

     //8  pSpike.


   }


}
