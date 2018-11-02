#pragma once

#include <SDL2pp/Rect.hh>
#include "../../Engine/Component.hpp"
#include "../../Engine/Vector2.hpp"

class ImageRenderer;

class Collider : public Component<Collider>{
public:

    Collider(ImageRenderer* rend);

    Collider(SDL2pp::Rect rect);

    Collider(const int& w, const int& h);

    void Update(const float &deltaTime) override;

    void Render() override;

    void Start() override;

    void OnDestroy() override;


    const SDL2pp::Rect GetBounds() const;

    bool Collides(const SDL2pp::Rect& other);
    bool Collides(const int& x, const int& y);

    bool Collides(const SDL2pp::Rect& other, const Vector2& velocity);
    bool Collides(const int& x, const int& y, const Vector2& velocity);
    bool Collides(const float& x, const float& y, const Vector2& velocity);
    bool Collides(const Vector2& other, const Vector2& velocity);


private:
        int width = 0;
        int height = 0;
        std::pair<int,int> offsets = {0,0};

    bool bdone = false;
};
