#pragma once
#include <vector>
#include <string>
class Empty;
class Collider;
class Spike;
class Level {

public:
    std::vector<Empty*> worldTileList{};
    std::vector<Collider*> worldColliders{};
    std::vector<Spike*> worldSpikes{};

    static Level* LoadLevel(const std::string& szMapName);

private:

    void LoadColliders(const std::string& szMapName);
    void LoadDoor(const std::string& szMapName);
    void LoadKey(const std::string& szMapName);
    void LoadSpikes(const std::string& szMapName);
    void LoadSwitch(const std::string& szMapName);
    void LoadTutKeys(const std::string& szMapName);

};
