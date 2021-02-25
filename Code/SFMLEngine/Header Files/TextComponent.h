#pragma once
#include <string>
#include "IComponent.h"
#include <SFML/Graphics.hpp>
#include <SFMLEngine/Header Files/Engine.h>

namespace sf
{
	class Font;
	class Text;
	class String;
}

class TextComponent : public IComponent
{
public:
	TextComponent() = default;
	TextComponent(std::string text);
	~TextComponent();

public:
	// set the Text
	void SetFont(const std::string font);
	// set the Text
	void SetText(const std::string text);
	// Set the CharacterSize
	void SetSize(const int size);
	// Set the Text's Color
	void SetColor(const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha = 255);
	// need to return the Text here so that Window can render it
	const sf::Text* GetText() { return m_Text; }
	// Set the Position of the Text
	void SetPosition(const float X, const float Y);
	// Center text
	void CenterText();
	// Set in which game state the text is visible
	void SetVisibleState(eGameState state) { m_VisibleState = state; }
	// Sets visibility according to game state
	void SetVisibilityOnStateChange();
private:
	sf::Text* m_Text = nullptr;
	sf::Font* m_Font = nullptr;
	eGameState m_VisibleState = eGameState::start;
	sf::Color m_Color;
};