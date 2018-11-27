#pragma once

#include "stdafx.h"
#include "Triangle.h"

struct Mesh
{
public:
	Mesh() : Triangles(NULL), TriangleCount(0) {}

public:
	Triangle* Triangles;
	int TriangleCount;
};