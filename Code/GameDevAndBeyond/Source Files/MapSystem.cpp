#include "../Header Files/MapSystem.h"
#include "SFMLEngine/Header Files/Engine.h"
#include "SFMLEngine/Header Files/LevelComponent.h"
#include "SFMLEngine/Header Files/SpriteComponent.h"
#include "SFMLEngine/Header Files/TextComponent.h"
#include "SFMLEngine/Header Files/EventManager.h"
//#include "PlayerComponent.h"
//#include "PhysicComponent.h"
//#include "EnemyComponent.h"
//#include "IOComponent.h"
//#include "ItemComponent.h"
//#include "ScoreEvent.h"
//#include "ScoreComponent.h"
//#include "HealthComponent.h"
//#include "CollisionFlags.h"

MapSystem::MapSystem()
{

}

MapSystem::~MapSystem()
{

}

bool MapSystem::DoesEntityMatch(std::shared_ptr<Entity> entity)
{
	//for reload, we save everything that has a physicsComponent
	if (entity->HasComponent<SpriteComponent>() || entity->HasComponent<TextComponent>())
	{
		return true;
	}
	return false;
}

void MapSystem::Update(Engine* engine, float dt)
{
	bool keyPress = engine->IsKeyPressed(Key::F1);
	if (!m_ReloadKeyPress && keyPress)
	{
		// TODO: Make Reload
		//Reload(engine, "levels/glt_level.tmx");
		m_ReloadKeyPress = true;
	}
	else if (m_ReloadKeyPress && !keyPress)
	{
		m_ReloadKeyPress = false;
	}
}

void MapSystem::Init(Engine* engine)
{
	m_LevelEntity = std::make_shared<Entity>();
	std::shared_ptr<LevelComponent> levelComp = m_LevelEntity->AddComponent<LevelComponent>();
	engine->AddEntity(m_LevelEntity);

	// TODO: Load Level
	//LoadLevel(engine, "levels/level2.tmx");
}

// TODO: Make custom tile and return it
const tmx::Tileset::Tile* MapSystem::GetTile(tmx::Map& map, int tileID)
{
	for (const tmx::Tileset& tileset : map.getTilesets())
	{
		if (tileset.hasTile(tileID))
		{
			return tileset.getTile(tileID);
		}
	}
	return nullptr;
}

void MapSystem::Reload(Engine* engine, std::string level)
{
	while (m_Entities.size() != 0)
	{
		engine->RemoveEntity(m_Entities[0]);
	}
	LoadLevel(engine, level);
}

// TODO: Make load from custom tile
// tmx::Map has to be replaced with the actual map layout from the json/xml file
void MapSystem::LoadLevel(Engine* engine, std::string level)
{
	// Make map variable
	tmx::Map map;
	// Load Map data (serialization)
	map.load(level);
	// Set tileset has to be reworked, so it can be used with my way
	SetTileset(engine, map);

	// Layers are for interactables etc. Can be used to identify items and drawing order
	const std::vector<tmx::Layer::Ptr>& layers = map.getLayers();
	// Save size of the map to the computer
	m_MapWidth = map.getTileCount().x;
	m_MapHeight = map.getTileCount().y;

	// Adds tiles and objects etc.
	for (int i = 0; i < layers.size(); ++i)
	{
		if (layers[i]->getType() == tmx::Layer::Type::Tile)
		{
			const tmx::TileLayer* layer = dynamic_cast<const tmx::TileLayer*>(layers[i].get());
			AddTiles(layer, map);
		}
		else if (layers[i]->getType() == tmx::Layer::Type::Object)
		{
			const tmx::ObjectGroup& objects = *dynamic_cast<const tmx::ObjectGroup*>(layers[i].get());
			for (const tmx::Object& object : objects.getObjects())
			{
				const tmx::Tileset::Tile* tile = GetTile(map, object.getTileID());
				if (tile->type == "Player")
				{
					AddPlayer(engine, object, tile);
				}
				else if (tile->type == "Enemy")
				{
					AddEnemy(engine, object, tile);
				}
				else if (tile->type == "Item")
				{
					AddItem(engine, object, tile);
				}
			}

		}
	}
	// TODO: Make LoadUI its own system
	LoadUI(engine);
}

void MapSystem::LoadUI(Engine* engine)
{
	// TODO: make UI work and rethink it to be its own system
	/*std::shared_ptr<Entity> iOEntity = std::make_shared<Entity>();
	std::shared_ptr<TextComponent> iOText = iOEntity->AddComponent<TextComponent>();
	std::shared_ptr<IOComponent> iOComp = iOEntity->AddComponent<IOComponent>();

	iOText->SetFont("Hyperspace.otf");
	iOText->SetColor(255, 255, 255, 255);
	iOText->SetPosition(10.0f, 10.0f);
	iOText->SetSize(24);

	engine->AddEntity(iOEntity);

	std::shared_ptr<Entity> scoreEntity = std::make_shared<Entity>();
	std::shared_ptr<TextComponent> scoreText = scoreEntity->AddComponent<TextComponent>();
	std::shared_ptr<ScoreComponent> scoreComp = scoreEntity->AddComponent<ScoreComponent>();

	scoreText->SetFont("Hyperspace.otf");
	scoreText->SetColor(255, 255, 255, 255);
	scoreText->SetPosition(10.0f, 10.0f);
	scoreText->SetSize(24);

	engine->AddEntity(scoreEntity);

	std::shared_ptr<Entity> healthEntity = std::make_shared<Entity>();
	std::shared_ptr<TextComponent> healthText = healthEntity->AddComponent<TextComponent>();
	std::shared_ptr<HealthComponent> healthComp = healthEntity->AddComponent<HealthComponent>();

	healthText->SetFont("Hyperspace.otf");
	healthText->SetColor(255, 255, 255, 255);
	healthText->SetPosition(10.0f, 35.0f);
	healthText->SetSize(24);

	engine->AddEntity(healthEntity);*/
}

