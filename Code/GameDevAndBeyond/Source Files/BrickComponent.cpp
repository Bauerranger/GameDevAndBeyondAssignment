#include "../Header Files/BrickComponent.h"
#include <iostream>

BrickComponent::BrickComponent()
= default;

BrickComponent::~BrickComponent()
= default;

void BrickComponent::GetBrickMatrixPosition(int& matrixPosX, int& matrixPosY)
{
	matrixPosX = m_MatrixPosX;
	matrixPosY = m_MatrixPosY;
}

void BrickComponent::SetBrickMatrixPosition(int posX, int posY)
{
	m_MatrixPosX = posX;
	m_MatrixPosY = posY;
}

void BrickComponent::GetShapeKey(int & shapeKey)
{
	shapeKey = m_ShapeKey;
}

void BrickComponent::SetShapeKey(int shapeKey)
{
	m_ShapeKey = shapeKey;
}
