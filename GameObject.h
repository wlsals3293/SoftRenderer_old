#pragma once

#include "stdafx.h"
#include "Mesh.h"
#include "Transform.h"

struct GameObject2D
{
	GameObject2D() {}
	GameObject2D(Transform2D InTransform, Mesh InMesh)
		: transform(InTransform), mesh(InMesh) {}
public:
	Mesh mesh;
	Transform2D transform;

	void SetPosition(float InX, float InY)
	{
		transform.position.X = InX;
		transform.position.Y = InY;
	}

	void SetRotation(float degree)
	{
		transform.angle = degree;
	}
};