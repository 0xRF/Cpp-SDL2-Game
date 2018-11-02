#include "../../Engine/Component.hpp"
#include <SDL2pp/SDL2pp.hh>
#include "ImageRenderer.hpp"

class AnimatedRenderer : public Component<AnimatedRenderer> {

public:
    AnimatedRenderer(ImageRenderer* pRend,  const int& frames, const int& speed);
    void Update(const float &deltaTime) override;
    void Render() override;
    void Start() override;
    void OnDestroy() override;

    int nframes = 0;
    int animSpeed = 0.0f;

    std::pair<int,int> nOffset = {0,0};

    void AdjustAnimSpeed(const int& speed);
    void AdjustSpritePosition(const int& y);

    ImageRenderer* renderer = nullptr;
};

