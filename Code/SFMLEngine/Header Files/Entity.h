#pragma once

#include <vector>
#include <type_traits>
#include <memory>

#include "IComponent.h"

class Entity
{

public:
	template<typename T>
	std::shared_ptr<T> AddComponent()
	{
		//check if T derives from IComponent
		static_assert(std::is_base_of<IComponent, T>::value, "T must be derived from IComponent");
		//check if the Component is already inside of the array
		std::shared_ptr<T> foundComponent = GetComponent<T>();
		if (foundComponent != nullptr)
		{
			return foundComponent;
		}

		std::shared_ptr<T> component = std::make_shared<T>();
		m_Components.push_back(std::shared_ptr<T>(component));
		return component;
	}

	template<typename T>
	void RemoveComponent()
	{
		//check if T derives from IComponent
		static_assert(std::is_base_of<IComponent, T>::value, "T must be derived from IComponent");
		//go through all components and delete the matching Component
		for (std::vector<std::shared_ptr<IComponent>>::iterator componentIterator = m_Components.begin(); componentIterator != m_Components.end();)
		{
			if (std::shared_ptr<T> cast = std::dynamic_pointer_cast<T>(*componentIterator))
			{
				m_Components.erase(componentIterator);
				break;
			}
			else
			{
				++componentIterator;
			}
		}
	}

	template<typename T>
	std::shared_ptr<T> GetComponent()
	{
		//check if T derives from IComponent
		static_assert(std::is_base_of<IComponent, T>::value, "T must be derived from IComponent");
		//Go through all Components and return if the cast to the derived class is true
		for (std::shared_ptr<IComponent> component : m_Components)
		{
			if (std::shared_ptr<T> cast = std::dynamic_pointer_cast<T>(component))
			{
				return cast;
			}
		}

		return nullptr;
	}

	template<typename T>
	bool HasComponent()
	{
		return GetComponent<T>() != nullptr;
	}

protected:
	std::vector<std::shared_ptr<IComponent>> m_Components;
};