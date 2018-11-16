cmake_minimum_required(VERSION 3.10)
project(Memegeon)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

FIND_PACKAGE(SDL2PP REQUIRED)

INCLUDE_DIRECTORIES(${SDL2PP_INCLUDE_DIRS})


SET(SDL2PP_WITH_IMAGE ON)
SET(SDL2PP_WITH_TTF ON)
SET(SDL2PP_WITH_MIXER ON)
ADD_SUBDIRECTORY(extlib/libSDL2pp)


add_executable(Memegeon / Engine/Entity.hpp Engine/Entity.cpp
        Engine/Engine.hpp Engine/Engine.cpp
        Engine/Component.hpp Game/Entities/Player.hpp
        Engine/Singleton.hpp Engine/Vector2.hpp
        Game/Components/ImageRenderer.hpp Game/Components/ImageRenderer.cpp
        Game/Components/AnimatedRenderer.cpp Game/Components/AnimatedRenderer.hpp
        Game/Components/Collider.cpp Game/Components/Collider.hpp
        Game/Components/PlayerController.cpp Game/Components/PlayerController.hpp
        Engine/InputManager.cpp Engine/InputManager.hpp
        Game/Entities/Player.cpp
        Game/Components/Editor.hpp Game/Components/Editor.cpp
        Engine/UI.cpp Engine/UI.hpp Engine/Camera.hpp Engine/Camera.cpp Tools.hpp
        Game/Background.hpp Game/Background.cpp Game/Components/DoorManager.cpp Game/Components/DoorManager.hpp Engine/Vector2.cpp
        Game/Components/LevelEditor/LevelEditor.cpp Game/Components/LevelEditor/LevelEditor.hpp
        Game/Components/LevelEditor/EditorMode.hpp Game/Components/LevelEditor/Modes/SelectionTool.hpp Game/Components/LevelEditor/Modes/SelectionTool.cpp
        Game/Components/LevelEditor/Modes/CreationTool.cpp Game/Components/LevelEditor/Modes/CreationTool.hpp Game/Components/LevelEditor/Modes/PickerTool.cpp
        Game/Components/LevelEditor/Modes/PickerTool.hpp Game/Components/LevelEditor/Modes/ColliderTool.cpp Game/Components/LevelEditor/Modes/ColliderTool.hpp
        Game/Components/LevelEditor/Modes/EraserTool.cpp Game/Components/LevelEditor/Modes/EraserTool.hpp Game/Components/LevelEditor/Modes/DoorTool.cpp
        Game/Components/LevelEditor/Modes/DoorTool.hpp Game/Entities/GameManager.hpp Game/Entities/GameManager.cpp Engine/Button.cpp Engine/Button.hpp
        Game/Components/LevelEditor/Modes/SpikeTool.cpp Game/Components/LevelEditor/Modes/SpikeTool.hpp Game/Entities/Spike.cpp Game/Entities/Spike.hpp
        Game/Entities/Key.hpp Game/Components/LevelEditor/Modes/KeyTool.cpp Game/Components/LevelEditor/Modes/KeyTool.hpp Game/Level.hpp Game/Components/MainMenu.cpp
        Game/Components/MainMenu.hpp Game/Level.cpp Game/Entities/Door.hpp Game/Entities/Key.cpp Game/Entities/Door.cpp Game/Entities/TutKey.hpp Game/Components/LevelEditor/Modes/CustomImagePlacer.hpp
        Game/Components/LevelEditor/Modes/CustomImagePlacer.cpp Game/Components/LevelEditor/Modes/SwitchTool.cpp Game/Components/LevelEditor/Modes/SwitchTool.hpp Game/Entities/TutKey.cpp
        Game/Entities/Switch.hpp Game/Entities/Switch.cpp imgui_sdl.h imgui_sdl.cpp
        imgui_sdl.cpp imgui_sdl.h imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_demo.cpp imgui/imconfig.h imgui/imgui_internal.h imgui/imgui_widgets.cpp imgui/examples/imgui_impl_sdl.h imgui/examples/imgui_impl_sdl.cpp)

TARGET_LINK_LIBRARIES(Memegeon ${COMMON_LIBRARIES} ${SDL2PP_LIBRARIES})