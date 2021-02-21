#include "../Header Files/SpriteComponent.h"
#include "../Header Files/TextureManager.h"
#include <SFML/Graphics.hpp>

SpriteComponent::SpriteComponent()
= default;

SpriteComponent::~SpriteComponent()
{
	delete m_Sprite;
}

void SpriteComponent::CreateSprite(std::string url)
{
	//dereference the pointer, so that we can access it via &
	sf::Texture* texture = TextureManager::GetInstance().GetTexture(url);
	m_Sprite = new sf::Sprite(*texture);

	unsigned int width = 0;
	unsigned int height = 0;

	sf::Vector2f origin;
	origin.x = texture->getSize().x / 2;
	origin.y = texture->getSize().y / 2;
	m_Sprite->setOrigin(origin);
}

void SpriteComponent::SetPosition(const float X, const float Y)
{
	m_Sprite->setPosition(X, Y);
}

void SpriteComponent::SetRotation(const float RotationInDegree)
{
	m_Sprite->setRotation(RotationInDegree);
}

void SpriteComponent::GetPosition(float& X, float& Y)
{
	X = m_Sprite->getPosition().x;
	Y = m_Sprite->getPosition().y;
}

void SpriteComponent::GetRotation(float& RotationInDegree)
{
	RotationInDegree = m_Sprite->getRotation();
}

void SpriteComponent::GetSize(unsigned int& Width, unsigned int& Height)
{
	const sf::Texture* texture = m_Sprite->getTexture();
	Width = texture->getSize().x;
	Height = texture->getSize().y;
}

void SpriteComponent::SetVisible(bool visible)
{
	m_Visible = visible;
}

void SpriteComponent::GetVisible(bool& visible)
{
	visible = m_Visible;
}

void SpriteComponent::SetColor(const int blockKey)
{
	switch (blockKey)
	{
	case 0:
		m_Sprite->setColor(sf::Color::Color(255, 255, 255, 255));
		break;
	case 1:
		m_Sprite->setColor(sf::Color::Color(50, 124, 240, 255));
		break;
	case 2:
		m_Sprite->setColor(sf::Color::Color(240, 84, 62, 255));
		break;
	case 3:
		m_Sprite->setColor(sf::Color::Color(38, 223, 240, 255));
		break;
	case 4:
		m_Sprite->setColor(sf::Color::Color(240, 165, 14, 255));
		break;
	case 5:
		m_Sprite->setColor(sf::Color::Color(26, 240, 138, 255));
		break;
	case 6:
		m_Sprite->setColor(sf::Color::Color(150, 150, 150, 255));
		break;
	default:
		break;
	}
}

const bool SpriteComponent::IsCollidingWith(std::shared_ptr<SpriteComponent> otherSprite) const
{
	const sf::Sprite* sfmlSprite = otherSprite->GetSprite();
	sf::FloatRect otherRect = sfmlSprite->getGlobalBounds();
	sf::FloatRect thisRect = m_Sprite->getGlobalBounds();
	return thisRect.intersects(otherRect);
}

const bool SpriteComponent::IsCollidingWith(const float left, const float top, const float width, const float height) const
{
	sf::FloatRect otherRect(left, top, width, height);
	sf::FloatRect thisRect = m_Sprite->getGlobalBounds();
	return thisRect.intersects(otherRect);
}