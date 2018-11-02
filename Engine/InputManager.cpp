
#include "InputManager.hpp"
#include <SDL2/SDL.h>
#include <vector>
#include <map>
#include <iostream>
#include "Camera.hpp"
#include <algorithm>
#include "../imgui/imgui.h"

std::vector<SDL_Keycode> held = std::vector<SDL_Keycode>();
std::vector<SDL_Keycode> up = std::vector<SDL_Keycode>();
std::vector<SDL_Keycode> down = std::vector<SDL_Keycode>();



std::vector<Uint8 > mHeld = std::vector<Uint8>();
std::vector<Uint8> mUp = std::vector<Uint8>();
std::vector<Uint8> mDown = std::vector<Uint8>();


void InputManager::UpdateInput(const SDL_Event& event) {

    switch (event.type){

            case SDL_KEYDOWN:
                if(!(std::find(down.begin(), down.end(), event.key.keysym.sym) != down.end()))
                    down.push_back(event.key.keysym.sym);

            if(!(std::find(held.begin(), held.end(), event.key.keysym.sym) != held.end()))
                    held.push_back(event.key.keysym.sym);
            break;


            case SDL_KEYUP:

                if(!(std::find(up.begin(), up.end(), event.key.keysym.sym) != up.end()))
                    up.push_back(event.key.keysym.sym);

                if((std::find(held.begin(), held.end(), event.key.keysym.sym) != held.end()))
                    held.erase(std::remove(held.begin(), held.end(), event.key.keysym.sym), held.end());
            break;


        case SDL_MOUSEBUTTONDOWN:
                if(!(std::find(mDown.begin(), mDown.end(), event.button.button) != mDown.end()))
                    mDown.push_back(event.button.button);

                if(!(std::find(mHeld.begin(), mHeld.end(), event.button.button) != mHeld.end()))
                    mHeld.push_back(event.button.button);
            break;

        case SDL_MOUSEBUTTONUP:

            if(!(std::find(mUp.begin(), mUp.end(), event.button.button) != mUp.end()))
                mUp.push_back(event.button.button);

            if((std::find(mHeld.begin(), mHeld.end(), event.button.button) != mHeld.end()))
                mHeld.erase(std::remove(mHeld.begin(), mHeld.end(), event.button.button), mHeld.end());

            break;
    }
}


bool InputManager::KeyDown(const SDL_Keycode &key) {
    return (std::find(down.begin(), down.end(), key) != down.end()) && !ImGui::IsAnyItemActive();
}

bool InputManager::KeyHeld(const SDL_Keycode &key){
    return (std::find(held.begin(), held.end(), key) != held.end()) && !ImGui::IsAnyItemActive();
}


bool InputManager::KeyPressed(const SDL_Keycode &key){
    return (std::find(up.begin(), up.end(), key) != up.end()) && !ImGui::IsAnyItemActive();;
}


bool InputManager::MouseDown(const Uint8 &mbutton) {
    return std::find(mDown.begin(), mDown.end(), mbutton) != mDown.end() && !ImGui::IsAnyItemActive() && !ImGui::IsAnyWindowHovered();
}

bool InputManager::MouseClicked(const Uint8 &mbutton) {
    return std::find(mUp.begin(), mUp.end(), mbutton) != mUp.end() && !ImGui::IsAnyItemActive() && !ImGui::IsAnyWindowHovered();
}

bool InputManager::MouseHeld(const Uint8 &mbutton) {

    return std::find(mHeld.begin(), mHeld.end(), mbutton) != mHeld.end() && !ImGui::IsAnyItemActive() && !ImGui::IsAnyWindowHovered();
}


std::pair<int, int> InputManager::GetMousePos() {
    int x = 0,  y = 0;
    SDL_GetMouseState(&x, &y);

    return {x, y};
}

void InputManager::ResetInput() {
    up.clear();
    down.clear();
    mDown.clear();
    mUp.clear();
}
