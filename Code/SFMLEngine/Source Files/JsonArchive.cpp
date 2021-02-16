#include "../Header Files/JsonArchive.h"
#include <fstream>

void JsonOutputArchive::Save(const std::string& filePath)
{
	std::ofstream fileStream;
	fileStream.open(filePath);

	fileStream << m_root.dump(4);
}

void JsonOutputArchive::Serialize(bool& value, const std::string& label)
{
	(*m_pCurrentJson)[label] = value;
}

void JsonOutputArchive::Serialize(sf::Uint8& value, const std::string& label)
{
	(*m_pCurrentJson)[label] = value;
}

void JsonOutputArchive::Serialize(sf::Int8& value, const std::string& label)
{
	(*m_pCurrentJson)[label] = value;
}

void JsonOutputArchive::Serialize(sf::Uint16& value, const std::string& label)
{
	(*m_pCurrentJson)[label] = value;
}

void JsonOutputArchive::Serialize(sf::Int16& value, const std::string& label)
{
	(*m_pCurrentJson)[label] = value;
}

void JsonOutputArchive::Serialize(sf::Uint32& value, const std::string& label)
{
	(*m_pCurrentJson)[label] = value;
}

void JsonOutputArchive::Serialize(sf::Int32& value, const std::string& label)
{
	(*m_pCurrentJson)[label] = value;
}

void JsonOutputArchive::Serialize(sf::Uint64& value, const std::string& label)
{
	(*m_pCurrentJson)[label] = value;
}

void JsonOutputArchive::Serialize(sf::Int64& value, const std::string& label)
{
	(*m_pCurrentJson)[label] = value;
}

void JsonOutputArchive::Serialize(float& value, const std::string& label)
{
	(*m_pCurrentJson)[label] = value;
}

void JsonOutputArchive::Serialize(double& value, const std::string& label)
{
	(*m_pCurrentJson)[label] = value;
}

void JsonOutputArchive::Serialize(std::string& value, const std::string& label)
{
	(*m_pCurrentJson)[label] = value;
}

void JsonOutputArchive::Serialize(VectorInterface& value, const std::string& label)
{
	JSONType* oldJson = m_pCurrentJson;

	JSONType vectorJson;
	m_pCurrentJson = &vectorJson;

	const size_t vectorSize = value.GetSize();
	for (int i = 0; i < vectorSize; i++)
	{
		value.SerializeElement(*this, i);
	}

	(*oldJson)[label] = vectorJson;
	m_pCurrentJson = oldJson;
}

void JsonOutputArchive::Serialize(ObjectInterface& value, const std::string& label)
{
	JSONType* oldJson = m_pCurrentJson;

	JSONType tileMapJson;
	m_pCurrentJson = &tileMapJson;

	value.SerializeObject(*this);

	(*oldJson)[label] = tileMapJson;
	m_pCurrentJson = oldJson;
}










void JsonInputArchive::Load(const std::string& filePath)
{
	std::ifstream fileStream(filePath);
	fileStream >> m_root;
}

void JsonInputArchive::Serialize(bool& value, const std::string& label)
{
	value = (*m_pCurrentJson)[label].get<bool>();
}

void JsonInputArchive::Serialize(sf::Uint8& value, const std::string& label)
{
	value = (*m_pCurrentJson)[label].get<sf::Uint8>();
}

void JsonInputArchive::Serialize(sf::Int8& value, const std::string& label)
{
	value = (*m_pCurrentJson)[label].get<sf::Int8>();
}

void JsonInputArchive::Serialize(sf::Uint16& value, const std::string& label)
{
	value = (*m_pCurrentJson)[label].get<sf::Uint16>();
}

void JsonInputArchive::Serialize(sf::Int16& value, const std::string& label)
{
	value = (*m_pCurrentJson)[label].get<sf::Int16>();
}

void JsonInputArchive::Serialize(sf::Uint32& value, const std::string& label)
{
	value = (*m_pCurrentJson)[label].get<sf::Uint32>();
}

void JsonInputArchive::Serialize(sf::Int32& value, const std::string& label)
{
	value = (*m_pCurrentJson)[label].get<sf::Int32>();
}

void JsonInputArchive::Serialize(sf::Uint64& value, const std::string& label)
{
	value = (*m_pCurrentJson)[label].get<sf::Uint64>();
}

void JsonInputArchive::Serialize(sf::Int64& value, const std::string& label)
{
	value = (*m_pCurrentJson)[label].get<sf::Int64>();
}

void JsonInputArchive::Serialize(float& value, const std::string& label)
{
	value = (*m_pCurrentJson)[label].get<float>();
}

void JsonInputArchive::Serialize(double& value, const std::string& label)
{
	value = (*m_pCurrentJson)[label].get<double>();
}

void JsonInputArchive::Serialize(std::string& value, const std::string& label)
{
	value = (*m_pCurrentJson)[label].get<std::string>();
}

void JsonInputArchive::Serialize(VectorInterface& value, const std::string& label)
{
	JSONType* oldJson = m_pCurrentJson;

	JSONType& vectorJson = (*m_pCurrentJson)[label];
	m_pCurrentJson = &vectorJson;

	const size_t vectorSize = vectorJson.size();
	value.Resize(vectorSize);

	for (int i = 0; i < vectorSize; i++)
	{
		value.SerializeElement(*this, i);
	}

	m_pCurrentJson = oldJson;
}

void JsonInputArchive::Serialize(ObjectInterface& value, const std::string& label)
{
	JSONType* oldJson = m_pCurrentJson;

	JSONType& tilemapJson = (*m_pCurrentJson)[label];
	m_pCurrentJson = &tilemapJson;

	value.SerializeObject(*this);

	m_pCurrentJson = oldJson;
}
