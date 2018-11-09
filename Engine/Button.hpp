#pragma once

#include <SDL2pp/Texture.hh>

class Engine;


class Button {

public:


    Button(std::function<void(void)>& func, const SDL2pp::Rect dst, bool* pDisabled);

    Button(const std::string szMessage, std::function<void(void)>& func, const std::pair<int,int>& pos,  bool* pBDisabled, const int& scale);
    Button(const std::string szMessage, std::function<void(void)> func, const std::pair<intptr_t, intptr_t> &pos, bool *pBDisabled, const int &scale);


    void SetTopTexture(const std::string szMessage, const SDL_Color normal, const SDL_Color hover);
    void SetTopTexture(SDL2pp::Texture* normal, SDL2pp::Texture* pHover);

    void SetBottomTexture(SDL2pp::Texture* normal, SDL2pp::Texture* pHover, SDL2pp::Rect src);
    void SetBottomTexture(SDL2pp::Texture* normal, SDL2pp::Texture* pHover);
    bool* pbDisabled = nullptr;
    SDL2pp::Rect dstrect = {0,0,0,0};
    std::function<void(void)> pFunc;

    std::pair<intptr_t, intptr_t> dynamicPositions {};
    bool bHasPointers = false;
    void Render(const bool& bHovered);

    ~Button();
private:

    SDL2pp::Texture* top = nullptr;
    SDL2pp::Texture* bottom = nullptr;

    SDL2pp::Texture* topHover = nullptr;
    SDL2pp::Texture* bottomHover = nullptr;

    SDL2pp::Rect bSrcRect = {0,0,0,0};

};

