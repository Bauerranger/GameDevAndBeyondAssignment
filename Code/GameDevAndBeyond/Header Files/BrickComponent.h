#pragma once
#include <SFMLEngine/Header Files/IComponent.h>
#include <SFMLEngine/Header Files/Engine.h>

class BrickComponent : public IComponent 
{
public:
	BrickComponent();
	~BrickComponent();
public:
	void GetBrickMatrixPosition(int& matrixPosX, int& matrixPosY);
	void SetBrickMatrixPosition(int matrixPosX, int matrixPosY);
	void GetShapeKey(int& shapeKey);
	void SetShapeKey(int shapeKey);
private:
	float m_MatrixPosX = 0;
	float m_MatrixPosY = 0;
	int m_ShapeKey = 0;
};