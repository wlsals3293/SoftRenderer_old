#pragma once

#include "stdafx.h"
#include "Vector.h"

struct Transform2D
{
public:
	Transform2D() : angle(0), scale(Vector2(1,1)) {}
	Transform2D(Vector2 InPosition, float InAngle, Vector2 InScale)
		: position(InPosition), angle(InAngle), scale(InScale) {}

public:
	Vector2 position;
	float angle;
	Vector2 scale;
};