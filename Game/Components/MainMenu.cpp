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
#include "../Entities/Player.hpp"
#include "../Components/Collider.hpp"
#include "../Entities/Spike.hpp"
#include "../Entities/TutKey.hpp"
#include "../Entities/Switch.hpp"
#include "../Entities/Key.hpp"
static Camera* pCam = nullptr;
static SDL2pp::Texture *pTexture = nullptr;
static int bpX = 0;
static int bpY = 0;
static int lol = 320;
static int bpY2 = 0;
std::vector<std::pair<int*, int>> mapButtonOffsets{};

void MainMenu::Update(const float &deltaTime) {
    bpX = pCam->viewport.w / 2 - ((pTexture->GetWidth() * 4) / 2) + 110;
    bpY = pCam->viewport.h / 2 - ((pTexture->GetHeight() * 4) / 2) + 180;
    bpY2 = bpY + 60;

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

            std::pair<intptr_t,intptr_t> fd = {(intptr_t)&bpX, (intptr_t)yPos};
            auto play = new Button(lBuff.c_str(), aa, fd, &playClosed, 2);
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


    std::pair<intptr_t,intptr_t > fd = {(intptr_t)&bpX, (intptr_t)&bpY};
    std::pair<intptr_t,intptr_t> bg = {(intptr_t)&bpX, (intptr_t)&bpY2};

    auto play = new Button("Play", a, fd, &bbDe, 2);
    auto leveleditor = new Button("LevelEditor", b, bg, &bbDe, 2);

/*    auto b = new Button(a, {300,300,200,100}, &bbDe);
    b->SetTopTexture("Play", {0,0,0}, {255,255,255});
    b->SetBottomTexture(Engine::LoadTexture("assets/ui_button.png"), nullptr);

*/
}

void MainMenu::OnDestroy() {

}


static bool bRanOnce = false;
bool playedMapOnce = false;
//memory leak central
void MainMenu::OnGameEnd(bool bWon) {

    if(playedMapOnce)
    {
        bEndMenuClosed = false;

        static std::function<void(void)> lol = [&]() {

            bEndMenuClosed = true;

            Engine::Stop();
        };

            std::pair<intptr_t, intptr_t> pos = {(intptr_t)&bpX, (intptr_t)&bpY};

        std::pair<int,int> wtho = {300,300};
        auto nl = new Button("End", lol, pos,  &bEndMenuClosed, 2);

        return;
    }

    if(bRanOnce)
        return;
bRanOnce = true;


     static std::function<void(void)> fQuit = [](){
        GameManager::Instance()->bDestroy = true;
        Engine::Stop();
    //    bEndMenuClosed = true;
      //  bChoiceMade = true;
    };


    if(bWon){

        static std::function<void(void)> b = [&]() {
            bEndMenuClosed = true;
            //bChoiceMade = true;

            auto pLevel = GameManager::Instance()->pCurrentLevel;


            std::ifstream fileStream("alllevels");
            std::string lBuff;
            bool exists = false;
            int index = 0;

            bool nxtMap = false;
            std::string szNxtMap = "";
            while(std::getline(fileStream, lBuff))
            {
                if(nxtMap){
                    szNxtMap = lBuff;
                }

                if(std::string(lBuff + "/map") == pLevel->levelName)
                        nxtMap = true;

            }
            fileStream.close();


            if(szNxtMap != ""){
                pLevel->ChangeLevel(szNxtMap + "/map");
                playedMapOnce = true;
                //GameManager::Instance()->pCurrentLevel = Level::LoadLevel(szNxtMap + "/map");
                new Player;

            }else {
                Engine::Stop();
            }

        };

        std::pair<intptr_t, intptr_t> pos = {(intptr_t)&bpX, (intptr_t)&bpY};
        std::pair<intptr_t, intptr_t> pos1 = {(intptr_t)&bpX, (intptr_t)&bpY2};

        auto nl = new Button("Next Level", b, pos, &bEndMenuClosed, 2);
    //    auto dl = new Button("Quit", fQuit, pos1, &bEndMenuClosed, 2);


    }else{

        std::function<void(void)> b = [&](){
            bEndMenuClosed = true;

            //do shit
           // bChoiceMade = true;
        };

        std::pair<intptr_t, intptr_t> pos = {(intptr_t)&bpX, (intptr_t)&bpY};
        std::pair<intptr_t, intptr_t> pr = {(intptr_t)&bpX, (intptr_t)&bpY2};


        std::pair<int,int> wtho = {300,300};
        auto nl = new Button("Retry", b, pos,  &bEndMenuClosed, 2);
     //   auto dl = new Button("Quit", fQuit, pr, &bEndMenuClosed, 2);
    }

    bEndMenuClosed = false;
}
