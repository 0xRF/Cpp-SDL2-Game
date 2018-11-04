#pragma once

#include "../EditorMode.hpp"
#include "../LevelEditor.hpp"

class LevelEditor;
class TutKey;

class CustomImagePlacer : public EditorMode {

public:
    void OnUpdate() override;

    void OnRender() override;

    void OnActivate() override;

    void OnDisable() override;

    void OnSave(const std::string& szMapDir)override;

    CustomImagePlacer(LevelEditor* _pEditor);


    void OnLoad(const std::string& szMapDir) override;

    void ForceRender() override;

private:
    LevelEditor* pEditor = nullptr;


    void PlaceKey();

};

