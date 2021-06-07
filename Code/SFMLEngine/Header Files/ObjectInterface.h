#pragma once

// Unused

struct IArchive;

struct ObjectInterface
{
	virtual void SerializeObject(IArchive& archive) = 0;
};

template<typename T>
struct ObjectInterface_Impl : public ObjectInterface
{
	ObjectInterface_Impl(T& value)
		: objectReference(value)
	{ }

	void SerializeObject(IArchive& archive) override
	{
		objectReference.Serialize(archive);
	}

	T& objectReference;
};