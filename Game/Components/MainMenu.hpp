#pragma once

#include "../../Engine/Component.hpp"



class MainMenu :  public Component<MainMenu>{

public:
    void Update(const float &deltaTime) override;

    void Render() override;

    void Start() override;

    void OnDestroy() override;


    bool bEndMenuClosed = true;

    void OnGameEnd(bool bWon);

};

