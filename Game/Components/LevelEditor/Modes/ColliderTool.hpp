#pragma once

#include "../EditorMode.hpp"
#include "../LevelEditor.hpp"

class LevelEditor;

class ColliderTool : public EditorMode {

public:
    void OnUpdate() override;

    void OnRender() override;

    void OnActivate() override;

    void OnDisable() override;

    void OnSave(const std::string& szMapDir) override;

    ColliderTool(LevelEditor* _pEditor);


    void OnLoad(const std::string& szMapDir) override;

    void ForceRender() override;

private:
    LevelEditor* pEditor = nullptr;

    SDL2pp::Rect selrect = {0,0,0,0};

    void PlaceCollider();

};

