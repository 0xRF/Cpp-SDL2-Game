#pragma once


#include "../EditorMode.hpp"

class LevelEditor;
class Key;

class KeyTool : public  EditorMode {

public:
    void OnUpdate() override;

    void OnRender() override;

    void OnActivate() override;

    void OnDisable() override;

    void OnSave(const std::string& szMapDir) override;

    void OnLoad(const std::string& szMapDir)  override;

    KeyTool(LevelEditor* _pEditor);

    void ForceRender() override;

private:
    LevelEditor* pEditor = nullptr;
private: Key* pKey = nullptr;

};
