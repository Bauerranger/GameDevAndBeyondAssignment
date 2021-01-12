//#include "SFMLEngine/Core/Engine.h"
//#include "Game.h"
//#include <fstream>
//#include <iostream>
//
//#include <nlohmann/json.hpp>
//#include <tinyxml2.h>
//
//#include <future>
//
//int main()
//{
//	std::unique_ptr<Engine> engine = std::make_unique<Engine>();
//	std::unique_ptr<Game> game = std::make_unique<Game>();
//
//	engine->RegisterTickListener(*game);
//	engine->Run(sf::Vector2u(1280, 736), "GameDevAndBeyond");
//
//	return 0;
//}

#include "SFMLEngine/Header Files/Engine.h"
//#include "PhysicSystem.h"
//#include "PlayerSystem.h"
//#include "UISystem.h"
//#include "MapSystem.h"
//#include "EnemySystem.h"
#include "../Header Files/AudioComponent.h"
#include "SFMLEngine/Header Files/AudioSystem.h"
#include <ctime>
#include <algorithm>
#include <vector>

#ifdef _DEBUG
#define EngineMain main
#else
#define EngineMain WinMain
#endif



int EngineMain()
{

	int width = 1600;
	int height = 900;
	std::shared_ptr<Engine> engine = std::make_shared<Engine>(width, height, "Fluffy Bunny Overdrive", false);

	//Normally this all would be defined in data which is somehow loaded during runtime
	/*std::shared_ptr<PhysicSystem> physicSystem = std::make_shared<PhysicSystem>();
	engine->AddSystem(physicSystem);

	std::shared_ptr<PlayerSystem> playerSystem = std::make_shared<PlayerSystem>();
	engine->AddSystem(playerSystem);

	std::shared_ptr<EnemySystem> enemySystem = std::make_shared<EnemySystem>();
	engine->AddSystem(enemySystem);

	std::shared_ptr<UISystem> uiSystem = std::make_shared<UISystem>();
	engine->AddSystem(uiSystem);

	std::shared_ptr<MapSystem> mapSystem = std::make_shared<MapSystem>();
	engine->AddSystem(mapSystem);*/

	std::shared_ptr<AudioSystem> audioSystem = std::make_shared<AudioSystem>();
	engine->AddSystem(audioSystem, eThreadImportance::lazy);

	while (engine->IsRunning())
	{
		engine->Update();
	}

	return 0;
}