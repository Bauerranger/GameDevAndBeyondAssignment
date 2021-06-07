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

///////////////////////////////////ISystem

bool RenderSystem::DoesEntityMatch(std::shared_ptr<Entity> entity)
{
	if (entity->HasComponent<SpriteComponent>() || entity->HasComponent<TextComponent>() || entity->HasComponent<LevelComponent>())
	{
		return true;
	}
	return false;
}

///////////////////////////////////Render

void RenderSystem::Draw(std::shared_ptr<Window> window)
{
	std::vector<std::shared_ptr<Entity>> copiedEntities = m_Entities;
	for (const std::shared_ptr<Entity>& entity : copiedEntities)
	{
		std::shared_ptr<SpriteComponent> sprite = entity->GetComponent<SpriteComponent>();
		std::shared_ptr<TextComponent> text = entity->GetComponent<TextComponent>();
		std::shared_ptr<LevelComponent> level = entity->GetComponent<LevelComponent>();

		float windowPosition_X = 0;
		float windowPosition_Y = 0;
		window->GetViewPosition(windowPosition_X, windowPosition_Y);

		if (sprite != nullptr)
		{
			// Check if the sprites are in view and only draw them, if they are
			float position_X = 0;
			float position_Y = 0;
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
	}
}