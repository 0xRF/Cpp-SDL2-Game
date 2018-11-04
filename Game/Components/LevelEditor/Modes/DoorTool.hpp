#pragma once


#include "../EditorMode.hpp"
#include <SDL2pp/Rect.hh>
class LevelEditor;
class Door;

class DoorTool : public EditorMode{

public:
    void OnUpdate() override;

    void OnRender() override;

    void OnActivate() override;

    void OnDisable() override;

    void OnSave(const std::string& szMapDir) override;

    void OnLoad(const std::string& szMapDir) override;


    DoorTool(LevelEditor* _pEditor);

    void ForceRender() override;

private:
    LevelEditor* pEditor = nullptr;

    void Place();


};

