#pragma once

#include "../EditorMode.hpp"
#include "../LevelEditor.hpp"

class LevelEditor;

class SpikeTool : public EditorMode {

public:
    void OnUpdate() override;

    void OnRender() override;

    void OnActivate() override;

    void OnDisable() override;

    void OnSave(const std::string& szMapDir) override;

    SpikeTool(LevelEditor* _pEditor);

    void ForceRender() override;

    void OnLoad(const std::string& szMapDir) override;

private:
    LevelEditor* pEditor = nullptr;

    Spike* GetSpike(const int& x, const int& y);

};

