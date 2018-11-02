#pragma once


#include "../../Engine/Component.hpp"
#include <SDL2pp/SDL2pp.hh>

class ImageRenderer : public Component<ImageRenderer>
{
public:
    ImageRenderer(SDL2pp::Texture* texture);
    ImageRenderer(SDL2pp::Texture* texture, const SDL2pp::Rect& srcrect);

    ImageRenderer(const char* szFileName);
    ImageRenderer(const char* szFileName, const SDL2pp::Rect& srcrect);

    void Update(const float &deltaTime) override;
    void Start() override;
    void OnDestroy() override;
    void Render() override;

    SDL2pp::Texture* pTexture = nullptr;
    SDL2pp::Rect srcrect = {0,0,0,0};
};
