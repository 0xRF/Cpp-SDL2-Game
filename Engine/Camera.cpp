#include "Camera.hpp"

Vector2 Camera::ScreenToWorldPoint(const int &x, const int &y) {
    return {x + viewport.x, y + viewport.y};
}

Vector2 Camera::ScreenToWorldPoint(const std::pair<int, int> &mPos) {
    return {mPos.first + viewport.x, mPos.second + viewport.y};
}

std::pair<int, int> Camera::WorldToScreenPoint(const int &x, const int &y) {
    return {x - viewport.x, y - viewport.y};
}

std::pair<int, int> Camera::WorldToScreenPoint(const std::pair<int, int> &mPos) {
    return {mPos.first - viewport.x, mPos.second - viewport.y};
}

std::pair<int, int> Camera::WorldToScreenPoint(const float &x, const float &y) {
    return {static_cast<int>(x - viewport.x), static_cast<int>(y - viewport.y)};
}

std::pair<int, int> Camera::WorldToScreenPoint(const Vector2 &pos) {
    return {pos.x - viewport.x, pos.y - viewport.y};
}



