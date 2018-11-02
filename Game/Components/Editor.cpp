//
// Created by xrf on 27/07/18.
//

#include "Editor.hpp"
#include "../../Engine/InputManager.hpp"
#include "../../Engine/Entity.hpp"
#include "ImageRenderer.hpp"
#include "Collider.hpp"
#include "../../Engine/Vector2.hpp"
#include "../../Engine/Engine.hpp"
#include "../../Engine/UI.hpp"
#include <fstream>
#include <sstream>
#include "../../Engine/Camera.hpp"
#include <sys/stat.h>
#include "../../Tools.hpp"
#include "../Entities/Player.hpp"
#include "PlayerController.hpp"

const char* modes[] {"Select", "Delete", "Place", "Colliders", "Door", "Keys"};

static Player* pPlayer = nullptr;

void Editor::PlaceTile(std::pair<int,int> mPos) {


    mPos = SnapToGrid(mPos, 64);

    auto wPos = SnapToGrid(Camera::Instance().ScreenToWorldPoint(mPos), 64);

    for(auto tile : copyBuffer){


        Vector2 pos = SnapToGrid(Camera::Instance().ScreenToWorldPoint(tile.position.first - selRect.x + mPos.first, tile.position.second - selRect.y + mPos.second), 64);

        auto ent = GetEntity((int)pos.x,(int)pos.y);

     //   if(tile.spritePosition.first == 0 && tile.spritePosition.second == 0)
       //     continue;

        if(ent)
            RemoveTile(ent);

        auto pEnt = new Empty(pos);
        pEnt->scale = 2.0f;
        auto ir = pEnt->AddComponent(ImageRenderer(pSpriteMap, {tile.spritePosition.first/2, tile.spritePosition.second/2, 32, 32}));
       //d pEnt->AddComponent(Collider(ir));
        tileList.push_back(pEnt);
    }

}

void Editor::RemoveTile(Empty* pEntity) {

    auto it = tileList.begin();

    while(it != tileList.end()){

        if((*it) == pEntity)
        {
            (*it)->bDestroy = true;
            it = tileList.erase(it);
        }
        else
        ++it;
    }
}

Empty *Editor::GetEntity(const int &x, const int &y) {
    for (auto it = tileList.begin(); it != tileList.end(); ++it) {

        if ((*it)->position.x == x && (*it)->position.y == y) {
            return *it;
        }
    }
    return nullptr;
}

bool Editor::bTileExist(std::pair<int,int>& pr) {
    return GetEntity(pr.first, pr.second) != nullptr;
}



void Editor::Start() {

   pPlayer = (Player*)Engine::FindObjectOfType(Entity<Player>::EID());


    bEnabled = false;
    LoadMap("uLevel");
    pSpriteMap = Engine::LoadTexture("assets/duntiles.png");

  //  UI::RegisterButton("Yeet", nullptr, {255,0,0}, pSpriteMap, 300,300, 100,100, nullptr);
}

void Editor::OnDestroy() {

}

void Editor::ForceUpdate(){

    if(InputManager::KeyPressed(SDLK_ESCAPE))
    {
            bEnabled = !bEnabled;
           pPlayer->GetComponent<PlayerController>()->bEnabled = !pPlayer->GetComponent<PlayerController>()->bEnabled;

    }

   // for(auto col : colliderList)
 //   UI::DrawRect(Camera::Instance().WorldToScreenPoint(col.x, col.y), col.w, col.h, {255,255,0}, 2);
}

void Editor::Update(const float &deltaTime) {

    auto mPos = InputManager::GetMousePos();

   /*/ if (InputManager::KeyPressed(SDLK_TAB)) {
        if ((int) mode == 4)
            mode = (EMode) 0;
        else
            mode = static_cast<EMode>((static_cast<int>(mode )+ 1));
    }
*/
    if (InputManager::KeyPressed(SDLK_o))
        SaveMap("uLevel");

    switch (mode) {

        case EMode::Select:
            OnSelectUpdate(mPos);
            break;

        case EMode::Delete:
            OnRemoveUpdate(mPos);
            break;

        case EMode::Colliders:
            OnColliderUpdate(mPos);
            break;

        case EMode::Place:
            OnPlaceUpdate(mPos);
            break;

        case EMode ::Doors:
            OnDoorUpdate(mPos);
            break;
    }

    UpdateCamera(deltaTime);
}

