//
// Created by user0 on 7/21/18.
//

#include "ImageRenderer.hpp"
#include "../../Engine/Engine.hpp"
#include <SDL2pp/Optional.hh>
#include "../../Engine/Entity.hpp"
#include "../../Engine/Camera.hpp"
#include <cstdlib>
#include "../../Engine/Vector2.hpp"

inline  SDL2pp::Rect GenerateRect(SDL2pp::Texture* pTexture){
    return {0,0, pTexture->GetWidth(), pTexture->GetHeight()};
}



ImageRenderer::ImageRenderer(SDL2pp::Texture *texture) {
    pTexture = texture;
    srcrect = GenerateRect(pTexture);
}


ImageRenderer::ImageRenderer(SDL2pp::Texture *texture, const SDL2pp::Rect &srcrect) : pTexture(texture), srcrect(srcrect) {}


ImageRenderer::ImageRenderer(const char *szFileName) {
   pTexture = Engine::LoadTexture(szFileName);
   srcrect = GenerateRect(pTexture);
}

ImageRenderer::ImageRenderer(const char *szFileName, const SDL2pp::Rect &srcrect) : srcrect(srcrect) {
    pTexture = Engine::LoadTexture(szFileName);
}



void ImageRenderer::Update(const float &deltaTime) {

}

void ImageRenderer::Start() {


}

void ImageRenderer::OnDestroy() {


}

void ImageRenderer::Render(){

    SDL_Rect src = {srcrect.x, srcrect.y, srcrect.w, srcrect.h};

    static Camera& cam = Camera::Instance();

    auto sp = cam.WorldToScreenPoint(entity->position);

    SDL_Rect pointOnScreen = {sp.first, sp.second,
                      static_cast<int>(src.w * abs(entity->scale)), static_cast<int>(abs(entity->scale) * src.h)};

    SDL2pp::Rect vp = {0,0, cam.viewport.w, cam.viewport.h};

    if(!vp.Intersects(pointOnScreen))
        return;

    SDL_RenderCopyEx(&*Engine::Instance().g_pRenderer->Get(), &*pTexture->Get(), &src, &pointOnScreen, 0, NULL, entity->scale < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);

 //   Engine::Instance().g_pRenderer->Copy(pTexture, srcrect, SDL2pp::NullOpt);
      //   SDL_RenderCopy(Engine::Instance().g_pRenderer->Get(), &*pTexture->Get(), &src, &world);
}
