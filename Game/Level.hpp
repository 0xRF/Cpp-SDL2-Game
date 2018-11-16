#pragma once
#include <vector>
#include <string>
class Empty;
class Collider;
class Spike;
class Key;
class TutKey;
class Switch;
class Level {

public:
    std::vector<Empty*> worldTileList{};
    std::vector<Collider*> worldColliders{};
    std::vector<Spike*> worldSpikes{};
    std::vector<TutKey*> tutKeys{};
    std::vector<Switch*> worldSwitches{};

    Key* key = nullptr;

    static Level* LoadLevel(const std::string& szMapName);
    std::string levelName = "";

    void ChangeLevel(const std::string& szNewLevel);

    bool bGrabbedKey = false;

private:


    void LoadColliders(const std::string& szMapName);
    void LoadDoor(const std::string& szMapName);
    void LoadKey(const std::string& szMapName);
    void LoadSpikes(const std::string& szMapName);
    void LoadSwitch(const std::string& szMapName);
    void LoadTutKeys(const std::string& szMapName);

    void LoadTiles(const std::string &basic_string);
};
