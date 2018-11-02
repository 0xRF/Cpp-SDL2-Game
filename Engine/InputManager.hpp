#pragma once

#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_events.h>



class InputManager {
public:
    static void UpdateInput(const SDL_Event& event);
    static void ResetInput();


    static bool KeyPressed(const SDL_Keycode& keycode);
    static bool KeyHeld(const SDL_Keycode& keycode);
    static bool KeyDown(const SDL_Keycode& keycode);

    static std::pair<int,int> GetMousePos();

    static bool MouseHeld(const Uint8& mbutton);
    static bool MouseDown(const Uint8& mbutton);
    static bool MouseClicked(const Uint8& mbutton);
};

