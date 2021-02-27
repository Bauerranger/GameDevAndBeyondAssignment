#include "../Header Files/ISystem.h"

void ISystem::Update(Engine* engine, float dt)
{

}

void ISystem::Init(Engine* engine)
{

}

void ISystem::AddEntity(std::shared_ptr<Entity> entity)
{
	if (std::find(m_Entities.begin(), m_Entities.end(), entity) != m_Entities.end())
	{
		return;
	}
	m_Entities.push_back(entity);
}

void ISystem::RemoveEntity(std::shared_ptr<Entity> entity)
{
	std::lock_guard<std::mutex> lock(m_Mutex);
	std::vector<std::shared_ptr<Entity>>::iterator entityIterator = std::find(m_Entities.begin(), m_Entities.end(), entity);
	if (entityIterator == m_Entities.end())
	{
		return;
	}
	m_Entities.erase(entityIterator);
}

