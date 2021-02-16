#pragma once

#include "SFMLEngine/Header Files/ISystem.h"

#include <string>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/ObjectGroup.hpp>

struct Tile
{
	sf::Uint8 x;
	sf::Uint8 y;
	sf::Uint8 tileID;
};

struct TileConfig
{
	sf::Uint8 tileID;
	sf::Uint8 movementCost;
	sf::Uint8 layer;
	std::string filePath;
};


class MapSystem : public ISystem
{
public:
	MapSystem();
	~MapSystem();
public:
	//ISystem
	virtual bool DoesEntityMatch(std::shared_ptr<Entity> entity) override;
	virtual void Update(Engine* engine, float dt) override;
	virtual void Init(Engine* engine) override;
private:
	const tmx::Tileset::Tile* GetTile(tmx::Map& map, int tileID);
	void Reload(Engine* engine, std::string level);
	void LoadLevel(Engine* engine, std::string level);
	void LoadUI(Engine* engine);
	void SetTileset(Engine* engine, tmx::Map& map);
	void AddTiles(const tmx::TileLayer* layer, tmx::Map& map);
	//void AddPlayer(Engine* engine, const tmx::Object& object, const tmx::Tileset::Tile* tile);
	//void AddEnemy(Engine* engine, const tmx::Object& object, const tmx::Tileset::Tile* tile);
	//void AddItem(Engine* engine, const tmx::Object& object, const tmx::Tileset::Tile* tile);
private:
	std::shared_ptr<Entity> m_LevelEntity = nullptr;
	unsigned int m_MapWidth = 0;
	unsigned int m_MapHeight = 0;
	unsigned int m_TileWidth = 0;
	unsigned int m_TileHeight = 0;

	bool m_ReloadKeyPress = false;
};