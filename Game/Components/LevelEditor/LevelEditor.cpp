#include "LevelEditor.hpp"
#include "../../../Engine/InputManager.hpp"
#include <SDL2/SDL_keycode.h>
#include "EditorMode.hpp"
#include "../../../Engine/UI.hpp"
#include "../PlayerController.hpp"
#include "../../../Engine/Engine.hpp"
#include "../ImageRenderer.hpp"
#include <sstream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include "../../../Engine/Camera.hpp"
#include "../../../Engine/Button.hpp"
#include "../../../imgui/imgui.h"
#include "../../Entities/Spike.hpp"
#include "../../Entities/TutKey.hpp"
#include "../../Entities/Switch.hpp"
static std::vector<std::string> savedMaps{};


void LevelEditor::Update(const float &deltaTime) {

    if (InputManager::KeyPressed(SDLK_o)) {
        SaveMap("uTut");
        return;
    }

  if(InputManager::KeyDown(SDLK_TAB)){
      CycleEditorModes();
    }

  GetActiveMode()->OnUpdate();

    UpdateCamera(deltaTime);
}

void LevelEditor::Render() {

    ImGui::SetNextWindowSize(ImVec2(300, 300), 1 << 2);
    ImGui::Begin("Level Settings");

    if (ImGui::BeginCombo("Current Mode", GetActiveMode()->modeName.c_str(), ImGuiComboFlags_None)) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < editorModes.size(); n++)
        {

            bool is_selected = (n == activeMode);
            if (ImGui::Selectable(editorModes[modeIndex[n]]->modeName.c_str(), is_selected))
                SetEditorMode(editorModes[modeIndex[n]]->modeName);
            if (is_selected)
                ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
        }
        ImGui::EndCombo();
    }


    ImGui::Text(("Map "+ currentLevelName).c_str());

    if(currentLevelName != "")
    {
        if(ImGui::Button(currentLevelName.c_str()))
            SaveMap(currentLevelName.c_str());
    }


    if(ImGui::CollapsingHeader("Maps")) {

        for(auto szMap : savedMaps)

            if(ImGui::Button(szMap.c_str())) {

                    this->colliderList.clear();
                    this->doorPosition = {0, 0};
                    this->keyPosition = {0, 0};

                    for (auto ent : worldTileList)
                        ent->bDestroy = true;
                    worldTileList.clear();

                    for (auto ent : worldSpikes)
                        ent->bDestroy = true;
                    worldSpikes.clear();

                    for (auto ent : allSwitches)
                        ent->bDestroy = true;
                    allSwitches.clear();

                    for (auto ent : mKeys)
                        ent->bDestroy = true;
                    mKeys.clear();

                LoadMap(szMap);

            }
    }


    ImGui::Checkbox("Colldiers", &bAlwaysRenderColliders);

    GetActiveMode()->OnRender();

    ImGui::End();

    if(ImGui::BeginMainMenuBar()) {
        for (auto md : editorModes) {
            if (ImGui::Button(md.first.c_str())) {
                SetEditorMode(md.first);
                //ImGui::EndMenu();
            }
        }
        ImGui::EndMainMenuBar();
    }


    UI::DrawString(modeIndex[activeMode].c_str(), Camera::Instance().viewport.w/2, 50, 3, {0,255,99});

    RenderColliders();

    for (auto md : editorModes)
        md.second->ForceRender();
}


#include "Modes/SelectionTool.hpp"
#include "Modes/CreationTool.hpp"
#include "Modes/PickerTool.hpp"
#include "Modes/ColliderTool.hpp"
#include "Modes/EraserTool.hpp"
#include "Modes/DoorTool.hpp"
#include "Modes/SpikeTool.hpp"
#include "Modes/KeyTool.hpp"
#include "Modes/CustomImagePlacer.hpp"
#include "Modes/SwitchTool.hpp"
#include "../../../imgui/imgui.h"
void LevelEditor::Start() {

    pTileMap = Engine::LoadTexture("assets/duntiles.png");;

    editorModes["selection"] = new SelectionTool(this);
    editorModes["picker"] = new PickerTool(this);
    editorModes["creation"] = new CreationTool(this);
    editorModes["collider"] = new ColliderTool(this);
    editorModes["eraser"] = new EraserTool(this);
    editorModes["door"] = new DoorTool(this);
    editorModes["spikes"] = new SpikeTool(this);
    editorModes["key"] = new KeyTool(this);
    editorModes["tut"] = new CustomImagePlacer(this);
    editorModes["switch"] = new SwitchTool(this);
    for(auto md : editorModes){md.second->modeName =md.first; }

    std::ifstream fileStream("alllevels");
    std::string lBuff;
    while (std::getline(fileStream, lBuff)) {
        savedMaps.push_back(lBuff);
    }
    fileStream.close();



    int id = 0;
    for(auto md : editorModes)
        modeIndex.insert({id++, md.first});


    GetActiveMode()->OnActivate();
}

void LevelEditor::ForceUpdate() {

    if(InputManager::KeyPressed(SDLK_ESCAPE))
    {
      //  bEnabled = !bEnabled;
    //    entity->GetComponent<PlayerController>()->bEnabled = !entity->GetComponent<PlayerController>()->bEnabled;
    }
}

