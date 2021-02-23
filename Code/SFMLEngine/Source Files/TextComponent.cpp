#include "../Header Files/TextComponent.h"
#include <SFML/Graphics.hpp>

TextComponent::TextComponent(std::string text)
{
	m_Text->setString(text);
}

TextComponent::~TextComponent()
{
	delete m_Font;
	delete m_Text;
}

void TextComponent::SetFont(const std::string font)
{
	m_Font = new sf::Font();
	m_Font->loadFromFile(font);
	m_Text = new sf::Text();
	m_Text->setFont(*m_Font);
}

void TextComponent::SetText(const std::string text)
{
	m_Text->setString(text);
}

void TextComponent::SetSize(const int size)
{
	m_Text->setCharacterSize(size);
}

void TextComponent::SetColor(const unsigned char red, const unsigned char green, const unsigned char blue, const unsigned char alpha)
{
	sf::Color color(red, green, blue, alpha);
	m_Text->setFillColor(color);
}

void TextComponent::SetPosition(const float X, const float Y)
{
	m_Text->setPosition(X, Y);
}
