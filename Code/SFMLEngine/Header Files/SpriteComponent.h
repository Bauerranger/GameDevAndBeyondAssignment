#pragma once
#include <memory>
#include <string>
#include "IComponent.h"

///////////////////////////////////SFML
//forward declaration, so that we don't need to include SFML in Game Project
namespace sf
{
	class Sprite;
	class Texture;
}

class SpriteComponent : public IComponent
{
public:
	SpriteComponent();
	~SpriteComponent();

public:
	///////////////////////////////////Sprite

	void CreateSprite(std::string url);
	const sf::Sprite* GetSprite() { return m_Sprite; }

	///////////////////////////////////Getter

	void SetPosition(const float X, const float Y);
	void SetRotation(const float RotationInDegree);
	void SetVisible(bool visible);
	void SetColor(const int blockKey);

	///////////////////////////////////Setter

	void GetPosition(float& X, float& Y);
	void GetRotation(float& RotationInDegree);
	void GetSize(unsigned int& Width, unsigned int& Height);
	void GetVisible(bool& visible);

	///////////////////////////////////Collision

	const bool IsCollidingWith(std::shared_ptr<SpriteComponent> otherSprite) const;
	const bool IsCollidingWith(const float left, const float top, const float width, const float height) const;

private:
	sf::Sprite* m_Sprite = nullptr;
	bool m_Visible = true;

};