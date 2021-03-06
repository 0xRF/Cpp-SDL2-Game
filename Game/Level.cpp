#include "Level.hpp"
#include "../Engine/Engine.hpp"
#include <fstream>
#include <sstream>
#include "Components/ImageRenderer.hpp"
#include "../Engine/Entity.hpp"
#include <string>
#include "Components/Collider.hpp"
#include "Entities/Key.hpp"
#include "Entities/Spike.hpp"
#include "Entities/Door.hpp"
#include "Entities/Switch.hpp"
#include "Entities/TutKey.hpp"
#include <string>
#include <iostream>


Level* Level::LoadLevel(const std::string &szMap) {

    auto pLevel = new Level();
    pLevel->levelName = szMap;

    pLevel->LoadTiles(szMap);
    pLevel->LoadColliders(szMap);
    pLevel->LoadDoor(szMap);
    pLevel->LoadKey(szMap);
    pLevel->LoadSpikes(szMap);
    pLevel->LoadSwitch(szMap);
    pLevel->LoadTutKeys(szMap);


    pLevel->bGrabbedKey = false;
    std::cout << "Plz not crash  " << pLevel->worldColliders.size() << std::endl;

    return pLevel;

}

void Level::LoadColliders(const std::string &szMapDir) {

    std::string lBuff;
    std::ifstream fColliders(szMapDir + ".cdata");

    if(!fColliders)
    {
        return;
    }

    while(std::getline(fColliders, lBuff)) {
        std::stringstream line(lBuff);
        std::string buff;

        int data[4] = {0, 0, 0, 0};

        for (int i = 0; i < 4; i++) {
            line >> buff;
            data[i] = std::stoi(buff);
        }
        auto pEnt = new Empty();
        pEnt->position = {data[0], data[1]};
        worldColliders.push_back(pEnt->AddComponent<Collider>({data[2], data[3]}));

    }
    fColliders.close();
}

void Level::LoadDoor(const std::string &szMapDir) {

    std::string lBuff;
    std::ifstream fileStream(szMapDir + ".ddata");

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

        auto door = new Door();
        door->position = {x,y};

    }
    fileStream.close();
}

void Level::LoadKey(const std::string &szMapDir) {

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


        key = new Key();
        if(key)
        key->position = {x,y};

    }
    fileStream.close();

}

void Level::LoadSpikes(const std::string &szMapDir) {

    std::string lBuff;
    std::ifstream fileStream(szMapDir + ".sdata");

    if(!fileStream)
    {
        return;
    }

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

        pSpike->position = {x,y};
        worldSpikes.push_back(pSpike);

    }
    fileStream.close();
}

void Level::LoadSwitch(const std::string &szMapDir) {

    std::string lBuff;
    std::ifstream fileStream(szMapDir + ".zdata");

    if (!fileStream) {
        return;
    }


    if (std::getline(fileStream, lBuff)) {
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
        this->worldSwitches.push_back(str);
    }

    fileStream.close();

}

void Level::LoadTutKeys(const std::string &szMapDir) {


    std::string lBuff;
    std::ifstream fileStream(szMapDir + ".tdata");
    if(!fileStream)
        return;

    while(std::getline(fileStream, lBuff))
    {
        std::stringstream line(lBuff);
        std::string buff;

        char type = '5';
        int data[2] = {0,0};

        for (int i = 0; i < 3; i++) {
            line >> buff;
            if(i == 0){
                type = buff[0];
            }
            else
                data[i - 1] = std::stoi(buff);
        }

        int x = data[0];
        int y = data[1];

        std::cout << "Type " << type << std::endl;
        auto pKey = new TutKey(type);
        tutKeys.push_back(pKey);
        pKey->position.x = x;
        pKey->position.y = y;
    }
    fileStream.close();
}

void Level::ChangeLevel(const std::string &szNewLevel) {

    for (auto ent : worldTileList)
        ent->bEnabled = false;
    worldTileList.clear();

    for (auto spike : worldSpikes)
        spike->bEnabled = false;
    worldSpikes.clear();

    for (auto pSwi : worldSwitches)
        pSwi->bEnabled = false;
    worldSwitches.clear();

    for (auto tkey : tutKeys)
        tkey->bEnabled = false;
    tutKeys.clear();

    for (auto col : worldColliders)
        col->entity->bEnabled = false;
    worldColliders.clear();

    key->bEnabled = false;

    bGrabbedKey = false;

    levelName = szNewLevel;
    LoadTiles(szNewLevel);
    LoadColliders(szNewLevel);
   LoadDoor(szNewLevel);
   LoadKey(szNewLevel);
   LoadSpikes(szNewLevel);
   LoadSwitch(szNewLevel);
   LoadTutKeys(szNewLevel);

}

void Level::LoadTiles(const std::string &szMap) {
    std::ifstream fileStream(szMap + ".mdata");
    std::string lBuff;

    if(!fileStream)
    {
        return;
    }

    while(std::getline(fileStream, lBuff)) {
        std::stringstream line(lBuff);
        std::string buff;

        int data[4] = {0, 0, 0, 0};

        for (int i = 0; i < 4; i++) {
            line >> buff;
            data[i] = std::stoi(buff);
        }

        int x = data[0];
        int y = data[1];

        int sx = data[2];
        int sy = data[3];

        auto pEnt = new Empty();
        pEnt->position = Vector2(x,y);
        pEnt->scale = 2.0f;
        auto ir = pEnt->AddComponent(ImageRenderer(Engine::LoadTexture("assets/duntiles.png"), {sx, sy, 32, 32}));
        worldTileList.push_back(pEnt);
    }
}
