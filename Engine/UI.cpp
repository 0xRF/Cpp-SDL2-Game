//
// Created by rowan fimmano on 30/7/18.
//

#include "UI.hpp"
#include "Engine.hpp"
#include "Button.hpp"
#include <fstream>
namespace UI {


    static std::vector<Button*> vButtons = std::vector<Button*>();
    static std::vector<Button*> buttonsToAdd = std::vector<Button*>();
    SDL2pp::Texture* TextureFromString(const char* szMessage, const SDL_Color& color){
        auto surf = TTF_RenderText_Solid(pFont->Get(), szMessage, color);

        auto texture = new SDL2pp::Texture(SDL_CreateTextureFromSurface(&*Engine::Instance().g_pRenderer->Get(), surf));

        SDL_FreeSurface(surf);
        return texture;
    }


    void Start(){


        pFont = new SDL2pp::Font(std::string("arial.ttf"), 128, 0);
        if(pFont)std::cout << "yeet\n";
    }

    void DrawRect(const int &x, const int &y, const int &w, const int &h, const SDL2pp::Color &color) {
        DrawRect(x,y,w,h,color, 1);
    }

    void DrawRect(const SDL2pp::Rect &rect, const SDL2pp::Color &color) {
        DrawRect(rect,color, 1);
    }

    void DrawRect(const Vector2 &pos, const int& w, const int& h, const SDL2pp::Color &color) {
        DrawRect(pos,w,h,color,1);
    }


