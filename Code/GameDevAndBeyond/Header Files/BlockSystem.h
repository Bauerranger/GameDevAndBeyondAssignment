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

	static BlockSystem* Instance();
public:

	//ISystem
	virtual void Init(Engine* engine);
	virtual bool DoesEntityMatch(std::shared_ptr<Entity> entity) override;
	virtual void Update(Engine* engine, float dt) override;
	std::vector<std::shared_ptr<Entity>> GetBrickEntities() { return m_Entities; };
private:

	//different block types
	struct BlockShape
	{
		bool BlockShape[2][4];
	};
	std::map<int, BlockShape> m_BlockShapes;
	bool m_ReadyForLeftKeyPress = false;
	bool m_ReadyForRightKeyPress = false;
	bool m_ReadyForDropKeyPress = false;
	bool m_ReadyForRotateKeyPress = false;
	std::shared_ptr<EventHandler> m_CollisionListener;
	std::shared_ptr<EventHandler> m_LooseListener;
	EventFunctor m_CollisionEventFunctor;
	EventFunctor m_LooseEventFunctor;
	float m_TickTime = 0;
	Engine* m_Engine;
	int m_ShapeKey = 0;
	int m_CurrentRotation = 0;
	int m_MaximumRotations = 4;

	bool inline UpdateEntities(Engine* engine, float dt);
	void MoveLeft(Engine * engine);
	void Drop(Engine * engine);
	void MoveRight(Engine * engine);
	void Rotation(Engine * engine);
	void GetUnmovedEntities(std::vector<std::shared_ptr<Entity>> &copiedEntities, std::vector<std::shared_ptr<Entity>> &unmovedEntities);
	void MoveBrickEntities(bool  rotationArray[4][4], std::vector<std::shared_ptr<Entity>> &movedEntities, int &movedEntitiesCounter, int mostLeftMatrixPosition, int mostTopMatrixPosition);
	void CheckIfRotationPossible(bool  rotationArray[4][4], std::vector<std::shared_ptr<Entity>> &movedEntities, int &movedEntitiesCounter, std::vector<std::shared_ptr<Entity>> &unmovedEntities, bool &retflag);
	void SetSpritePosOnWindow(std::vector<std::shared_ptr<Entity>> &movedEntities, int movedEntitiesCounter, int brickMatrixPosX, int brickMatrixPosY);
	void SpawnBlock();
	void ResetPosition();
	void OnCollision(std::shared_ptr<IEvent> event);
	void OnLoose(std::shared_ptr<IEvent> event);

	static BlockSystem* s_pInstance;
};