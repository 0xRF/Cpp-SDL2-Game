//
// Created by xrf on 5/10/18.
//

#include "Button.hpp"
#include "UI.hpp"
#include "Engine.hpp"
#include <SDL2pp/Optional.hh>
#include "../Tools.hpp"
Button::Button(std::function<void(void)>& func, const SDL2pp::Rect dst, bool* pDisabled) : pFunc((func)), dstrect(dst), pbDisabled(pDisabled) {

    UI::AddButton(this);

}

void Button::SetTopTexture(const std::string &szMessage, const SDL_Color normal, const SDL_Color hover) {
   top = UI::TextureFromString(szMessage.c_str(), normal);
   topHover = UI::TextureFromString(szMessage.c_str(), hover);
}

void Button::SetTopTexture(SDL2pp::Texture *normal, SDL2pp::Texture *pHover) {
    top = normal;
    topHover = pHover;
}

void Button::SetBottomTexture(SDL2pp::Texture *normal, SDL2pp::Texture *pHover, SDL2pp::Rect src) {
    bottom = normal;
    bottomHover = pHover;
    bSrcRect = src;
}
void Button::SetBottomTexture(SDL2pp::Texture *normal, SDL2pp::Texture *pHover) {
    bottom = normal;
    bottomHover = pHover;
}



void Button::Render(const bool& bHovered) {

    static Engine& engine = Engine::Instance();

    auto bTexture = bHovered ? bottomHover : bottom;
    auto fTexture = bHovered ? topHover : top;

    auto rct = dstrect;
    if(bHasPointers){
        rct.x += *dynamicPositions.first;
        rct.y += *dynamicPositions.second;
    }


    if(bTexture) {
        if (ZeroRect(bSrcRect)) {
            engine.g_pRenderer->Copy(*bTexture, SDL2pp::NullOpt, rct);
        } else {
            engine.g_pRenderer->Copy(*bTexture, bSrcRect, rct);
        }
    }

    if(fTexture){
        SDL2pp::Rect r = {rct.x + rct.w/4, rct.y + rct.h/4, rct.w/2, rct.h/2};
        engine.g_pRenderer->Copy(*fTexture, SDL2pp::NullOpt, r);
    }

}


Button::Button(const std::string &szMessage, std::function<void(void)> &func, const std::pair<int*, int*> &pos, bool *pBDisabled, const int &scale) : pFunc(func), pbDisabled(pBDisabled) {

    bottomHover = Engine::LoadTexture("assets/ui_button_hover.png");
    bottom = Engine::LoadTexture("assets/ui_button.png");

    top = UI::TextureFromString(szMessage.c_str(), {0, 0, 0});
    topHover = UI::TextureFromString(szMessage.c_str(), {255, 255, 255});

    dstrect = {0 - ((bottom->GetWidth()*scale)/2), 0 - ((bottom->GetHeight()*scale)/2), (int)(bottom->GetWidth() * scale), (int)(bottom->GetHeight()* scale) };
    bHasPointers = true;

    dynamicPositions = pos;
    UI::AddButton(this);
}


Button::Button(const std::string &szMessage, std::function<void(void)> &func, const std::pair<int, int> &pos, bool *pBDisabled, const int &scale) : pFunc(func), pbDisabled(pBDisabled) {

    bottomHover = Engine::LoadTexture("assets/ui_button_hover.png");
    bottom = Engine::LoadTexture("assets/ui_button.png");

    top = UI::TextureFromString(szMessage.c_str(), {0, 0, 0});
    topHover = UI::TextureFromString(szMessage.c_str(), {255, 255, 255});

    dstrect = {pos.first - (int)(bottom->GetWidth()*scale)/2, pos.second - (int)(bottom->GetHeight()*scale)/2, (int)(bottom->GetWidth() * scale), (int)(bottom->GetHeight()* scale)};

    UI::AddButton(this);
}
