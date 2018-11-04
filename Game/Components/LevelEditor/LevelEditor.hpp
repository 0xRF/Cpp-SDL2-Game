#pragma once

#include "../../../Engine/Component.hpp"
#include <SDL2pp/SDL2pp.hh>
#include <vector>
#include "../../../Engine/Vector2.hpp"

#include <map>

class Switch;
class Empty;
class EditorMode;
class Spike;
class TutKey;
class LevelEditor : public Component<LevelEditor>{

public:

	void Update(const float &deltaTime) override;

	void Render() override;

	void Start() override;

	void ForceUpdate() override;

	void OnDestroy() override;

	std::vector<SDL2pp::Rect> colliderList{};
    std::pair<std::pair<int,int>, std::vector<std::pair<int,int>>> tilesCopyBuffer{};
    std::vector<Spike*> worldSpikes{};

    std::vector<Empty*> worldTileList{};
    std::pair<int,int> doorPosition = {-1,-1};
    std::pair<int,int> keyPosition = {-1,-1};
    std::vector<TutKey*> mKeys{};
    std::vector<Switch*> allSwitches{};


	LevelEditor(){}
	void SetEditorMode(const std::string& mode);
	void CycleEditorModes();

	SDL2pp::Texture* pTileMap = nullptr;

	void RemoveTile(Empty* pEntity);
	Empty* GetEntity(int x, int y);


private: 

	std::map<std::string, EditorMode*> editorModes{};
	std::map<int, std::string> modeIndex{};
	int activeMode = 0;

  int GetIndexFromString(const std::string& szMode) const;

  EditorMode* GetActiveMode();

    void SaveMap(const std::string& szMapDir);

    void LoadMap(const std::string& szMapDir);

    void UpdateCamera(const float& deltaTime);

    bool bEditorMenusClosed = false;

    void RenderColliders();


    bool bAlwaysRenderColliders = false;
	std::string currentLevelName = "";
};

