#pragma once

#include <SFMLEngine/Header Files/ISystem.h>
#include <SFMLEngine/Header Files/EventManager.h>
#include "Events.h"
#include <map> 

class BlockSystem : public ISystem
{
public:
	BlockSystem();
	~BlockSystem();

	///////////////////////////////////////////////////// Singleton
	static BlockSystem* Instance();
public:

	///////////////////////////////////////////////////// ISystem
	void Init(Engine* engine) override;
	bool DoesEntityMatch(std::shared_ptr<Entity> entity) override;
	void Update(Engine* engine, float dt) override;
	std::vector<std::shared_ptr<Entity>> GetBrickEntities() { return m_Entities; };
private:

	///////////////////////////////////////////////////// Block types
	struct BlockShape
	{
		bool BlockShape[2][4];
	};
	std::map<int, BlockShape> m_BlockShapes;
	short unsigned int m_ShapeKey = 0;
	short unsigned int m_CurrentRotation = 0;
	const int m_MaximumRotations = 4;

	///////////////////////////////////////////////////// Controls
	bool m_ReadyForLeftKeyPress = false;
	bool m_ReadyForRightKeyPress = false;
	bool m_ReadyForDropKeyPress = false;
	bool m_ReadyForRotateKeyPress = false;

	void MoveLeft(Engine* engine);
	void Drop(Engine* engine);
	void MoveRight(Engine* engine);
	void Rotation(Engine* engine);

	///////////////////////////////////////////////////// Events
	std::shared_ptr<EventHandler> m_CollisionListener;
	std::shared_ptr<EventHandler> m_LooseListener;
	EventFunctor m_CollisionEventFunctor;
	EventFunctor m_LooseEventFunctor;

	void OnCollision(std::shared_ptr<IEvent> event);
	void OnLoose(std::shared_ptr<IEvent> event);

	///////////////////////////////////////////////////// Engine
	float m_TickTime = 0;

	bool inline UpdateEntities(Engine* engine, float dt);

	///////////////////////////////////////////////////// Movement
	void GetUnmovedEntities(std::vector<std::shared_ptr<Entity>> &copiedEntities, std::vector<std::shared_ptr<Entity>> &unmovedEntities);
	void MoveBrickEntities(bool  rotationArray[4][4], std::vector<std::shared_ptr<Entity>> &movedEntities, int &movedEntitiesCounter, int mostLeftMatrixPosition, int mostTopMatrixPosition);
	void CheckIfRotationPossible(bool  rotationArray[4][4], std::vector<std::shared_ptr<Entity>> &movedEntities, int &movedEntitiesCounter, std::vector<std::shared_ptr<Entity>> &unmovedEntities, bool &retflag);
	void SetSpritePosOnWindow(std::vector<std::shared_ptr<Entity>> &movedEntities, int movedEntitiesCounter, int brickMatrixPosX, int brickMatrixPosY);

	///////////////////////////////////////////////////// Spawn
	void SpawnBlock();
	void ResetPosition();
	bool m_IsEnd = false;

	///////////////////////////////////////////////////// Singleton
	static BlockSystem* s_pInstance;
};