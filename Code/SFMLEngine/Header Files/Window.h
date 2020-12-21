#pragma once
#include <string>

//forward declaration, so that we don't need to include SFML in Game Project
namespace sf
{
	class RenderWindow;
	class Sprite;
	class Text;
	class RenderWindow;
	class VertexArray;
	class Texture;
	class View;
}

class Window
{
public:
	Window(const int width, const int height, const std::string title, const bool fullscreen);
	~Window();

public:
	bool Update();
	void BeginDraw();
	void EndDraw();
	void Draw(const sf::Sprite* sprite);
	void Draw(const sf::Text* text);
	void Draw(const sf::VertexArray* vertexArray, const sf::Texture* texture);
	void SetViewCenter(float X, float Y);
	void GetViewPosition(float& X, float& Y);

	const sf::RenderWindow* GetWindow() { return m_Window; }

private:
	sf::RenderWindow* m_Window = nullptr;
	sf::View* m_GameView = nullptr;
	sf::View* m_UIView = nullptr;

};