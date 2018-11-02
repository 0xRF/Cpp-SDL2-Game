#pragma once

#include <SDL2pp/Rect.hh>
#include "Singleton.hpp"
#include "Vector2.hpp"

class Camera : public Singleton<Camera>
{
public:
    SDL2pp::Rect viewport = {0,0, 1280, 720};
    Vector2 ScreenToWorldPoint(const int& x, const int& y);
    Vector2 ScreenToWorldPoint(const std::pair<int,int>& mPos);

    std::pair<int,int> WorldToScreenPoint(const int& x, const int& y);
    std::pair<int,int> WorldToScreenPoint(const std::pair<int,int>& mPos);
    std::pair<int,int> WorldToScreenPoint(const float& x, const float& y);
    std::pair<int,int> WorldToScreenPoint(const Vector2& pos);


};
