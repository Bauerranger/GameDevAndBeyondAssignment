#include "../Header Files/RenderSystem.h"
#include "../Header Files/SpriteComponent.h"
#include "../Header Files/TextComponent.h"
#include "../Header Files/LevelComponent.h"
#include "../Header Files/TextureManager.h"
#include <SFML/Graphics.hpp>

RenderSystem::RenderSystem()
= default;

RenderSystem::~RenderSystem()
= default;

bool RenderSystem::DoesEntityMatch(std::shared_ptr<Entity> entity)
{
	if (entity->HasComponent<SpriteComponent>() || entity->HasComponent<TextComponent>() || entity->HasComponent<LevelComponent>())
	{
		return true;
	}
	return false;
}

void RenderSystem::Draw(std::shared_ptr<Window> window)
{
	std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
	for (const std::shared_ptr<Entity>& entity : copiedEntities)
	{
		std::shared_ptr<SpriteComponent> sprite = entity->GetComponent<SpriteComponent>();
		std::shared_ptr<TextComponent> text = entity->GetComponent<TextComponent>();
		std::shared_ptr<LevelComponent> level = entity->GetComponent<LevelComponent>();
		float position_X = 0;
		float position_Y = 0;
		float windowPosition_X = 0;
		float windowPosition_Y = 0;
		window->GetViewPosition(windowPosition_X, windowPosition_Y);
		if (sprite != nullptr)
		{

			// Check if the sprites are in view and only draw them, if they are
			sprite->GetPosition(position_X, position_Y);
			if (windowPosition_X - 50 < position_X && windowPosition_X + 1650 > position_X && windowPosition_Y - 50 < position_Y && windowPosition_Y + 950 > position_Y)
			{
				bool visible = false;
				sprite->GetVisible(visible);
				if (visible)
				{
					window->Draw(sprite->GetSprite());
				}
			}
		}
		if (text != nullptr)
		{
			window->Draw(text->GetText());
		}
		if (level != nullptr)
		{
			const unsigned int size = level->GetTotalTileAmount();
			unsigned int tileWidth = 0;
			unsigned int tileHeight = 0;
			level->GetTileSizes(tileWidth, tileHeight);

			sf::Texture* texture = TextureManager::GetInstance().GetTexture(level->GetTextureUrl());
			unsigned int textureX = texture->getSize().x;
			unsigned int textureY = texture->getSize().y;
			int XTiles = std::floor(textureX / tileWidth);
			int YTiles = std::floor(textureY / tileHeight);


			sf::VertexArray quad(sf::Quads, 4);
			for (int i = 0; i < size; ++i)
			{
				const Tile& tile = level->GetTileSprite(i);
				quad[0].position = sf::Vector2f(tile.X, tile.Y);
				quad[1].position = sf::Vector2f(tile.X + tileWidth, tile.Y);
				quad[2].position = sf::Vector2f(tile.X + tileWidth, tile.Y + tileHeight);
				quad[3].position = sf::Vector2f(tile.X, tile.Y + tileHeight);

				int X = tile.textureX;
				int Y = tile.textureY;
				quad[0].texCoords = sf::Vector2f(X, Y);
				quad[1].texCoords = sf::Vector2f(X + tileWidth, Y);
				quad[2].texCoords = sf::Vector2f(X + tileWidth, Y + tileHeight);
				quad[3].texCoords = sf::Vector2f(X, Y + tileHeight);

				// Check if the tiles are in view and only draw them, if they are
				if (windowPosition_X - 50 < tile.X && windowPosition_X + 1650 > tile.X && windowPosition_Y - 50 < tile.Y && windowPosition_Y + 950 > tile.Y)
				{
					window->Draw(&quad, texture);
				}
			}
		}
	}
}

