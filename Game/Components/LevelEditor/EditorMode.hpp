#pragma once

#include <string>

class EditorMode {

public:
    virtual void OnUpdate() = 0;
    virtual void OnRender() = 0;
    virtual void ForceRender() = 0;
    virtual void OnActivate() = 0;
    virtual void OnDisable() = 0;

    virtual void OnSave(const std::string& szMapDir) = 0;

    virtual void OnLoad(const std::string& szMapDir) = 0;

    std::string modeName{};


};


