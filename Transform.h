#pragma once

#include "stdafx.h"
#include "Vector.h"

struct Transform2D
{
public:
	Transform2D() : Angle(0), Scale(Vector2(1,1)) {}
	Transform2D(Vector2 InPosition, float InAngle, Vector2 InScale)
		: Position(InPosition), Angle(InAngle), Scale(InScale) {}

public:
	Vector2 Position;
	float Angle;
	Vector2 Scale;
};