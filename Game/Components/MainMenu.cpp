//
// Created by xrf on 8/10/18.
//

#include "MainMenu.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Engine/UI.hpp"
#include "../Entities/Player.hpp"
#include "../../Engine/Button.hpp"
#include "../Components/LevelEditor/LevelEditor.hpp"
#include "../../imgui/imgui.h"
#include "../Entities/GameManager.hpp"
#include "../../Engine/Camera.hpp"
#include <fstream>

static Camera* pCam = nullptr;
static SDL2pp::Texture *pTexture = nullptr;
static int bpX = 0;
static int bpY = 0;
static int bpY2 = 0;
std::vector<std::pair<int*, int>> mapButtonOffsets{};

void MainMenu::Update(const float &deltaTime) {
    bpX = pCam->viewport.w / 2 - ((pTexture->GetWidth() * 4) / 2) + 110;
    bpY = pCam->viewport.h / 2 - ((pTexture->GetHeight() * 4) / 2) + 180;
    bpY2 = pCam->viewport.h / 2 - ((pTexture->GetHeight() * 4) / 2) + 240;


    for(auto mpo : mapButtonOffsets){
        *mpo.first = bpY2 = pCam->viewport.h / 2 - ((pTexture->GetHeight() * 4) / 2) + mpo.second;
    }

}
bool bbDe = false;
bool playClosed = true;
void MainMenu::Render() {


    auto vp = pCam->viewport;

    static int scale = 4;
    if (!bbDe || !bEndMenuClosed || !playClosed) {
        UI::DrawTexture(pTexture, (pCam->viewport.w / 2) - (pTexture->GetWidth() * scale) / 2,
                        ( pCam->viewport.h / 2) - (pTexture->GetHeight() * scale) / 2, (float) scale);
        UI::DrawString("Memegeon",pCam->viewport.w / 2 - 60, pCam->viewport.h/2 - (pTexture->GetHeight() * scale)/2 + 40, 5, {0, 0, 0});
    }
}


void loadMap(const std::string map){
    std::cout << map << " wtf dennis \n";
    GameManager::Instance()->pCurrentLevel = Level::LoadLevel(map);
    auto pPlayer = new Player;
    playClosed = true;
}

void MainMenu::Start() {

    pCam = &Camera::Instance();
    pTexture = Engine::LoadTexture("assets/ui_bg.png");

    bpX = pCam->viewport.w/2 - (pTexture->GetWidth() * 4)/2 + 100;
    bpY = pCam->viewport.h/2 - (pTexture->GetHeight() * 4)/2 + 100;
    bpY = pCam->viewport.h/2 - (pTexture->GetHeight() * 4)/2 + 160;

    std::function<void(void)> a = [&](){

        std::ifstream fileStream("alllevels");
        std::string lBuff;
        bool exists = false;
        int index = 0;

        while(std::getline(fileStream, lBuff))
        {
            std::cout << "plz work " << lBuff.c_str();

            int* yPos = new int;
            mapButtonOffsets.push_back({yPos, 180 + index*60});
            std::function<void()> aa = std::bind(&loadMap, lBuff + "/map");

            auto play = new Button(lBuff.c_str(), aa, {&bpX, yPos}, &playClosed, 2);
            index++;
        }
        fileStream.close();

        std::cout << "INdex " << index << std::endl;

        bbDe = true;
        playClosed = false;
    };

    std::function<void(void)> b = [&](){
 //       auto le = new Player;
        entity->AddComponent(LevelEditor());
        bbDe = true;
    };


    auto play = new Button("Play", a, {&bpX, &bpY}, &bbDe, 2);
    auto leveleditor = new Button("LevelEditor", b, {&bpX, &bpY2}, &bbDe, 2);

/*    auto b = new Button(a, {300,300,200,100}, &bbDe);
    b->SetTopTexture("Play", {0,0,0}, {255,255,255});
    b->SetBottomTexture(Engine::LoadTexture("assets/ui_button.png"), nullptr);

*/
}

void MainMenu::OnDestroy() {

}

//memory leak central
void MainMenu::OnGameEnd(bool bWon) {

    bEndMenuClosed = false;

    std::function<void(void)> fQuit = [&](){
        Engine::Stop();
    };


    if(bWon){

        std::function<void(void)> b = [&](){
            bEndMenuClosed = true;
        };

        auto nl = new Button("Next Level", b, {&bpX, &bpY}, &bEndMenuClosed, 2);
        auto dl = new Button("Quit", fQuit, {&bpX, &bpY2}, &bEndMenuClosed, 2);


    }else{

        std::function<void(void)> b = [&](){
            bEndMenuClosed = true;
        };
        auto nl = new Button("Retry", b, {&bpX, &bpY},  &bEndMenuClosed, 2);
        auto dl = new Button("Quit", fQuit, {&bpX, &bpY2}, &bEndMenuClosed, 2);



    }


}
