#pragma once


#include "../EditorMode.hpp"
#include <SDL2pp/Rect.hh>
class LevelEditor;

class EraserTool : public EditorMode{

public:
    void OnUpdate() override;

    void OnRender() override;

    void OnActivate() override;

    void OnDisable() override;

    void OnSave(const std::string& szMapDir) override;

    void OnLoad(const std::string& szMapDir) override;


    EraserTool(LevelEditor* _pEditor);

    void ForceRender() override;

private:
    LevelEditor* pEditor = nullptr;

    SDL2pp::Rect selrect = {0,0,0,0};

    void Delete();

};

