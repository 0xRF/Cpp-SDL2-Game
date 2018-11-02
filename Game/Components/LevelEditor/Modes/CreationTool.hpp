#pragma once


#include <SDL2pp/Rect.hh>
#include "../EditorMode.hpp"

class LevelEditor;
class Empty;
class CreationTool : public EditorMode {

public:
    void OnUpdate();
    void OnRender();
    void OnDisable();
    void OnActivate();
    void OnSave(const std::string& szMapDir) override;

    void OnLoad(const std::string& szMapDir) override;

    CreationTool(LevelEditor* _pEditor) : pEditor(_pEditor){ }

    void ForceRender() override;

private:
    LevelEditor* pEditor = nullptr;

    Empty* GetEntity(std::pair<int, int> pos);
};
