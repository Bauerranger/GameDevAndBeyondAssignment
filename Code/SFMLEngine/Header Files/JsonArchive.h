#pragma once

#include "IArchive.h"
#include <nlohmann/json.hpp>

// Unused

using JSONType = nlohmann::ordered_json;

class JsonOutputArchive : public IArchive
{

public:

	JsonOutputArchive()
		: m_pCurrentJson(&m_root)
	{ }

	void Save(const std::string& filePath);

	bool IsInput() const override { return false; }
	bool IsOutput() const override { return true; }

	void Serialize(bool& value, const std::string& label) override;
	void Serialize(sf::Uint8& value, const std::string& label) override;
	void Serialize(sf::Int8& value, const std::string& label) override;
	void Serialize(sf::Uint16& value, const std::string& label) override;
	void Serialize(sf::Int16& value, const std::string& label) override;
	void Serialize(sf::Uint32& value, const std::string& label) override;
	void Serialize(sf::Int32& value, const std::string& label) override;
	void Serialize(sf::Uint64& value, const std::string& label) override;
	void Serialize(sf::Int64& value, const std::string& label) override;
	void Serialize(float& value, const std::string& label) override;
	void Serialize(double& value, const std::string& label) override;
	void Serialize(std::string& value, const std::string& label) override;
	void Serialize(VectorInterface& value, const std::string& label) override;
	void Serialize(ObjectInterface& value, const std::string& label) override;

private:

	JSONType m_root;
	JSONType* m_pCurrentJson;
};

class JsonInputArchive : public IArchive
{
public:

	JsonInputArchive()
		: m_pCurrentJson(&m_root)
	{ }

	void Load(const std::string& filePath);

	bool IsInput() const override { return true; }
	bool IsOutput() const override { return false; }

	void Serialize(bool& value, const std::string& label) override;
	void Serialize(sf::Uint8& value, const std::string& label) override;
	void Serialize(sf::Int8& value, const std::string& label) override;
	void Serialize(sf::Uint16& value, const std::string& label) override;
	void Serialize(sf::Int16& value, const std::string& label) override;
	void Serialize(sf::Uint32& value, const std::string& label) override;
	void Serialize(sf::Int32& value, const std::string& label) override;
	void Serialize(sf::Uint64& value, const std::string& label) override;
	void Serialize(sf::Int64& value, const std::string& label) override;
	void Serialize(float& value, const std::string& label) override;
	void Serialize(double& value, const std::string& label) override;
	void Serialize(std::string& value, const std::string& label) override;
	void Serialize(VectorInterface& value, const std::string& label) override;
	void Serialize(ObjectInterface& value, const std::string& label) override;

private:

	JSONType m_root;
	JSONType* m_pCurrentJson;
};