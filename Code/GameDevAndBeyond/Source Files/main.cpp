#include <SFMLEngine/Header Files/Engine.h>
#include <SFMLEngine/Header Files/SpriteComponent.h>
#include <SFMLEngine/Header Files/AudioSystem.h>
#include "../Header Files/AudioComponent.h"
#include "../Header Files/PhysicSystem.h"
#include "../Header Files/BlockSystem.h"
#include "../Header Files/UISystem.h"
#include "../Header Files/MapSystem.h"


int main()
{
	// Engine initialization with window settings
	int width = 1920;
	int height = 1080;
	std::shared_ptr<Engine> engine = std::make_shared<Engine>(width, height, "SFML-Tetris", false);

	// Initialize game play systems
	std::shared_ptr<PhysicSystem> physicSystem = std::make_shared<PhysicSystem>();
	engine->AddSystem(physicSystem, eThreadImportance::direct);

	std::shared_ptr<BlockSystem> blockSystem = std::make_shared<BlockSystem>();
	engine->AddSystem(blockSystem, eThreadImportance::direct);

	std::shared_ptr<MapSystem> mapSystem = std::make_shared<MapSystem>();
	engine->AddSystem(mapSystem, eThreadImportance::direct);

	// Initialize less important systems
	std::shared_ptr<UISystem> uiSystem = std::make_shared<UISystem>();
	engine->AddSystem(uiSystem, eThreadImportance::worker);

	std::shared_ptr<AudioSystem> audioSystem = std::make_shared<AudioSystem>();
	engine->AddSystem(audioSystem, eThreadImportance::worker);
	
	// Start the engine!
	while (engine->IsRunning())
	{
		engine->Update();
	}

	return 0;
}