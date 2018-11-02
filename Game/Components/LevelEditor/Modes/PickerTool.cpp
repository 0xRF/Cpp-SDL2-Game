
#include "PickerTool.hpp"
#include <SDL2/SDL.h>

#include "../../../../Engine/Engine.hpp"
#include <SDL2pp/Optional.hh>
#include "../../../../Engine/UI.hpp"
#include "../../../../Tools.hpp"
#include "../LevelEditor.hpp"

void PickerTool::OnUpdate() {

    auto mousePos = InputManager::GetMousePos();

    mousePos = SnapToGrid(mousePos, 64);

   // if(!boundries.Contains(mousePos.first, mousePos.second))
      //  return;



    if(InputManager::MouseDown(SDL_BUTTON_LEFT)) {

        if (ZeroRect(selrect)) {
            selrect.x = mousePos.first;
            selrect.y = mousePos.second;
        }
    }else if(InputManager::MouseHeld(SDL_BUTTON_LEFT)) {
        selrect.w = mousePos.first - selrect.x;
        selrect.h = mousePos.second - selrect.y;

      ///  if(selrect.x + selrect.w > boundries.x + boundries.w)
        //    selrect.w = boundries.w - selrect.x;

      //  if(selrect.y + selrect.h > boundries.y + boundries.h + 1)
       //     selrect.y = boundries.h - selrect.y;

    }else if(InputManager::MouseClicked(SDL_BUTTON_LEFT)){

        if(selrect.w != 0 && selrect.h != 0) {
            selrect = NormalizeRect(selrect);
            GrabTiles();
            selrect = {0, 0, 0, 0};
        }
    }



}

void PickerTool::OnRender() {

    if(pSnapShot) {
        UI::DrawTexture(pSnapShot, 0, 0, 1.0f);
    }
    else{

        Engine::Instance().g_pRenderer->FillRect({0,0,1280,720});
    }

    UI::DrawRect(boundries, {255,0,0});

        UI::DrawTexture(pEditor->pTileMap, boundries.x, boundries.y, 2.0f);

    UI::DrawRect(selrect, {255,0,0}, 1);

}

void PickerTool::OnDisable() {
    delete pSnapShot;
}

void PickerTool::OnActivate() {
    return;
    Blurr();
}



//never ever again touch this.
void PickerTool::Blurr() {


    int blurAmount = 3;
    SDL_Surface *sshot = SDL_CreateRGBSurface(0, 1280, 720, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);

    Engine::Instance().g_pRenderer->ReadPixels(SDL2pp::NullOpt, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);

    for (int y = 0; y < sshot->h; y++) {
        for (int x = 0; x < (sshot->pitch / 4); x++) {
            Uint32 color = ((Uint32 *) sshot->pixels)[(y * (sshot->pitch / 4)) + x];

            //SDL_GetRGBA() is a method for getting color
            //components from a 32 bit color
            Uint8 r = 0, g = 0, b = 0, a = 0;
            SDL_GetRGBA(color, sshot->format, &r, &g, &b, &a);

            Uint32 rb = 0, gb = 0, bb = 0, ab = 0;

            //Within the two for-loops below, colors of adjacent pixels are added up

            for (int xx = -blurAmount; xx <= blurAmount; xx++) {
                for (int yy = -blurAmount; yy <= blurAmount; yy++) {
                    if (y + xx >= 0 && x + yy >= 0
                        && y + xx < sshot->h && x + yy < (sshot->pitch / 4)
                            ) {
                        Uint32 colOth = ((Uint32 *) sshot->pixels)[((y + xx)
                                                                    * (sshot->pitch / 4)) + (x + yy)];

                        Uint8 ro = 0, go = 0, bo = 0, ao = 0;
                        SDL_GetRGBA(colOth, sshot->format, &ro, &go, &bo, &ao);

                        rb += ro;
                        gb += go;
                        bb += bo;
                        ab += ao;
                    }
                }
            }

            r = (Uint8) (rb / (((blurAmount * 2) + 1) * ((blurAmount * 2) + 1)));
            g = (Uint8) (gb / (((blurAmount * 2) + 1) * ((blurAmount * 2) + 1)));
            b = (Uint8) (bb / (((blurAmount * 2) + 1) * ((blurAmount * 2) + 1)));
            a = (Uint8) (ab / (((blurAmount * 2) + 1) * ((blurAmount * 2) + 1)));

            color = (r) | (g << 8) | (b << 16) | (a << 24);
            ((Uint32 *) sshot->pixels)[(y * (sshot->pitch / 4)) + x] = color;
        }
    }

    pSnapShot = new SDL2pp::Texture(*Engine::Instance().g_pRenderer, SDL2pp::Surface(sshot));
}

PickerTool::PickerTool(LevelEditor *_pEditor) : pEditor(_pEditor) {


    boundries.x = SnapToGrid(1280/2 - pEditor->pTileMap->GetWidth(), 64);
    boundries.y = SnapToGrid(720/2 - pEditor->pTileMap->GetHeight(), 64);
    boundries.w = pEditor->pTileMap->GetWidth()*2;
    boundries.h = pEditor->pTileMap->GetHeight()*2;
}

void PickerTool::GrabTiles() {

    auto tileVec = std::vector<std::pair<int,int>>();

    for(int x = selrect.x; x < selrect.x + selrect.x; x+=64){
        for(int y = selrect.y; y < selrect.y + selrect.h; y+=64){

            tileVec.push_back({
                x - boundries.x,
                y - boundries.y
            });
        }
    }

    pEditor->tilesCopyBuffer = {{selrect.w/64, selrect.h/64},tileVec};
}

void PickerTool::OnSave(const std::string& szMapDir) {

}

void PickerTool::OnLoad(const std::string& szMapDir) {

}

void PickerTool::ForceRender() {

}


































