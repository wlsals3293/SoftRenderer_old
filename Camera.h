#pragma once

#include "stdafx.h"
#include "Transform.h"
#include "Matrix.h"

struct Camera2D
{
	Camera2D() {}
	Camera2D(Transform2D InTransform)
		: transform(InTransform) {}
public:
	Transform2D transform;
	Matrix3 inverseMatrix;

	void SetPosition(float InX, float InY)
	{
		transform.position.X = InX;
		transform.position.Y = InY;
		CalculateMatrix();
	}

	void SetRotation(float degree)
	{
		transform.angle = degree;
		CalculateMatrix();
	}

	void CalculateMatrix()
	{
		Matrix3 tMat, rMat;

		tMat.SetTranslation(-transform.position.X, -transform.position.Y);
		rMat.SetRotation(transform.angle);
		rMat.Transpose();

		inverseMatrix = rMat * tMat;
	}
};