void LevelEditor::SetEditorMode(const std::string& mode) {

    if(editorModes.count(mode) > 0){
      GetActiveMode()->OnDisable();
      activeMode = GetIndexFromString(mode);
      editorModes[mode]->OnActivate();
    }
}

void LevelEditor::CycleEditorModes() {

    GetActiveMode()->OnDisable();

  if(activeMode >= editorModes.size() - 1)
    activeMode = 0;
  else
    activeMode++;

  GetActiveMode()->OnActivate();

}

int LevelEditor::GetIndexFromString(const std::string& szMode) const{

  int index = 0;
  for(auto mode : editorModes){

    if(mode.first == szMode)
      return index;
    index++;
  }

    return -1;
}

EditorMode* LevelEditor::GetActiveMode() {
  return editorModes[modeIndex[activeMode]];
}

void LevelEditor::OnDestroy() {

}

void LevelEditor::SaveMap(const std::string& szFileName) {


    std::ifstream fileStream("alllevels");
    std::string lBuff;
    bool exists = false;
    while(std::getline(fileStream, lBuff))
    {
        if(lBuff == szFileName)
        {
            exists = true;
            break;
        }
    }
    fileStream.close();

    if(!exists){
        std::ofstream allLevels("alllevels", std::ios_base::app | std::ofstream::out);
        allLevels << szFileName << std::endl;
        allLevels.close();
    }



    int nError = 0;
#if defined(_WIN32)
    nError = _mkdir(szFileName.c_str()); // can be used on Windows
#else
    nError = mkdir(szFileName.c_str(),0733); // can be used on non-Windows
#endif

     std::string dir = szFileName + "/map";

    std::ofstream fSaveMap((dir + ".mdata").c_str(),  std::ofstream::out);

    for(Empty* t : worldTileList) {

        int x = t->GetComponent<ImageRenderer>()->srcrect.x;
        int y = t->GetComponent<ImageRenderer>()->srcrect.y;

        fSaveMap << (int)ceil(t->position.x) << ' ' << (int)ceil(t->position.y) << ' ' << x << ' ' << y << '\n';
    }
    std::cout << "Finished Saving Map: " << worldTileList.size() << std::endl;
    fSaveMap.close();

    for(auto mode : editorModes)
        mode.second->OnSave(dir);

    std::cout << "Map saved\n";

}

void LevelEditor::LoadMap(const std::string& mapNme){

    currentLevelName = mapNme;

    std::string dir = mapNme + "/map";
    std::ifstream fileStream(dir + ".mdata");

    std::string lBuff;
    while(std::getline(fileStream, lBuff))
    {
        std::stringstream line(lBuff);
        std::string buff;

        int data[4] = {0,0,0,0};

        for (int i = 0; i < 4; i++) {
            line >> buff;
            data[i] = std::stoi(buff);
        }

        int x = data[0];
        int y = data[1];

        int sx = data[2];
        int sy = data[3];

        auto pEnt = new Empty({(float)x,(float)y});
        pEnt->scale = 2.0f;
        auto ir = pEnt->AddComponent(ImageRenderer(Engine::LoadTexture("assets/duntiles.png"), {sx, sy, 32,32}));
        worldTileList.push_back(pEnt);
    }

    fileStream.close();

    for(auto mode : editorModes)
        mode.second->OnLoad(dir);


    std::cout << "Finished loading map: " << currentLevelName <<  std::endl;

    std::cout << "Loaded : " << colliderList.size() <<  std::endl;
}

void LevelEditor::UpdateCamera(const float &deltaTime) {

    static float offsetx = 0;
    static float offsety = 0;

    if(InputManager::KeyHeld(SDLK_RIGHT))
        offsetx+= 128 * deltaTime;

    if(InputManager::KeyHeld(SDLK_LEFT))
        offsetx-= 128 * deltaTime;

    if(InputManager::KeyHeld(SDLK_UP))
        offsety-=128 * deltaTime;

    if(InputManager::KeyHeld(SDLK_DOWN))
        offsety+=128 * deltaTime;

    Camera::Instance().viewport.x = static_cast<int>(offsetx/64) * 64;
    Camera::Instance().viewport.y = static_cast<int>(offsety/64) * 64;
}



void LevelEditor::RemoveTile(Empty *pEntity) {

        auto it = worldTileList.begin();

        while(it != worldTileList.end()){

            if((*it) == pEntity)
            {
                (*it)->bDestroy = true;
                it = worldTileList.erase(it);
            }
            else
                ++it;
        }
}

Empty *LevelEditor::GetEntity(int x, int y) {
    for (auto it = worldTileList.begin(); it != worldTileList.end(); ++it) {

        if ((*it)->position.x == x && (*it)->position.y == y) {
            return *it;
        }
    }
    return nullptr;

}

void LevelEditor::RenderColliders() {

    bool bactive = GetActiveMode()->modeName == "collider";

    if(bactive|| bAlwaysRenderColliders)
    {
        for(auto col : colliderList){

            auto sp = (Camera::Instance().WorldToScreenPoint(col.x, col.y));
               auto wPos = Camera::Instance().ScreenToWorldPoint(InputManager::GetMousePos());
                if (bactive && col.Contains((int) wPos.x, (int) wPos.y))
                    UI::DrawRect({sp.first, sp.second, col.w, col.h}, {255, 0, 0}, 2);
                else
                UI::DrawRect({sp.first, sp.second, col.w, col.h}, {255,0,255}, 2);
        }
    }


}
