#pragma once
#include <SFMLEngine/Header Files/IComponent.h>
#include <vector>
#include <memory>

class Entity;

class PhysicComponent : public IComponent
{
public:
	PhysicComponent();
	~PhysicComponent();
public:
	void GetUpdateWaitTime(float& updateWaitTime);
	void SetUpdateWaitTime(float updateWaitTime);
private:
	float m_UpdateWaitTime = 0.5f;
};