void Editor::OnColliderUpdate(std::pair<int, int> &pos) {

    pos = SnapToGrid(pos,64);

    auto wPos = SnapToGrid(Camera::Instance().ScreenToWorldPoint({pos.first, pos.second}),64);

    if (InputManager::MouseHeld(SDL_BUTTON_LEFT)) {

        if (colRect.x == 0 && colRect.y == 0 && colRect.w == 0 && colRect.h == 0) {
            colRect.x =  (int)wPos.x;
            colRect.y =  (int)wPos.y;
        } else {
            colRect.w = SnapToGrid((int)wPos.x - colRect.x,64);
            colRect.h = SnapToGrid((int)wPos.y - colRect.y,64);
        }
    } else if (InputManager::MouseClicked(SDL_BUTTON_LEFT)) {


        FixRect(colRect);

        if(colRect.w != 0 && colRect.h != 0) {
            colliderList.push_back(colRect);
        }
        colRect = {0, 0, 0, 0};
    }else if(InputManager::MouseClicked(SDL_BUTTON_RIGHT)){

        auto it = colliderList.begin();
        while(colliderList.end() != it){

            if((*it).Contains((int)wPos.x, (int)wPos.y))
                it = colliderList.erase(it);
            else ++it;
        }


    }
}

void Editor::OnSelectUpdate(std::pair<int, int> &pos) {

    pos = SnapToGrid(pos,64);

    if (InputManager::KeyHeld(SDLK_SPACE)) {

        bTileMapOpen = true;

      //  selRect.x = std::clamp(selRect.x, 0, pSpriteMap->GetWidth() * 2);

        if (selRect.x > pSpriteMap->GetWidth() * 2) {
            selRect.x = pSpriteMap->GetWidth() * 2;
            selRect.h = 0;
        }
        if (selRect.x + selRect.w >= pSpriteMap->GetWidth() * 2)
            selRect.w = pSpriteMap->GetWidth() * 2 - selRect.x;


        if (selRect.y > pSpriteMap->GetHeight() * 2) {
            selRect.h = 0;
            selRect.y = pSpriteMap->GetHeight() * 2;
        }

        if (selRect.y + selRect.h >= pSpriteMap->GetHeight() * 2) {
            selRect.h = pSpriteMap->GetHeight() * 2 - selRect.y;
        }

    } else {
        if (bTileMapOpen)
            mode = EMode::Place;

        bTileMapOpen = false;
    }

    if (InputManager::MouseHeld(SDL_BUTTON_LEFT)) {

        auto wPos = SnapToGrid(Camera::Instance().ScreenToWorldPoint(pos),64);

        if (selRect.x == 0 && selRect.y == 0 && selRect.w == 0 && selRect.h == 0) {
            selRect.x = (int)wPos.x;
            selRect.y = (int)wPos.y;
        } else {
            selRect.w = (int)wPos.x- selRect.x;
            selRect.h = (int)wPos.y - selRect.y;
        }

    }
    else if (InputManager::MouseClicked(SDL_BUTTON_LEFT)) {

        copyBuffer.clear();

        if (selRect.w < 0) {
            int w = abs(selRect.w);
            selRect.x -= w;
            selRect.w = w;
        }

        if (selRect.h < 0) {
            int h = abs(selRect.h);
            selRect.y -= h;
            selRect.h = h;
        }

        if (bTileMapOpen) {
            for (int i = selRect.x; i < selRect.x + selRect.w; i += 64) {
                for (int j = selRect.y; j < selRect.y + selRect.h; j += 64) {

                    if (bTileMapOpen) {
                        copyBuffer.push_back({{i, j},
                                              {i, j}});
                    }
                }
            }
        } else {

            for (int i = selRect.x; i < selRect.x + selRect.w; i += 64) {
                for (int j = selRect.y; j < selRect.y + selRect.h; j += 64) {

                    auto ent = GetEntity(i, j);
                    if(!ent)
                        continue;

                    if (ent->ID() != Empty::EID())
                        continue;

                    if (!ent->GetComponent<ImageRenderer>())
                        continue;

                    auto rct = ent->GetComponent<ImageRenderer>()->srcrect;

                    copyBuffer.push_back({{i, j},
                                          {rct.x*2, rct.y*2}});
                }
            }

        }
      //  mode = EMode::Place;
        selRect = {0,0,0,0};
    }
}

void Editor::OnPlaceUpdate(std::pair<int, int> &pos) {
    if (InputManager::MouseClicked(SDL_BUTTON_LEFT)) {
            PlaceTile(pos);
    }else if(InputManager::MouseDown(SDLK_SPACE))
        mode = EMode::Select;
}

void Editor::OnRemoveUpdate(std::pair<int, int> &pos) {

    pos = SnapToGrid(pos,64);

    auto wPos = SnapToGrid(Camera::Instance().ScreenToWorldPoint(pos),64);

    if (InputManager::MouseHeld(SDL_BUTTON_LEFT)) {

        if (delRect.x == 0 && delRect.y == 0 && delRect.w == 0 && delRect.h == 0) {
            delRect.x = (int)(wPos.x);
            delRect.y = (int)(wPos.y);
        } else {
            delRect.w = SnapToGrid((int)(wPos.x) - delRect.x, 64);
            delRect.h = SnapToGrid((int)(wPos.y) - delRect.y, 64);
        }
    } else if (InputManager::MouseClicked(SDL_BUTTON_LEFT)) {

        FixRect(delRect);

        for (int i = delRect.x; i < delRect.x + delRect.w; i += 64) {
            for (int j = delRect.y; j < delRect.y + delRect.h; j += 64) {
                auto ent = GetEntity(i, j);

                if (ent) {
                    RemoveTile(ent);
                }
            }
        }
        delRect = {0, 0, 0, 0};

    }
}

