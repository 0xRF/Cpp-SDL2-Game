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


Level* Level::LoadLevel(const std::string &szMapName) {

    auto pLevel = new Level();

    char buffer[256];
    strncpy(buffer, szMapName.c_str(), sizeof(buffer));
    strncat(buffer, ".mdata", sizeof(buffer));

    std::ifstream fileStream(buffer);
    std::string lBuff;

    if(!fileStream)
    {
        return nullptr;
    }

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
        pLevel->worldTileList.push_back(pEnt);
    }


    pLevel->LoadColliders(szMapName);
    pLevel->LoadDoor(szMapName);
    pLevel->LoadKey(szMapName);
    pLevel->LoadSpikes(szMapName);
    pLevel->LoadSwitch(szMapName);
    pLevel->LoadTutKeys(szMapName);

    return pLevel;

}

void Level::LoadColliders(const std::string &szMapName) {
    char buffer[256];
    strncpy(buffer, szMapName.c_str(), sizeof(buffer));
    strncat(buffer, ".cdata", sizeof(buffer));

    std::string lBuff;
    std::ifstream fColliders(buffer);

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

        auto pEnt = new Empty({(float) data[0], (float) data[1]});
       worldColliders.push_back(pEnt->AddComponent<Collider>({data[2], data[3]}));

    }

    fColliders.close();
}

void Level::LoadDoor(const std::string &szMapName) {
    char buffer[256];
    strncpy(buffer, szMapName.c_str(), sizeof(buffer));
    strncat(buffer, ".ddata", sizeof(buffer));

    std::string lBuff;
    std::ifstream fileStream(buffer);

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

void Level::LoadKey(const std::string &szMapName) {

    char buffer[256];
    strncpy(buffer, szMapName.c_str(), sizeof(buffer));
    strncat(buffer, ".kdata", sizeof(buffer));

    std::string lBuff;
    std::ifstream fileStream(buffer);

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


        auto key = new Key();
        key->position = {x,y};

    }
    fileStream.close();

}

void Level::LoadSpikes(const std::string &szMapName) {

    char buffer[256];
    strncpy(buffer, szMapName.c_str(), sizeof(buffer));
    strncat(buffer, ".sdata", sizeof(buffer));

    std::string lBuff;
    std::ifstream fileStream(buffer);

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

void Level::LoadSwitch(const std::string &szMapName) {

    std::string lBuff;
    std::ifstream fileStream(szMapName + ".zdata");

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
    }

    fileStream.close();

}

void Level::LoadTutKeys(const std::string &szMapName) {
    char buffer[256];
    strncpy(buffer, szMapName.c_str(), sizeof(buffer));
    strncat(buffer, ".tdata", sizeof(buffer));

    std::string lBuff;
    std::ifstream fileStream(buffer);
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

        pKey->position.x = x;
        pKey->position.y = y;
    }
    fileStream.close();
}
