#pragma once

#include "../../Engine/Entity.hpp"

class Key : public Entity<Key>{

public:
    void OnDestroy() override;

    void Start() override;
};
