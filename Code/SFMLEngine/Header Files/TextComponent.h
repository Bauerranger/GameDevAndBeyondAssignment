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

	///////////////////////////////////Utility

	const sf::Text* GetText() { return m_Text; }
	void CenterText();

	///////////////////////////////////Setter

	void SetFont(const std::string font);
	void SetText(const std::string text);
	void SetSize(const int size);
	void SetColor(const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha = 255);
	void SetPosition(const float X, const float Y);
	void SetVisibleState(eGameState state) { m_VisibleState = state; }
	void SetVisibilityOnStateChange();

private:
	sf::Text* m_Text = nullptr;
	sf::Font* m_Font = nullptr;
	eGameState m_VisibleState = eGameState::start;
	sf::Color m_Color;
};