#pragma once


#include <SDL2pp/Rect.hh>
#include "../EditorMode.hpp"
#include <SDL2pp/SDL2pp.hh>
class LevelEditor;

class PickerTool : public EditorMode {

public:
    void OnUpdate() override ;
    void OnRender() override ;
    void OnDisable() override ;
    void OnActivate() override ;
    void OnSave(const std::string& szMapDir) override;

    void OnLoad(const std::string& szMapDir) override;

    PickerTool(LevelEditor* _pEditor);

    void ForceRender() override;

private:

    LevelEditor* pEditor = nullptr;
    SDL2pp::Rect selrect = {0,0,0,0};

    SDL2pp::Texture* pSnapShot = nullptr;

    SDL2pp::Rect boundries = {0,0,0,0};
    void Blurr();

    void GrabTiles();
};
