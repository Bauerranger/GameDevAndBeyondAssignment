#pragma once

#include "SFMLEngine/Core/Interfaces/EngineTickListener.h"
#include "SFMLEngine/Core/InputManager.h"
#include "SFMLEngine/EntitySystem/EntityId.h"
#include "SFMLEngine/Serialization/IArchive.h"

#include "GameDevAndBeyond/Tiles/Tile.h"
#include "GameDevAndBeyond/Tiles/TileMap.h"
#include "GameDevAndBeyond/Tiles/TileUtility.h"
#include "GameDevAndBeyond/Navigation/NavigationDefinitions.h"

#include <array>
#include <future>

#include "SFML/Network.hpp"

struct TileChangeNetworkRMI
{
	void Serialize(IArchive& archive)
	{
		archive.Serialize(x, "x");
		archive.Serialize(y, "y");
		archive.Serialize(id, "id");
	}

	TileCoordinate x = 0;
	TileCoordinate y = 0;
	TileId id = 0;
};

class Game 
	: public IEngineTickListener
{

public:

	static Game* Instance();
	
	Game();
	~Game();

private:

	Game(const Game&) = delete;
	Game& operator=(Game const&) = delete;

	virtual void OnStart() override;
	virtual void OnTick(float deltaSeconds) override;

	void SetSelectedTileOnPalette(int id);
	void TogglePaletteVisibility();
	void DeleteReachableTilePreviews();

	void LeftClick();
	void RightClick();
	void ToggleBGM();
	void ProcessNavigationResults();

	void SaveState();
	void LoadState();

	void Serialize(IArchive& archive);

	bool IsPlayingMusic() const;
	bool IsNavigationFutureDone() const;

	EntityId m_textEntityId;
	EntityId m_tilePaletteEntityId;
	EntityId m_tilePreviewEntityId;
	EntityId m_musicEntityId;

	std::vector<EntityId> m_reachableTilePreviews;
	sf::Texture m_reachableTilePreviewTexture;

	sf::Font m_font;
	InputBindingScope m_inputScope;

	bool m_isCalculatingNavigation = false;
	std::future<NavigationArea> m_navigationFuture;

	std::unique_ptr<TileMap> m_pTileMap;
	bool m_isPaletteOpen = false;
	TileId m_selectedTileId = -1;

	sf::UdpSocket m_udpSocket;
	sf::Uint16 m_portToSendTo = 0;
	sf::Uint16 m_myPort = 0;
	bool m_isServer = false;

	static Game* s_pInstance;
};