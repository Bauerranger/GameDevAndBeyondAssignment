#pragma once
#include <memory>
#include <string>
#include "IComponent.h"

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
	void CreateSprite(std::string url);
	//need to return the Sprite here so that Window can render it
	const sf::Sprite* GetSprite() { return m_Sprite; }
	//Set the Position of the Sprite
	void SetPosition(const float X, const float Y);
	//Rotate a Sprite to a specific Rotation
	void SetRotation(const float RotationInDegree);
	//Return the Position
	void GetPosition(float& X, float& Y);
	//Return the Rotation
	void GetRotation(float& RotationInDegree);
	//Return the Size
	void GetSize(unsigned int& Width, unsigned int& Height);
	//Set whether the sprite can be seen or not
	void SetVisible(bool visible);
	//Get whether the sprite can be seen or not
	void GetVisible(bool& visible);

	//check if the Sprite is Colliding with another Sprite
	const bool IsCollidingWith(std::shared_ptr<SpriteComponent> otherSprite) const;
	//check if the Sprite is Colliding with a fixed Rectangle
	const bool IsCollidingWith(const float left, const float top, const float width, const float height) const;

private:
	sf::Sprite* m_Sprite = nullptr;
	bool m_Visible = true;

};