#pragma once

#include <string>
#include <SFML/Config.hpp>

#include "VectorInterface.h"
#include "ObjectInterface.h"

struct IArchive
{
	virtual bool IsInput() const = 0;
	virtual bool IsOutput() const = 0;

	virtual void Serialize(bool& value, const std::string& label) = 0;
	virtual void Serialize(sf::Uint8& value, const std::string& label) = 0;
	virtual void Serialize(sf::Int8& value, const std::string& label) = 0;
	virtual void Serialize(sf::Uint16& value, const std::string& label) = 0;
	virtual void Serialize(sf::Int16& value, const std::string& label) = 0;
	virtual void Serialize(sf::Uint32& value, const std::string& label) = 0;
	virtual void Serialize(sf::Int32& value, const std::string& label) = 0;
	virtual void Serialize(sf::Uint64& value, const std::string& label) = 0;
	virtual void Serialize(sf::Int64& value, const std::string& label) = 0;
	virtual void Serialize(float& value, const std::string& label) = 0;
	virtual void Serialize(double& value, const std::string& label) = 0;
	virtual void Serialize(std::string& value, const std::string& label) = 0;
	virtual void Serialize(VectorInterface& value, const std::string& label) = 0;
	virtual void Serialize(ObjectInterface& value, const std::string& label) = 0;

	template<typename T>
	void Serialize(std::vector<T>& value, const std::string& label)
	{
		VectorInterface_Impl<T> vectorInterfaceImpl(value);
		VectorInterface& vectorInterface = vectorInterfaceImpl;
		Serialize(vectorInterface, label);
	}

	template<typename T>
	void Serialize(T& value, const std::string& label)
	{
		ObjectInterface_Impl<T> objectInterfaceImpl(value);
		ObjectInterface& objectInterface = objectInterfaceImpl;
		Serialize(objectInterface, label);
	}
};