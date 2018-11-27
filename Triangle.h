#pragma once

#include "stdafx.h"
#include "Vertex.h"

struct Triangle
{
public:
	Triangle() {}
	Triangle(Vertex v1, Vertex v2, Vertex v3)
	{
		vt[0] = v1;
		vt[1] = v2;
		vt[2] = v3;
	}

public:
	Vertex vt[3];

};