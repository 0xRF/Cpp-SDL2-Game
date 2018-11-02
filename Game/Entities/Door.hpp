#pragma once

#include "../../Engine/Entity.hpp"

class Door : public Entity<Door>{

public:
    void OnDestroy() override;

    void Start() override;
};
