#include "../Header Files/TextComponent.h"

TextComponent::TextComponent(std::string text)
{
	m_Text->setString(text);
}

TextComponent::~TextComponent()
{
	delete m_Font;
	delete m_Text;
}

///////////////////////////////////Utility
void TextComponent::CenterText()
{
	// TODO Center text does not work find out why
	sf::FloatRect textRect = m_Text->getLocalBounds();
	m_Text->setOrigin(textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);
}


///////////////////////////////////Setter

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
	m_Color = color;
	m_Text->setFillColor(color);
}

void TextComponent::SetPosition(const float X, const float Y)
{
	m_Text->setPosition(X, Y);
}

void TextComponent::SetVisibilityOnStateChange()
{
	eGameState state;
	Engine::Instance()->GetGameState(state);
	if (state == m_VisibleState) 
	{
		sf::Color color(255, 255, 255, 255);
		m_Text->setFillColor(color);
		// TODO m_Color is changed somewhere find out why
		// m_Text->setFillColor(m_Color);
	}
	else 
	{
		SetColor(0, 0, 0, 0);
	}
}
