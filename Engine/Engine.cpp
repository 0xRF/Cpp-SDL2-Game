#include <SDL2pp/SDL2pp.hh>
#include "Engine.hpp"
#include "Entity.hpp"
#include <SDL2/SDL.h>
#include <SDL2pp/SDLImage.hh>
#include <SDL2/SDL_image.h>
#include <SDL2pp/SDLTTF.hh>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include "Entity.hpp"
#include "../Game/Entities/Player.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include "../Game/Background.hpp"
#include "Component.hpp"
#include "UI.hpp"
#include "Camera.hpp"
#include "../imgui_sdl.h"
#include "../imgui/imgui.h"
#include "../imgui/examples/imgui_impl_sdl.h"
#include "../Game/Entities/GameManager.hpp"
#include "../imgui/imgui.h"


static Engine* _engine;

static SDL2pp::SDL* _SDL_;
static SDL2pp::SDLImage* _IMAGE_;
static SDL2pp::SDLTTF* _TTF_;

static SDL_Thread* p_tRender;

static std::vector<BaseEntity*> entToAdd = std::vector<BaseEntity*>();
const bool Engine::Start()
{
    _engine = new Engine;


    //init all the sdl features required
   _SDL_   = new SDL2pp::SDL(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
   _IMAGE_ = new SDL2pp::SDLImage(IMG_INIT_PNG | IMG_INIT_JPG);
   _TTF_   = new SDL2pp::SDLTTF();


//if any of them failed, dont start the game..
    if(!_SDL_ || !_IMAGE_ || !_TTF_)return false;

    //I dont know why i made it an object, but yea
    _engine->inputManager = new InputManager;

    //create and set the sdl2 window
    _engine->g_pWindow = new SDL2pp::Window("Memegeon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _engine->windowRect.w, _engine->windowRect.h,  SDL_WINDOW_RESIZABLE |SDL_WINDOW_ALLOW_HIGHDPI);


    //SDL_GL_MakeCurrent(_engine->g_pWindow->Get(), nullptr);
//Create the sdl2 renderer which draws textures to our window
    _engine->g_pRenderer = new SDL2pp::Renderer(*_engine->g_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    //Initilazeing the imgui implementation for the level editor
    ImGui::CreateContext();
    ImGuiSDL::Initialize(_engine->g_pRenderer->Get(), 1280, 720);
    ImGui_ImplSDL2_Init(_engine->g_pWindow->Get());

    //init some ui things, eg loading the font
    UI::Start();
   // p_tRender = SDL_CreateThread(RenderThread, "t_Render", (void*)NULL);

    return true;
}

void Engine::Stop() {

    if(!_engine)return;
    _engine->bShutdown = true;
}

Engine& Engine::Instance() {
    return *_engine;
}

int main(int argc, const char* args[]){

    SDL_assert(Engine::Start());

    Engine::Instance().Update();


    return 0;
}


int Engine::RenderThread(void* data){

    std::cout << "Render Thread Started\n";




    // _engine->Render();

    return 0;
}


//this was left in from when the rendering of the game and the update loop were seperated
void Engine::Render() {

    while(!bShutdown) {

        for(auto it = entityList.begin(); it != entityList.end(); ++it) {

                for(int i = 0; i < 8; i++) {
                    BaseComponent *pComp = (*it)->components[i];
                    if (!pComp)continue;

                //    if (!pComp->bDestroy)
           //         pComp->Render();
                }
            }
        g_pRenderer->Present();
        g_pRenderer->Clear();
    }
}


void Engine::Update() {

    std::cout << "Update Started\n Waiting for Render Thread\n";

    while(!g_pRenderer){ SDL_Delay(300);}
    std::cout << "Render & Window Created!\n";

    BG::GenerateBackground("assets/duntiles.png", 0,0, 2400, 2400);

   //LevelManager::LoadMap("uLevel");

    new GameManager;

    SDL_Event event;
    float lastFrame = 0;
    float deltaTime = 1;

    int lastFrameUpdate = 0;
    unsigned int fps = 0;
    unsigned int lastFps = 0;

    while(!bShutdown) {

      InputManager::ResetInput();
      ImGuiIO& io = ImGui::GetIO();

      int wheel = 0;

      while (SDL_PollEvent(&event)) {



          if(event.type == SDL_QUIT) {
              bShutdown = true;
              break;
          }else if (event.type == SDL_WINDOWEVENT_RESIZED){

              Camera::Instance().viewport.w = event.window.data1;
              Camera::Instance().viewport.h = event.window.data2;
              io.DisplaySize.x = static_cast<float>(event.window.data1);
              io.DisplaySize.y = static_cast<float>(event.window.data2);
          }
          else if (event.type == SDL_WINDOWEVENT)
          {
              if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                  io.DisplaySize.x = static_cast<float>(event.window.data1);
                  io.DisplaySize.y = static_cast<float>(event.window.data2);
                  Camera::Instance().viewport.w = event.window.data1;
                  Camera::Instance().viewport.h = event.window.data2;
              }
          }
          else if (event.type == SDL_MOUSEWHEEL)
          {
              wheel = event.wheel.y;
          }
          ImGui_ImplSDL2_ProcessEvent(&event);
          InputManager::UpdateInput(event);
      }

        int mouseX, mouseY;
        const int buttons = SDL_GetMouseState(&mouseX, &mouseY);

        // Setup low-level inputs (e.g. on Win32, GetKeyboardState(), or write to those fields from your Windows message loop handlers, etc.)

        io.DeltaTime = 1.0f / 60.0f;
        io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
        io.MouseWheel = static_cast<float>(wheel);


        ImGui::NewFrame();

        for(auto it = entToAdd.begin(); it != entToAdd.end(); ++it) {
            entityList.push_back((*it));
        }
        entToAdd.clear();


        auto it = entityList.begin();

        while (it != entityList.end()) {


            if((*it)->bDestroy) {
                (*it)->OnDestroy();

                delete (*it);
               it = entityList.erase(it);

            }
            else {


                if((*it)->bNeedsStart) {
                    (*it)->Start();
                    (*it)->bNeedsStart = false;
                }

                for(int i = 0; i < 8; i++) {


                    BaseComponent *pComp = (*it)->components[i];
                    if (!pComp)continue;

                     if (pComp->bDestroy) {
                        size_t size =  pComp->Size();
                        delete pComp;
                        std::cout << "Destroyed Component\n";
                        (*it)->components[i]  = 0;

                    } else {

                         pComp->ForceUpdate();

                         if((*it)->bEnabled) {

                             if (pComp->bEnabled) {
                                 pComp->Update(deltaTime);
                                 pComp->Render();

                                 if (bShutdown)
                                     break;
                             }
                         }
                     }
                }
                ++it;
            }
        }

        if(!bShutdown) {
            UI::Render();
            UI::HandleButtons();

            ImGui::Render();
            ImGuiSDL::Render(ImGui::GetDrawData());

            g_pRenderer->Present();
            g_pRenderer->Clear();

            BG::Render();

            auto currTicks = SDL_GetTicks();


            if (lastFrameUpdate < currTicks - 1.0 * 1000) {
                lastFps = fps;
                g_pWindow->SetTitle("FPS " + std::to_string(fps));
                fps = 0;
                lastFrameUpdate = SDL_GetTicks();
            }

            UI::DrawString("FPS: ", 10, 20, 2, {0, 255, 255});
            UI::DrawString(lastFps, 40, 20, 2, {0, 255, 255});

            if (lastFrame < 1000 / 120) {
                //        SDL_Delay((1000 / 120) - lastFrame);
            }

            deltaTime = (currTicks - lastFrame) / 1000;
            lastFrame = currTicks;
            fps++;
        }
    }

    ImGuiSDL::Deinitialize();


    delete _IMAGE_;
    delete _TTF_;
    delete _SDL_;

    ImGui::DestroyContext();
}

SDL2pp::Texture* Engine::LoadTexture(const char* szFileName){

    //rather than loading the same texture in to memory twice, keep track of them this
    static auto pMap = new std::map<const char *, SDL2pp::Texture *>();

    //if there is a texture of the same file
    if(pMap->count(szFileName)) {
      //  std::cout << "Texture already exists\n";
        //return it..
        return (*pMap)[szFileName];
    }
    //else set the value of the texture ptr at szfilename to a new texture loading
    (*pMap)[szFileName] = new SDL2pp::Texture(*Engine::Instance().g_pRenderer, szFileName);

    std::cout  << "IMG ERR " << IMG_GetError() << std::endl;

    //return it.
    return (*pMap)[szFileName];
}

void Engine::AddEntity(BaseEntity *pEnt) {
    entToAdd.push_back(pEnt);
}

//find entity of given type, probs never gonna use it tho.
BaseEntity* Engine::FindObjectOfType(const std::size_t &id) {

    for(auto pEnt : Engine::Instance().entityList) {

        if(pEnt->ID() == id)
            return pEnt;
    }
    std::cout << "Failed to find type\n";
    return nullptr;
}


