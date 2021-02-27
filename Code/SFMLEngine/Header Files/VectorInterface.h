#pragma once

#include <vector>

struct IArchive;

struct VectorInterface
{
	virtual size_t GetSize() const = 0;
	virtual void Resize(size_t newSize) = 0;
	virtual void SerializeElement(IArchive& archive, size_t index) = 0;
};

template<typename T>
struct VectorInterface_Impl : public VectorInterface
{
	VectorInterface_Impl(std::vector<T>& vector)
		: vectorReference(vector)
	{ }

	size_t GetSize() const override
	{
		return vectorReference.size();
	}

	void Resize(size_t newSize) override
	{
		vectorReference.resize(newSize);
	}

	void SerializeElement(IArchive& archive, size_t index) override
	{
		archive.Serialize(vectorReference[index], std::to_string(index));
	}

	std::vector<T>& vectorReference;
};