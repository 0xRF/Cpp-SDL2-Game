#pragma once

#include "../../Engine/Entity.hpp"

class Spike : public Entity<Spike>{

public:
    void OnDestroy() override;

    void Start() override;

};


