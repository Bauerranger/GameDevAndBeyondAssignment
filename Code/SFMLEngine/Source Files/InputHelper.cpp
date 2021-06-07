#include "../Header Files/InputHelper.h"
#include "../Header Files/Engine.h"
#include <SFML/Graphics.hpp>

///////////////////////////////////////////////////// Mouse

bool InputHelper::IsMousePressed(MouseButton button)
{
	return sf::Mouse::isButtonPressed((sf::Mouse::Button)button);
}

void InputHelper::GetCursorPosition(const Engine* engine, float& X, float& Y)
{
	const std::shared_ptr<Window>& window = engine->GetWindow();
	const sf::RenderWindow* renderWindow = window->GetWindow();

	sf::Vector2i position = sf::Mouse::getPosition(*renderWindow);

	float viewX, viewY = 0.f;
	window->GetViewPosition(viewX, viewY);

	X = position.x + viewX;
	Y = position.y + viewY;
}

///////////////////////////////////////////////////// Keyboard

bool InputHelper::IsKeyPressed(Key key)
{
	return sf::Keyboard::isKeyPressed((sf::Keyboard::Key)key);
}

char m_Input;
void InputHelper::GetInput(char& input)
{
	input = m_Input;
	m_Input = 0;
}

void InputHelper::SetInput(char input)
{
	m_Input = input;
}
