#include "../Header Files/LevelComponent.h"
#include "../Header Files/TextureManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/FileInputStream.hpp>

LevelComponent::LevelComponent()
= default;

LevelComponent::~LevelComponent()
= default;

void LevelComponent::CreateTileset(std::string url, int tileWidth, int tileHeight)
{
	TextureManager::GetInstance().GetTexture(url); //to cache
	m_TextureUrl = url;
	m_TileWidth = tileHeight;
	m_TileHeight = tileHeight;
}

void LevelComponent::SetLevel(std::vector<Tile>& tiles, int width, int height)
{
	m_Tiles = tiles;
	m_Width = width;
	m_Height = height;
}

const Tile& LevelComponent::GetTileSprite(const unsigned int index) const
{
	return m_Tiles[index];
}

const bool LevelComponent::IsWalkable(const float left, const float top, const float width, const float height) const
{
	const Tile& leftTopTile = GetTile(left - width / 2, top - height / 2);
	if (leftTopTile.IsCollidable) return false;
	const Tile& rightTopTile = GetTile(left + width / 2, top - height / 2);
	if (rightTopTile.IsCollidable) return false;
	const Tile& leftBottomTile = GetTile(left - width / 2, top + height / 2);
	if (leftBottomTile.IsCollidable) return false;
	const Tile& rightBottomTile = GetTile(left + width / 2, top + height / 2);
	if (rightBottomTile.IsCollidable) return false;
	return true;
}

const Tile& LevelComponent::GetTile(const float X, const float Y) const
{
	unsigned int tileX = (int)std::floor(X / (float)m_TileWidth);
	unsigned int tileY = (int)std::floor(Y / (float)m_TileHeight);
	if (m_Width < tileX)
	{
		tileX = m_Width - 1;
	}
	if (m_Height < tileY)
	{
		tileY = m_Height - 1;
	}
	int index = tileX + (tileY * m_Width);
	return m_Tiles[index];

}