void MapSystem::SetTileset(Engine* engine, tmx::Map& map)
{
	std::shared_ptr<LevelComponent> levelComp = m_LevelEntity->GetComponent<LevelComponent>();
	for (const tmx::Tileset& tileset : map.getTilesets())
	{
		// Check if data is part of a tile
		bool isTileMap = false;
		for (const tmx::Property& property : tileset.getProperties())
		{
			if (property.getName() == "IsTileMap")
			{
				isTileMap = true;
				break;
			}
		}

		// create tile
		if (isTileMap)
		{
			m_TileWidth = tileset.getTileSize().x;
			m_TileHeight = tileset.getTileSize().y;
			levelComp->CreateTileset(tileset.getImagePath(), m_TileWidth, m_TileHeight);
			break;
		}
	}
}

void MapSystem::AddTiles(const tmx::TileLayer* layer, tmx::Map& map)
{
	std::vector<Tile> gameTiles;
	std::shared_ptr<LevelComponent> levelComp = m_LevelEntity->AddComponent<LevelComponent>();
	const std::vector<tmx::TileLayer::Tile>& layerTiles = layer->getTiles();
	for (int j = 0; j < layerTiles.size(); ++j)
	{
		const tmx::TileLayer::Tile& layerTile = layerTiles[j];
		const tmx::Tileset::Tile* tile = GetTile(map, layerTile.ID);

		Tile gameTile;
		gameTile.X = (j % m_MapWidth) * m_TileWidth;
		gameTile.Y = std::floor(j / m_MapHeight) * m_TileHeight;
		gameTile.textureX = tile->imagePosition.x;
		gameTile.textureY = tile->imagePosition.y;
		for (const tmx::Property& property : tile->properties)
		{
			if (property.getName() == "Collidable")
			{
				gameTile.IsCollidable = property.getBoolValue();
			}
		}
		gameTiles.push_back(gameTile);
	}

	levelComp->SetLevel(gameTiles, m_MapWidth, m_MapHeight);
}

// TODO: Make player
//void MapSystem::AddPlayer(Engine* engine, const tmx::Object& object, const tmx::Tileset::Tile* tile)
//{
//	std::shared_ptr<Entity> playerEntity = std::make_shared<Entity>();
//	std::shared_ptr<SpriteComponent> playerSprite = playerEntity->AddComponent<SpriteComponent>();
//	std::shared_ptr<PhysicComponent> playerPhysics = playerEntity->AddComponent<PhysicComponent>();
//	std::shared_ptr<PlayerComponent> playerComp = playerEntity->AddComponent<PlayerComponent>();
//	playerSprite->CreateSprite(tile->imagePath);
//	playerSprite->SetPosition(object.getPosition().x, object.getPosition().y);
//
//	playerPhysics->SetVelocity(0.f, 0.0f);
//	playerPhysics->SetDamping(0.1f);
//	playerPhysics->SetTargetFlag(EnemyCollisionFlag | ItemCollisionFlag);
//
//	for (const tmx::Property& property : tile->properties)
//	{
//		if (property.getName() == "SpeedPerSecond")
//		{
//			playerComp->SetSpeedPerSecond(property.getFloatValue());
//		}
//		if (property.getName() == "StartHP")
//		{
//			playerComp->SetHP(property.getIntValue());
//		}
//	}
//	engine->AddEntity(playerEntity);
//}

// TODO: Make enemy
//void MapSystem::AddEnemy(Engine* engine, const tmx::Object& object, const tmx::Tileset::Tile* tile)
//{
//	std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//	std::shared_ptr<SpriteComponent> spriteComp = entity->AddComponent<SpriteComponent>();
//	std::shared_ptr<PhysicComponent> physicComp = entity->AddComponent<PhysicComponent>();
//	std::shared_ptr<EnemyComponent> enemyComp = entity->AddComponent<EnemyComponent>();
//	spriteComp->CreateSprite(tile->imagePath);
//	spriteComp->SetPosition(object.getPosition().x, object.getPosition().y);
//
//	physicComp->SetVelocity(0.f, 0.0f);
//	physicComp->SetDamping(0.1f);
//	physicComp->SetCollisionFlag(EnemyCollisionFlag);
//	physicComp->SetTargetFlag(ItemCollisionFlag);
//	for (const tmx::Property& property : tile->properties)
//	{
//		if (property.getName() == "SpeedPerSecond")
//		{
//			enemyComp->SetSpeedPerSecond(property.getFloatValue());
//		}
//	}
//	engine->AddEntity(entity);
//}

// TODO: Make item
//void MapSystem::AddItem(Engine* engine, const tmx::Object& object, const tmx::Tileset::Tile* tile)
//{
//	std::shared_ptr<Entity> entity = std::make_shared<Entity>();
//	std::shared_ptr<SpriteComponent> spriteComp = entity->AddComponent<SpriteComponent>();
//	std::shared_ptr<PhysicComponent> physicComp = entity->AddComponent<PhysicComponent>();
//	std::shared_ptr<ItemComponent> itemComp = entity->AddComponent<ItemComponent>();
//	spriteComp->CreateSprite(tile->imagePath);
//	spriteComp->SetPosition(object.getPosition().x, object.getPosition().y);
//	physicComp->SetCollisionFlag(ItemCollisionFlag);
//	engine->AddEntity(entity);
//}
