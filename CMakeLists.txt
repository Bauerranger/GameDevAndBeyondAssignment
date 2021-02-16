cmake_minimum_required(VERSION 3.1)

set(ENGINE_PROJECT_NAME "SFMLEngine")
set(GAME_PROJECT_NAME "GameDevAndBeyond")
set(CMAKE_CXX_STANDARD 17)

IF (WIN32)
	set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/Code/ThirdParty/SFML/lib/cmake/SFML")
ELSE()
	set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/Code/ThirdParty/SFML_GCC/lib/cmake/SFML")
ENDIF()

set(SFML_STATIC_LIBRARIES TRUE)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(nlohmann_json_subDir "Code/ThirdParty/json-3.9.1/")
add_subdirectory(${nlohmann_json_subDir})

set(tinyxml2_subDir "Code/ThirdParty/tinyxml2/")
add_subdirectory(${tinyxml2_subDir})

project(${ENGINE_PROJECT_NAME})
project(${GAME_PROJECT_NAME})

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

include_directories(${CMAKE_CURRENT_LIST_DIR}/Code)

set(ENGINE_SOURCE_PATH Code/${ENGINE_PROJECT_NAME})
set(ENGINE_SOURCE_FILES 
"${ENGINE_SOURCE_PATH}/Header Files/AudioSystem.h"
"${ENGINE_SOURCE_PATH}/Header Files/Engine.h"
"${ENGINE_SOURCE_PATH}/Header Files/Entity.h"
"${ENGINE_SOURCE_PATH}/Header Files/EventManager.h"
"${ENGINE_SOURCE_PATH}/Header Files/IArchive.h"
"${ENGINE_SOURCE_PATH}/Header Files/IComponent.h"
"${ENGINE_SOURCE_PATH}/Header Files/InputHelper.h"
"${ENGINE_SOURCE_PATH}/Header Files/ISystem.h"
"${ENGINE_SOURCE_PATH}/Header Files/JsonArchive.h"
"${ENGINE_SOURCE_PATH}/Header Files/LevelComponent.h"
"${ENGINE_SOURCE_PATH}/Header Files/NetworkingComponent.h"
"${ENGINE_SOURCE_PATH}/Header Files/NetworkingSystem.h"
"${ENGINE_SOURCE_PATH}/Header Files/ObjectInterface.h"
"${ENGINE_SOURCE_PATH}/Header Files/RenderSystem.h"
"${ENGINE_SOURCE_PATH}/Header Files/SpriteComponent.h"
"${ENGINE_SOURCE_PATH}/Header Files/TextComponent.h"
"${ENGINE_SOURCE_PATH}/Header Files/TextureManager.h"
"${ENGINE_SOURCE_PATH}/Header Files/Time.h"
"${ENGINE_SOURCE_PATH}/Header Files/VectorInterface.h"
"${ENGINE_SOURCE_PATH}/Header Files/Window.h"
"${ENGINE_SOURCE_PATH}/Source Files/AudioSystem.cpp"
"${ENGINE_SOURCE_PATH}/Source Files/Engine.cpp"
"${ENGINE_SOURCE_PATH}/Source Files/EventManager.cpp"
"${ENGINE_SOURCE_PATH}/Source Files/InputHelper.cpp"
"${ENGINE_SOURCE_PATH}/Source Files/ISystem.cpp"
"${ENGINE_SOURCE_PATH}/Source Files/JsonArchive.cpp"
"${ENGINE_SOURCE_PATH}/Source Files/LevelComponent.cpp"
"${ENGINE_SOURCE_PATH}/Source Files/NetworkingComponent.cpp"
"${ENGINE_SOURCE_PATH}/Source Files/NetworkingSystem.cpp"
"${ENGINE_SOURCE_PATH}/Source Files/RenderSystem.cpp"
"${ENGINE_SOURCE_PATH}/Source Files/SpriteComponent.cpp"
"${ENGINE_SOURCE_PATH}/Source Files/TextComponent.cpp"
"${ENGINE_SOURCE_PATH}/Source Files/TextureManager.cpp"
"${ENGINE_SOURCE_PATH}/Source Files/Time.cpp"
"${ENGINE_SOURCE_PATH}/Source Files/Window.cpp"
)

set(GAME_SOURCE_PATH Code/${GAME_PROJECT_NAME})
set(GAME_SOURCE_FILES 
"${GAME_SOURCE_PATH}/Header Files/AudioComponent.h"
"${GAME_SOURCE_PATH}/Header Files/Events.h"
"${GAME_SOURCE_PATH}/Header Files/MapSystem.h"
"${GAME_SOURCE_PATH}/Source Files/AudioComponent.cpp"
"${GAME_SOURCE_PATH}/Source Files/main.cpp"
"${GAME_SOURCE_PATH}/Source Files/MapSystem.cpp"
)

add_library(${ENGINE_PROJECT_NAME} ${ENGINE_SOURCE_FILES})
add_executable(${GAME_PROJECT_NAME} ${GAME_SOURCE_FILES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${GAME_PROJECT_NAME})

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/${GAME_SOURCE_PATH} FILES ${GAME_SOURCE_FILES})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/${ENGINE_SOURCE_PATH} FILES ${ENGINE_SOURCE_FILES})

target_link_libraries(${ENGINE_PROJECT_NAME} sfml-graphics sfml-audio sfml-network nlohmann_json::nlohmann_json tinyxml2)
target_link_libraries(${GAME_PROJECT_NAME} ${ENGINE_PROJECT_NAME} )