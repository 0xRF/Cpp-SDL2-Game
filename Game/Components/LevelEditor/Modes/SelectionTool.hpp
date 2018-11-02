#pragma once

#include <SDL2pp/Rect.hh>
#include "../EditorMode.hpp"

class LevelEditor;
class Empty;
class SelectionTool : public EditorMode {

public:
  void OnUpdate();
  void OnRender();
  void OnDisable();
  void OnActivate();

    void OnSave(const std::string& szMapDir) override;

    void GrabTiles();

    void OnLoad(const std::string& szMapDir) override;

    SelectionTool(LevelEditor* _pEditor) : pEditor(_pEditor){ }

private:

    LevelEditor* pEditor = nullptr;
    SDL2pp::Rect selrect = {0,0,0,0};

    Empty* GetEntity(std::pair<int,int> pos);

public:
    void ForceRender() override;
};
