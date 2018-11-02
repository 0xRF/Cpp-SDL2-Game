//
// Created by rowan fimmano on 21/7/18.
//

#include "AnimatedRenderer.hpp"
#include "ImageRenderer.hpp"
#include "../../Engine/Entity.hpp"
#include "../../Engine/Engine.hpp"
void AnimatedRenderer::Update(const float &deltaTime) {
    renderer->srcrect.x = renderer->srcrect.w * static_cast<int>((SDL_GetTicks() / animSpeed) % nframes);
}

void AnimatedRenderer::Render() {

}

void AnimatedRenderer::Start() {

}

void AnimatedRenderer::OnDestroy() {

}


AnimatedRenderer::AnimatedRenderer(ImageRenderer* rend, const int& frames, const int& speed) : nframes(frames), animSpeed(speed) {

renderer = rend;



}

void AnimatedRenderer::AdjustSpritePosition( const int &y) {
    renderer->srcrect.y = y;
}

void AnimatedRenderer::AdjustAnimSpeed(const int &speed) {
    animSpeed = speed;
}



