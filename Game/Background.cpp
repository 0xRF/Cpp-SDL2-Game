#include "Background.hpp"
#include "../Engine/Engine.hpp"
#include <SDL2/SDL.h>
#include "../Engine/Camera.hpp"

struct BGTile{
    int width = 32;
    int height = 32;

    std::vector<SDL_Color> pixels = std::vector<SDL_Color>();

    BGTile(const int& x, const int& y, const int& w, const int& h, SDL_Surface* surface) : width(w), height(h), pixels(std::vector<SDL_Color>())
    {
        for (int i = x; i < x + w; i++)
            for(int j = y; j < y + h; j++) {

                SDL_Color* pixel = (SDL_Color *)((Uint8 *)surface->pixels + j * surface->pitch + i * surface->format->BytesPerPixel);
                  pixels.push_back(*pixel);
            }
    }

};

void BG::AddPixelsToSurface(const int& x, const int& y, BGTile* tile, SDL_Surface* surface) {

    for (int index = 0; index < tile->pixels.size(); index++) {

        int i = index % tile->width + x;
        int j = index / tile->width + y;


        auto pixel = (SDL_Color *) ((Uint8 *) surface->pixels + j * surface->pitch + i * surface->format->BytesPerPixel);

        if(i >= bounds.w || j >= bounds.h)
            break;

        *pixel = tile->pixels[index];
    }
}


void BG::GenerateBackground(const char *szFileName, const int &offsetx, const int &offsety, const int &width,
                        const int &height) {

    bounds.w = width*3;
    bounds.h = height*3;

    Uint32 rmask, gmask, bmask, amask;


    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif



    auto tileMap =  SDL2pp::Surface(szFileName);
    auto bgSurf = SDL2pp::Surface(0, bounds.w, bounds.h, 32, rmask, gmask , bmask, amask);


    std::vector<BGTile*> tiles = std::vector<BGTile*>();

    for(int i = 0; i < 8; i++){
        tiles.push_back(new BGTile(i * 32, 192 - 32, 32, 32, tileMap.Get()));
    }


    for(int i = 0; i < bounds.w; i+=32) {
        for (int j = 0; j < bounds.h; j += 32) {
            AddPixelsToSurface(i, j, tiles[rand() % 8], bgSurf.Get());


            // SDL_Color* pixel = (SDL_Color *)((Uint8 *)bgSurf.Get()->pixels + j * bgSurf.Get()->pitch + i * bgSurf.Get()->format->BytesPerPixel);

            //       *pixel = {0,0,255, 255};

        }
    }
    pTexture = new SDL2pp::Texture(*Engine::Instance().g_pRenderer, bgSurf);

}

void BG::Render(){

    SDL_Rect world = {0,0, Camera::Instance().viewport.w, Camera::Instance().viewport.h};
    SDL_Rect src = {bounds.x, bounds.y, bounds.w, bounds.h};
    SDL_RenderCopy(&*Engine::Instance().g_pRenderer->Get(), &*pTexture->Get(),  &src, nullptr);

  //  SDL_RenderCopyEx(), &*pTexture->Get(), &src, &world, 90.0f, &point, SDL_FLIP_NONE);
    //Engine::Instance().g_pRenderer->Copy(*pTexture);

}