void Editor::OnDoorUpdate(std::pair<int, int> &pos) {
    pos = SnapToGrid(pos,64);

    auto worldPos = SnapToGrid(Camera::Instance().ScreenToWorldPoint(pos),64);

    if(InputManager::MouseClicked(SDL_BUTTON_LEFT))
    {
        //PPlace door
        exitDoorPosition.first = (int)worldPos.x;
        exitDoorPosition.second = (int)worldPos.y;

    }
    else if(InputManager::MouseClicked(SDL_BUTTON_RIGHT)){
        exitDoorPosition = {0,0};
    }

}

void Editor::UpdateCamera(const float &deltaTime) {


    static float offsetx = 0;
    static float offsety = 0;

    if(InputManager::KeyHeld(SDLK_RIGHT))
        offsetx+= 128 * deltaTime;

    if(InputManager::KeyHeld(SDLK_LEFT))
        offsetx-= 128 * deltaTime;

    if(InputManager::KeyHeld(SDLK_UP))
        offsety-=128 * deltaTime;

    if(InputManager::KeyHeld(SDLK_DOWN))
        offsety+=128 * deltaTime;

    Camera::Instance().viewport.x = static_cast<int>(offsetx/64) * 64;
    Camera::Instance().viewport.y = static_cast<int>(offsety/64) * 64;

}

void Editor::Render() {

    if(!bEnabled)
        return;

    UI::DrawString(modes[(int)mode], 1280/2 - 20, 20, 3, {0,255,0});

    auto mPos = InputManager::GetMousePos();


    switch (mode) {

        case EMode::Select:
            OnSelectRender(mPos);
            break;

        case EMode::Delete:
            OnRemoveRender(mPos);
            break;

        case EMode::Colliders:
            OnColliderRender(mPos);
            break;

        case EMode::Place:
            OnPlaceRender(mPos);
            break;

        case EMode ::Doors:
            OnDoorRender(mPos);
            break;
    }

    auto sp = (Camera::Instance().WorldToScreenPoint(exitDoorPosition));

    if(exitDoorPosition.first != 0 && exitDoorPosition.second != 0)
        UI::DrawTexture(pSpriteMap, {0,0, 32,64}, sp.first, sp.second, 2.0f);

    UI::DrawString("TL", 1230, 700, 2, {255,0,0});
    UI::DrawString("CP", 1230, 690, 2, {255,0,0});
    UI::DrawString(std::to_string(copyBuffer.size()).c_str(), 1250, 700, 2, {255,0,0});
    UI::DrawString(std::to_string(Engine::Instance().entityList.size()).c_str(), 1250, 690, 2, {255,0,0});

}

void Editor::OnColliderRender(std::pair<int, int> &pos) {

    auto sp = Camera::Instance().WorldToScreenPoint(colRect.x, colRect.y);

    UI::DrawRect({sp.first, sp.second, colRect.w, colRect.h}, {0,255,0}, 2);

    auto wPos = Camera::Instance().ScreenToWorldPoint(pos);

    for(auto col : colliderList){

       auto sp = (Camera::Instance().WorldToScreenPoint(col.x, col.y));

        if(col.Contains((int)wPos.x, (int)wPos.y))
            UI::DrawRect({sp.first, sp.second, col.w, col.h}, {255,0,0}, 2);
        else
            UI::DrawRect({sp.first, sp.second, col.w, col.h}, {0,0,255}, 2);
    }
}

void Editor::OnSelectRender(std::pair<int, int> &pos) {

    auto sp = Camera::Instance().WorldToScreenPoint(selRect.x, selRect.y);

    UI::DrawRect({sp.first, sp.second, selRect.w, selRect.h}, {255,0,0}, 2);

    if(bTileMapOpen) {
        UI::DrawTexture(pSpriteMap, 0, 0, 2.0f);
        //  if(selRect.w != 0 && selRect.h != 0)

    }

}

void Editor::OnPlaceRender(std::pair<int, int> &pos) {

    pos = SnapToGrid(pos,64);

    for(auto tile : copyBuffer){
        UI::DrawTexture(pSpriteMap, {tile.spritePosition.first/2,tile.spritePosition.second/2, 32,32}, pos.first + tile.position.first ,pos.second + tile.position.second, 2.0f);
    }

}

