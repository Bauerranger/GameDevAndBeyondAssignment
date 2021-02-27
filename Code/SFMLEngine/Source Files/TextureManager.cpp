#include "../Header Files/TextureManager.h"
#include <SFML/Graphics.hpp>

TextureManager::TextureManager()
= default;

TextureManager::~TextureManager()
= default;

sf::Texture* TextureManager::GetTexture(std::string texturePath)
{
	if (m_Textures.find(texturePath) == m_Textures.end())
	{
		sf::Texture* texture = new sf::Texture();
		texture->loadFromFile(texturePath);
		m_Textures[texturePath] = texture;
		return texture;
	}
	return m_Textures.find(texturePath)->second;
}