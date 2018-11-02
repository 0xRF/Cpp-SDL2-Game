#pragma once

#include <SDL2pp/SDL2pp.hh>

struct BGTile;
namespace BG {
    void GenerateBackground(const char *szFileName, const int &offsetx, const int &offsety, const int &width, const int &height);
    void AddPixelsToSurface(const int& x, const int& y, BGTile* tile, SDL_Surface* surface);
    static SDL2pp::Texture* pTexture = nullptr;
    static SDL2pp::Rect bounds = {0,0,0,0};

    void Render();
}