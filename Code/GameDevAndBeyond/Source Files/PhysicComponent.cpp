#include "../Header Files/PhysicComponent.h"

PhysicComponent::PhysicComponent()
{
}

PhysicComponent::~PhysicComponent()
{
}

void PhysicComponent::GetUpdateWaitTime(float & updateWaitTime)
{
	updateWaitTime = m_UpdateWaitTime;
}

void PhysicComponent::SetUpdateWaitTime(float updateWaitTime)
{
	m_UpdateWaitTime = updateWaitTime;
}
