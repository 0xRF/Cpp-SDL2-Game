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

/*
#import "SDL2_image/SDL_image.h"
#import "SDL2/SDL.h"
#import "SDL2_mixer/SDL_mixer.h"
#import "SDL2_ttf/SDL_ttf.h"
*/
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
   _SDL_   = new SDL2pp::SDL(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO);
   _IMAGE_ = new SDL2pp::SDLImage(IMG_INIT_PNG | IMG_INIT_JPG);
   _TTF_   = new SDL2pp::SDLTTF();

//if any of them failed, dont start the game..
    if(!_SDL_ || !_IMAGE_ || !_TTF_) {
        std::cout << "Failed to init sdl\n";
        return false;
    }
    //I dont know why i made it an object, but yea
    _engine->inputManager = new InputManager;

    //create and set the sdl2 window
    _engine->g_pWindow = new SDL2pp::Window("Memegeon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _engine->windowRect.w, _engine->windowRect.h,  SDL_WINDOW_RESIZABLE |SDL_WINDOW_ALLOW_HIGHDPI);

    if(!_engine->g_pWindow)
    {
        std::cout << "Failed to init window\n";
        return false;
    }


    //SDL_GL_MakeCurrent(_engine->g_pWindow->Get(), nullptr);
//Create the sdl2 renderer which draws textures to our window
    _engine->g_pRenderer = new SDL2pp::Renderer(*_engine->g_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(!_engine->g_pRenderer)
    {
        std::cout << "Failed to init renderer\n";
        return false;
    }

    //init audio
    _engine->g_pMixer = new SDL2pp::Mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

    if(!_engine->g_pWindow)
    {
        std::cout << "Failed to init mixer\n";
        return false;
    }

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
    _engine->bShutdown = true;
}

Engine& Engine::Instance() {
    return *_engine;
}

#if _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#include <unistd.h>

#endif


void loadAsm(const char* assm){

    bool bLoaded = dlopen(assm, RTLD_GLOBAL) != NULL;
    if(!bLoaded) {
        std::cout << "Failed to load " << assm << std::endl;
        std::cin.get();
        exit(-1);
    }
}

//going to be typedef for windows and others etc.
const char* libraries[] = {
        "libSDL2-2.0.0.dylib",
        "libSDL2_image-2.0.0.dylib",
        "libSDL2_mixer-2.0.0.dylib",
        "libSDL2_ttf-2.0.0.dylib",
        "libfreetype.6.dylib",
        "libjpeg.9.dylib",
        "libmodplug.1.dylib",
        "libpng16.16.dylib",
        "libSystem.B.dylib",
        "libtiff.5.dylib",
        "libtiffxx.5.dylib",
        "libvorbisfile.3.dylib",
        "libwebp.7.dylib",
        "libwebpdecoder.3.dylib",
        "libwebpdemux.2.dylib",
        "libwebpmux.3.dylib"
}; //16 libs
int main(int argc, const char* args[]){

    //set the current working directy a fixup for macosx
    std::string meme(args[0]);
    chdir(meme.substr(0, meme.size() - 8).c_str());


    std::cout << "Loading dependencies\n";

    //load libraries
    for(int i = 0; i <16; i++){
     //  loadAsm(("libs/" + std::string(libraries[i])).c_str());
    }

    //if engine returns false quit application idk, why I used assert.
    SDL_assert(Engine::Start());

    //run the update loop
    Engine::Instance().Update();


    return 0;
}


//ignore
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

    //was in here when there was a seperate render thread
    while(!g_pRenderer){ SDL_Delay(300);}
    std::cout << "Render & Window Created!\n";

    //create ackground, desiered effect not achived by who cares.
    BG::GenerateBackground("assets/duntiles.png", 0,0, 2400, 2400);


    //play the background music provided by the music students
    SDL2pp::Music music("assets/song.mp3");
    PlaySound(&music, true);


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

      //handle events
              //eg adjusting the view of the cameras
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

        //setup input for imgui
        io.DeltaTime = 1.0f / 60.0f;
        io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
        io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
        io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
        io.MouseWheel = static_cast<float>(wheel);


        ImGui::NewFrame();

        //as entities and other functions look at the entity list, we need to seperate added funcitons at the stsrt so the size
        // of the vector doesnt change which screws with looping through all entitty componenets
        for(auto it = entToAdd.begin(); it != entToAdd.end(); ++it) {
            entityList.push_back((*it));
        }
        entToAdd.clear();


        auto it = entityList.begin();

        while (it != entityList.end()) {


            //if entity is destoryed remove it now so we can update the iterator
            if ((*it)->bDestroy) {
                (*it)->OnDestroy();

                try {
                    delete (*it);
                }
                catch(const std::exception& e) {}

                it = entityList.erase(it);

            } else {

            //if entiy needs start, call start function, probaby could move this above in the entity to add loop
                if ((*it)->bNeedsStart) {
                    (*it)->Start();
                    (*it)->bNeedsStart = false;
                }

                for (int i = 0; i < 8; i++) {


                    BaseComponent *pComp = (*it)->components[i];
                    if (!pComp)continue;

                    if (pComp->bDestroy) {
                        size_t size = pComp->Size();
                        delete pComp;
                        std::cout << "Destroyed Component\n";
                        (*it)->components[i] = 0;

                    } else {

                        pComp->ForceUpdate();

                        if ((*it)->bEnabled) {

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

            if(g_pRenderer) {
            ImGui::Render();
            ImGuiSDL::Render(ImGui::GetDrawData());

                g_pRenderer->Present();
                g_pRenderer->Clear();

                BG::Render();
            }
            auto currTicks = SDL_GetTicks();

            //count fps
            if (lastFrameUpdate < currTicks - 1.0 * 1000) {
                lastFps = fps;
           //     g_pWindow->SetTitle("FPS " + std::to_string(fps));
                fps = 0;
                lastFrameUpdate = SDL_GetTicks();
            }


            //UI::DrawString("FPS: ", 10, 20, 2, {0, 255, 255});
          //  UI::DrawString(lastFps, 40, 20, 2, {0, 255, 255});


            //count fps
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
//explained in Engine::Update
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

void Engine::PlaySound(const SDL2pp::Chunk* pSound){
    if(!_engine || !_engine->g_pMixer)
        return;

   _engine->g_pMixer->PlayChannel(-1, *pSound);
}
void Engine::PlaySound(const SDL2pp::Music *pSound, bool bLoop) {

    if(!_engine || !_engine->g_pMixer)
        return;

    _engine->g_pMixer->PlayMusic(*pSound, bLoop ? -1 : 0);
}


