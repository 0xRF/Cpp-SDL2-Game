#pragma once

#include "../EditorMode.hpp"
#include "../LevelEditor.hpp"

class LevelEditor;
class Empty;
class Switch;

class SwitchTool : public EditorMode {

public:
    void OnUpdate() override;

    void OnRender() override;

    void OnActivate() override;

    void OnDisable() override;

    void OnSave(const std::string& szMapDir) override;

    SwitchTool(LevelEditor* _pEditor);


    void OnLoad(const std::string& szMapDir) override;

    void ForceRender() override;

private:
    LevelEditor* pEditor = nullptr;

    std::vector<Switch*> allSwitches{};

    Switch* activeSwitch = nullptr;

    bool bEditSelection = false;

};

