#pragma once

#include <vector>
#include "Engine/Vector2.hpp"
#include <SDL2pp/Rect.hh>
#include "Engine/Camera.hpp"
#define sci static_cast<int>
#define scf static_cast<int>

inline int SnapToGrid(const int& val, const int& grd)
{
    return val/grd * grd;
}

inline std::pair<int,int> SnapToGrid(const std::pair<int,int>& val, const int& grd)
{
    return  {val.first/grd *grd, val.second/grd*grd};
}
inline Vector2 SnapToGrid(const Vector2& val, const int& grd)
{
	return {val.x/grd*grd, val.y/grd*grd};
}

inline SDL2pp::Rect NormalizeRect(const SDL2pp::Rect& rect){
  SDL2pp::Rect r = {0,0,0,0}; 

  if(rect.w < 0){
    r.w = abs(rect.w);
    r.x = rect.x - r.w;
  }else {
      r.x = rect.x;
      r.w = rect.w;
  }


  if(rect.h < 0){
    r.h = abs(rect.h);
    r.y = rect.y - r.h;
  }
  else{
      r.y = rect.y;
      r.h = rect.h;
  }
  return r;

}

inline bool ZeroRect(SDL2pp::Rect r){
  return r.x == 0 && r.y == 0 && r.w == 0 && r.h == 0;
}

inline SDL2pp::Rect WorldToScreen(SDL2pp::Rect r){

    auto sp = Camera::Instance().WorldToScreenPoint(r.x, r.y);

  return {sp.first, sp.second, r.w,r.h};
}

inline SDL2pp::Rect ScreenToWorld(SDL2pp::Rect r){

    auto wpp = Camera::Instance().ScreenToWorldPoint(r.x, r.y);

    return {(int)wpp.x, (int)wpp.y, r.w, r.h};
}
