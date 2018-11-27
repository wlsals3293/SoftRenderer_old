#pragma once

#include "stdafx.h"
#include "Mesh.h"
#include "Transform.h"

struct GameObject2D
{
	GameObject2D() {}
	GameObject2D(Transform2D InTransform, Mesh InMesh)
		: Transform(InTransform), Mesh(InMesh) {}
public:
	Mesh Mesh;
	Transform2D Transform;
};