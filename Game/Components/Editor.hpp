#pragma once

#include "../../Engine/Component.hpp"
#include <SDL2pp/SDL2pp.hh>
#include <vector>
#include "../../Engine/Vector2.hpp"

class Empty;


enum EMode : int{
    Select = 0,
    Delete = 1,
    Place = 2,
    Colliders = 3,
    Doors = 4,
    Spikes = 5
};

class Editor : public Component<Editor> {

public:

    void Update(const float &deltaTime) override;

    void Render() override;

    void Start() override;

    void OnDestroy() override;

    void SaveMap(const char* szFileName);

  void LoadMap(const char* szFileName);

    std::vector<SDL2pp::Rect> colliderList = std::vector<SDL2pp::Rect>();
    void ForceUpdate() override;
private:

    void LoadDoors(const char* szMapName);
    void LoadColliders(const char* szMapName);
    void LoadTiles(const char* szMapName);

    void SaveDoors(const char* szMapName);
    void SaveTiles(const char* szMapName);
    void SaveColliders(const char* szMapName);

    void PlaceTile(std::pair<int,int> mPos);
    void RemoveTile(Empty* pEntity);
    bool bTileExist(std::pair<int,int>& pr);

    void OnColliderUpdate(std::pair<int,int>& pos);
    void OnSelectUpdate(std::pair<int,int>& pos);
    void OnPlaceUpdate(std::pair<int,int>& pos);
    void OnRemoveUpdate(std::pair<int,int>& pos);
    void OnDoorUpdate(std::pair<int, int>& pos);
    void OnSpikeUpdate(std::pair<int,int>& pos);

    void OnColliderRender(std::pair<int,int>& pos);
    void OnSelectRender(std::pair<int,int>& pos);
    void OnPlaceRender(std::pair<int,int>& pos);
    void OnRemoveRender(std::pair<int,int>& pos);
    void OnDoorRender(std::pair<int,int>& pos);
    void OnSpikeRender(std::pair<int,int>& pos);

    void UpdateCamera(const float& deltaTime);

    Empty* GetEntity(const int& x, const int& y);

    std::vector<Empty*> tileList = std::vector<Empty*>();

    std::pair<int, int> exitDoorPosition = {0,0};

    bool bTileMapOpen = false;
    SDL2pp::Texture* pSpriteMap = nullptr;


    struct TileData{
        std::pair<int,int> position = {0,0};
        std::pair<int,int> spritePosition = {0,0};
    };

    std::vector<TileData> copyBuffer = std::vector<TileData>();


    SDL2pp::Rect colRect = {0,0,0,0};
    SDL2pp::Rect selRect = {0,0,0,0};
    SDL2pp::Rect delRect = {0,0,0,0};
    EMode mode = EMode::Select;

    void FixRect(SDL2pp::Rect& rect);

};