    void Render() {


        static Engine& engine = Engine::Instance();


        for(auto texture : textureBuffer){
            SDL_RenderCopyEx(&*Engine::Instance().g_pRenderer->Get(), &*texture.pTexture->Get(), &texture.srcrect, &texture.dstrect, 0, NULL, texture.scale < 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
        }
        textureBuffer.clear();

        for(auto rend : rectBuffer) {
            engine.g_pRenderer->SetDrawColor(rend.color);

            for (int i = 0; i < rend.cnt; ++i) {
                engine.g_pRenderer->DrawRect(
                        {rend.rect.x + i, rend.rect.y + i, rend.rect.w - (i * 2), rend.rect.h - (i * 2)});
            }
        }
        rectBuffer.clear();

        engine.g_pRenderer->SetDrawColor({255,255,255});


        for(auto texture : textBuffer){
            SDL_Rect r = {0,0, texture.pMessage->GetWidth(), texture.pMessage->GetHeight()};
            SDL_RenderCopyEx(&*Engine::Instance().g_pRenderer->Get(), &*texture.pMessage->Get(), &r, &texture.dstrect, 0, NULL, SDL_FLIP_NONE);
            delete texture.pMessage;
        }
        textBuffer.clear();

        auto mPos = InputManager::GetMousePos();


        for(auto button : vButtons) {

            if (!button|| *button->pbDisabled) {
              //  throw "Okden";
                continue;
            }
            bool hovered = false;
            if (button->bHasPointers) {
                auto dst = button->dstrect;
                dst.x += *(int *) button->dynamicPositions.first;
                dst.y += *(int *) button->dynamicPositions.second;
                hovered = dst.Contains(mPos.first, mPos.second);
            } else
                hovered = (button->dstrect.Contains(mPos.first, mPos.second));

            button->Render(hovered);

        }

        for(auto button : buttonBuffer){

            if(button.pTexture) {
                SDL_Rect r = {0, 0, button.pTexture->GetWidth(), button.pTexture->GetHeight()};
                SDL_RenderCopyEx(&*Engine::Instance().g_pRenderer->Get(), &*button.pTexture->Get(), &r, &button.dstrect,
                                 0, NULL, SDL_FLIP_NONE);
            }
            SDL_RenderCopyEx(&*Engine::Instance().g_pRenderer->Get(), &*button.pMessage->Get(), nullptr, &button.dstrect, 0, NULL, SDL_FLIP_NONE);
        }


    }

    void DrawTexture(SDL2pp::Texture *pTexture, const SDL2pp::Rect &rect, const int &x, const int &y,
                         const float& scale) {

        textureBuffer.push_back({pTexture, rect, {x, y, static_cast<int>(rect.w * scale), static_cast<int>(rect.h * scale)}});
    }

    void DrawTexture(SDL2pp::Texture *pTexture, const int &x, const int &y, const float &scale) {

        textureBuffer.push_back(
                {pTexture, {0,0, pTexture->GetWidth(), pTexture->GetHeight()},
                 {x, y, static_cast<int>(pTexture->GetWidth() * scale), static_cast<int>(pTexture->GetHeight() * scale)}});
    }

    void DrawString(const char *szMessage, const int &x, const int &y, const int &size, const SDL_Color& color) {

        auto texture = TextureFromString(szMessage, color);

       textBuffer.push_back({{x, y, (texture->GetWidth() * size )/ 20, (texture->GetHeight() * size) /20}, texture, color});
    }

    void DrawString(const char *szMessage, const std::pair<int, int>& pr, const int &size, const SDL_Color& color) {
            DrawString(szMessage, pr.first, pr.second, size, color);
    }

    void DrawString(const char *szMessage, Vector2 &pos, const int &size, const SDL_Color& color) {
        DrawString(szMessage, static_cast<int>(pos.x), static_cast<int>(pos.y), size, color);
    }

    void DrawRect(const Vector2 &pos, const int& w, const int& h, const SDL2pp::Color &color, const int &size) {
        rectBuffer.push_back({{static_cast<int>(pos.x), static_cast<int>(pos.y), w, h}, color, size});
    }

    void DrawRect(const int &x, const int &y, const int &w, const int &h, const SDL2pp::Color &color, const int &size) {
        rectBuffer.push_back({ {x, y, w, h}, color, size});
    }

    void DrawRect(const SDL2pp::Rect &rect, const SDL2pp::Color &color, const int &size) {
        rectBuffer.push_back({rect, color, size});
    }

    void RegisterButton(const char *szMessage,  std::function<void(void)> &func, const SDL_Color &col,
                        SDL2pp::Texture *pButton, const int &x, const int &y, const int &w, const int &h,
                        bool *bDelete) {
        RegisterButton(szMessage, func, col, pButton, {x,y,w,h}, bDelete);

    }

    void RegisterButton(const char *szMessage,  std::function<void(void)> &func, const SDL_Color &col, SDL2pp::Texture *pButton, const SDL_Rect &rect, bool *bDelete) {

        buttonBuffer.push_back({rect, TextureFromString(szMessage, col), pButton, bDelete, func});
    }

    void RegisterButton(const char *szMessage,  std::function<void(void)> &func, const SDL_Color &col,
                            SDL2pp::Texture *pButton, const Vector2 &pos, const int &w, const int &h, bool *bDelete) {

        RegisterButton(szMessage, func, col, pButton, {static_cast<int>(pos.x), static_cast<int>(pos.y), w, h}, bDelete);
    }

    void RegisterButton(const char *szMessage,  std::function<void(void)> &func, const SDL_Color &col,
                            SDL2pp::Texture *pButton, const std::pair<int, int> &pr, const int &w, const int &h,
                            bool *bDelete) {
        RegisterButton(szMessage, func, col, pButton, {pr.first,pr.second,w,h}, bDelete);
    }

    void DrawRect(const std::pair<int, int> &pos, const int &w, const int &h, const SDL2pp::Color &color) {
    DrawRect(pos.first, pos.second, w,h, color);
    }

    void DrawRect(const std::pair<int, int> &pos, const int &w, const int &h, const SDL2pp::Color &color,
                      const int &size) {
        DrawRect(pos.first, pos.second, w,h, color, size);

    }


    void HandleButtons() {

        if(InputManager::MouseClicked(SDL_BUTTON_LEFT)) {
            auto mPos = InputManager::GetMousePos();

            for (auto button : vButtons) {

                if (!button)
                    continue;

                if (button->pbDisabled == nullptr)
                    continue;

                if (*button->pbDisabled)
                    continue;

                bool hovered = false;
                if (button->bHasPointers) {
                    auto dst = button->dstrect;
                    dst.x += *(int *) button->dynamicPositions.first;
                    dst.y += *(int *) button->dynamicPositions.second;
                    hovered = dst.Contains(mPos.first, mPos.second);
                } else
                    hovered = (button->dstrect.Contains(mPos.first, mPos.second));

                if (hovered)
                    button->pFunc();

            }
        }
        for(auto but : buttonsToAdd)
            vButtons.push_back(but);
        buttonsToAdd.clear();

    }

    void AddButton(Button *pButton) {
        buttonsToAdd.push_back(pButton);
    }


}