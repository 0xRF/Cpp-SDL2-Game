#pragma once

#include <SDL2pp/SDL2pp.hh>
#include <vector>
#include "Vector2.hpp"

struct RectInfo{
    RectInfo(const SDL2pp::Rect& r, const SDL2pp::Color& col, const int& amount) : rect(r), color(col), cnt(amount){}
    RectInfo() : rect{0,0,0,0}, color{0,0,0,0}{}
    SDL2pp::Rect rect;
    SDL2pp::Color color;
    int cnt = 0;
};

struct TextureInfo{
    TextureInfo(SDL2pp::Texture* texture, const SDL_Rect& src, const SDL_Rect& dst) : pTexture(texture), srcrect(src), dstrect(dst) {}
    SDL_Rect srcrect = {0,0,0,0};
    SDL_Rect dstrect = {0,0,0,0};
    SDL2pp::Texture* pTexture = nullptr;
    float scale = 0.0f;
};

struct TextInfo{
    TextInfo(const SDL_Rect& rect, SDL2pp::Texture* pTexture, const SDL_Color& col) : dstrect(rect), pMessage(pTexture), color(col){}
    SDL_Rect dstrect = {0,0,0,0};
    SDL2pp::Texture* pMessage = nullptr;
    SDL_Color color = {0,0,0};
};

struct ButtonInfo{
    ButtonInfo(const SDL_Rect& rect, SDL2pp::Texture* pMsg, SDL2pp::Texture* pText, bool* bDel,  std::function<void(void)> fc) : dstrect(rect), pMessage(pMsg), pTexture{pText}, bDelete(bDel), func(fc){}
    SDL_Rect dstrect = {0,0,0,0};
    SDL2pp::Texture* pMessage = nullptr;
    SDL2pp::Texture* pTexture = nullptr;;
    bool* bDelete = nullptr;
    std::function<void(void)> func = nullptr;
};

class Button;

namespace UI
{
    static std::vector<RectInfo> rectBuffer = std::vector<RectInfo>();
    static std::vector<TextureInfo> textureBuffer = std::vector<TextureInfo>();
    static std::vector<TextInfo> textBuffer = std::vector<TextInfo>();
    static std::vector<ButtonInfo> buttonBuffer = std::vector<ButtonInfo>();
    static SDL2pp::Font* pFont = nullptr;

    void Start();
    void Render();
    void HandleButtons();

    void AddButton(Button* pButton);

    SDL2pp::Texture* TextureFromString(const char* szMessage, const SDL_Color& color);

    void RegisterButton(const char* szMessage, std::function<void(void)>& func, const SDL_Color& col, SDL2pp::Texture* pButton, const int& x, const int& y, const int& w, const int& h, bool* bDelete);
    void RegisterButton(const char* szMessage, std::function<void(void)>& func, const SDL_Color& col, SDL2pp::Texture* pButton, const SDL_Rect& rect, bool* bDelete);
    void RegisterButton(const char* szMessage, std::function<void(void)>& func, const SDL_Color& col, SDL2pp::Texture* pButton, const Vector2& pos, const int& w, const int& h, bool* bDelete);
    void RegisterButton(const char* szMessage, std::function<void(void)>& func, const SDL_Color& col, SDL2pp::Texture* pButton, const std::pair<int,int>& pr, const int& w, const int& h, bool* bDelete);

    template<typename T>
    void RegisterButton(const T &szMessage, std::function<void(void)> &func, const SDL_Color &col,
                        SDL2pp::Texture *pButton, const int &x, const int &y, const int &w, const int &h,
                        bool *bDelete) {
        RegisterButton(std::to_string(szMessage), func, col, pButton, {x,y,w,h}, bDelete);
    }

    template<typename T>
    void RegisterButton(const T &szMessage, std::function<void(void)> &func, const SDL_Color &col,
                        SDL2pp::Texture *pButton, const SDL_Rect &rect, bool *bDelete) {
        RegisterButton(std::to_string(szMessage), func, col, pButton, rect, bDelete);
    }

    template<typename T>
    void RegisterButton(const T &szMessage, std::function<void(void)> &func, const SDL_Color &col,
                        SDL2pp::Texture *pButton, const Vector2 &pos, const int &w, const int &h, bool *bDelete) {
        RegisterButton(std::to_string(szMessage), func, col, pButton, {static_cast<int>(pos.x), static_cast<int>(pos.y), w, h}, bDelete);
    }

    template<typename T>
    void RegisterButton(const T &szMessage, std::function<void(void)> &func, const SDL_Color &col,
                        SDL2pp::Texture *pButton, const std::pair<int, int> &pr, const int &w, const int &h,
                        bool *bDelete) {
        RegisterButton(std::to_string(szMessage), func, col, pButton, {pr.first,pr.second,w,h}, bDelete);
    }

    void DrawRect(const std::pair<int,int>& pos, const int& w, const int& h, const SDL2pp::Color& color);
    void DrawRect(const std::pair<int,int>& pos, const int& w, const int& h, const SDL2pp::Color& color, const int& size);
    void DrawRect(const int& x, const int& y, const int& w, const int& h, const SDL2pp::Color& color);
    void DrawRect(const SDL2pp::Rect& rect, const SDL2pp::Color& color);
    void DrawRect(const Vector2& pos,const int& w, const int& h, const SDL2pp::Color& color);

    void DrawRect(const Vector2& pos, const int& w, const int& h, const SDL2pp::Color& color, const int& size);
    void DrawRect(const int& x, const int& y, const int& w, const int& h, const SDL2pp::Color& color, const int& size);
    void DrawRect(const SDL2pp::Rect& rect, const SDL2pp::Color& color, const int& size);

    void DrawTexture(SDL2pp::Texture* pTexture, const int& x, const int& y, const float& scale);
    void DrawTexture(SDL2pp::Texture* pTexture, const SDL2pp::Rect& rect, const int& x, const int& y, const float& scale);

    void DrawString(const char* szMessage, const int& x, const int& y, const int& size, const SDL_Color& col);
    void DrawString(const char* szMessage, std::pair<int,int>& pr, const int& size, const SDL_Color& col);
    void DrawString(const char* szMessage, Vector2& pos, const int& size, const SDL_Color& col);

    template<typename T>
    void DrawString(const T ms, const int &x, const int &y, const int &size, const SDL_Color &col) {
        DrawString(std::to_string(ms).c_str(), x, y, size, col);
    }

    template<typename T>
    void DrawString(const T ms, std::pair<int, int> &pr, const int &size, const SDL_Color &col) {
        DrawString(std::to_string(ms).c_str(), pr, size, col);
    }

    template<typename T>
    void DrawString(const T ms, Vector2 &pos, const int &size, const SDL_Color &col) {
        DrawString(std::to_string(ms).c_str(), pos, size, col);
    }

};