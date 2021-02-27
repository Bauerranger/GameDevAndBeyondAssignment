#pragma once
#include <string>
#include <vector>
#include "IComponent.h"

struct Tile
{
	float X;
	float Y;
	int textureX;
	int textureY;
	bool IsCollidable;
};

class LevelComponent : public IComponent
{
public:
	LevelComponent();
	~LevelComponent();

public:
	void CreateTileset(std::string url, int tileWidth, int tileHeight);
	void SetLevel(std::vector<Tile>& tiles, int width, int height);

	const Tile& GetTileSprite(const unsigned int index) const;

	const std::string GetTextureUrl() { return m_TextureUrl; };
	const unsigned int GetTotalTileAmount() { return m_Width * m_Height; }
	const void GetTileSizes(unsigned int& tileWidth, unsigned int& tileHeight) { tileWidth = m_TileWidth; tileHeight = m_TileHeight; }
	const void GetMapSize(unsigned int& width, unsigned int& height) { width = m_Width; height = m_Height; }

	//check if the Rectangle is Colliding with a collidable Tile
	const bool IsWalkable(const float left, const float top, const float width, const float height) const;

private:
	const Tile& GetTile(const float X, const float Y) const;

private:
	std::vector<Tile> m_Tiles;
	std::string m_TextureUrl;
	unsigned int m_Width = 0;
	unsigned int m_Height = 0;
	unsigned int m_TileWidth = 0;
	unsigned int m_TileHeight = 0;

};