void Editor::OnRemoveRender(std::pair<int, int> &pos) {

    auto sp = Camera::Instance().WorldToScreenPoint(delRect.x, delRect.y);

    UI::DrawRect({sp.first, sp.second, delRect.w, delRect.h}, {255,0,0}, 2);

}

void Editor::OnDoorRender(std::pair<int, int> &pos) {
    pos = SnapToGrid(pos,64);

    auto sp = (Camera::Instance().WorldToScreenPoint(exitDoorPosition));

    UI::DrawRect({pos.first, pos.second + 32, 64, 96}, {0,255,0}, 2);
}

void Editor::SaveMap(const char *szTileMap) {

    SaveTiles(szTileMap);
    SaveColliders(szTileMap);
    SaveDoors(szTileMap);
}

void Editor::LoadMap(const char *szFileName) {
    std::cout << "Started loading map" << std::endl;

    LoadTiles(szFileName);
    LoadColliders(szFileName);
    LoadDoors(szFileName);

}

void Editor::LoadDoors(const char *szMapName) {

        char buffer[256];
        strncpy(buffer, szMapName, sizeof(buffer));
        strncat(buffer, ".ddata", sizeof(buffer));

        std::string lBuff;
        std::ifstream fileStream(buffer);

        std::getline(fileStream, lBuff);

        std::stringstream line(lBuff);
        std::string buff;

        int data[2] = {0,0};

        for (int i = 0; i < 4; i++) {
            line >> buff;
            if(line)
            data[i] = std::stoi(buff);
        }

        exitDoorPosition = {data[0], data[1]};

       fileStream.close();
}

void Editor::LoadColliders(const char *szMapName) {

    char buffer[256];
    strncpy(buffer, szMapName, sizeof(buffer));
    strncat(buffer, ".cdata", sizeof(buffer));

    std::string lBuff;
    std::ifstream fileStream(buffer);
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

        colliderList.push_back({x,y,w,h});
    }
    fileStream.close();
}

void Editor::LoadTiles(const char *szMapName) {

    char buffer[256];
    strncpy(buffer, szMapName, sizeof(buffer));
    strncat(buffer, ".mdata", sizeof(buffer));

    std::ifstream fileStream(buffer);
    std::string lBuff;
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

        int sx = data[2];
        int sy = data[3];

        auto pEnt = new Empty({(float)x,(float)y});
        pEnt->scale = 2.0f;
        auto ir = pEnt->AddComponent(ImageRenderer(Engine::LoadTexture("assets/duntiles.png"), {sx, sy, 32,32}));
        tileList.push_back(pEnt);
    }

    fileStream.close();
    std::cout << "Finished loading map: " <<  std::endl;
}

void Editor::SaveDoors(const char *szMapName) {

    char buffer[256];
    strncpy(buffer, szMapName, sizeof(buffer));
    strncat(buffer, ".ddata", sizeof(buffer));

    std::ofstream fileStream(buffer,  std::ofstream::out);
    fileStream << exitDoorPosition.first << ' ' << exitDoorPosition.second << '\n';

    fileStream.close();
    std::cout << "Finished Saving Door: " << colliderList.size() << std::endl;

}

void Editor::SaveTiles(const char *szMapName) {

    char buffer[256];
    strncpy(buffer, szMapName, sizeof(buffer));
    strncat(buffer, ".mdata", sizeof(buffer) - 6*sizeof(char));

    std::ofstream fSaveMap(buffer,  std::ofstream::out);

    for(Empty* t : tileList) {

        int x = t->GetComponent<ImageRenderer>()->srcrect.x;
        int y = t->GetComponent<ImageRenderer>()->srcrect.y;

        fSaveMap << (int)ceil(t->position.x) << ' ' << (int)ceil(t->position.y) << ' ' << x << ' ' << y << '\n';
    }
    std::cout << "Finished Saving Map: " << tileList.size() << std::endl;
    fSaveMap.close();
}

void Editor::SaveColliders(const char *szMapName) {

    char buffer[256];
    strncpy(buffer, szMapName, sizeof(buffer));
    strncat(buffer, ".cdata", sizeof(buffer));

    std::ofstream fileStream(buffer,  std::ofstream::out);
    for(auto col : colliderList) {
        fileStream << col.x << ' ' << col.y << ' ' << col.w << ' ' << col.h << '\n';
    }
    fileStream.close();
    std::cout << "Finished Saving Colliders: " << colliderList.size() << std::endl;
}

void Editor::FixRect(SDL2pp::Rect &rect) {
    if (rect.w < 0) {
        int w = abs(rect.w);
        rect.x -= w;
        rect.w = w;
    }
    if (rect.h < 0) {
        int h = abs(rect.h);
        rect.y -= h;
        rect.h = h;
    }
}
