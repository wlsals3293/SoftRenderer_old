
#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"

#include "Vector.h"
#include "Vertex.h"
#include "IntPoint.h"

#include "Camera.h"
#include "GameObject.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y);
void DrawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3, const Matrix3 trsMat);
void DrawTriangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3);


bool IsInRange(int x, int y)
{
	return (abs(x) < (g_nClientWidth / 2)) && (abs(y) < (g_nClientHeight / 2));
}

void PutPixel(const IntPoint& InPt)
{
	PutPixel(InPt.X, InPt.Y);
}

void PutPixel(int x, int y)
{
	if (!IsInRange(x, y)) return;

	ULONG* dest = (ULONG*)g_pBits;
	DWORD offset = g_nClientWidth * g_nClientHeight / 2 + g_nClientWidth / 2 + x + g_nClientWidth * -y;
	*(dest + offset) = g_CurrentColor;
}

void DrawLine(const Vector3 &start, const Vector3 &end)
{
	// L = (1-t)P + tQ, 0 <= t <= 1
	float length = Vector3::Dist(end, start);
	float inc = 1.0f / length;
	int nLength = (int)(length + 1);

	for (int i = 0; i < nLength; i++)
	{
		float t = inc * i;
		if (t >= length) t = 1.0f;
		Vector3 pt = start * (1.0f - t) + end * t;
		PutPixel(IntPoint(pt));
	}
}

void DrawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3, const Matrix3 &trsMat, const Matrix3 &camMat)
{
	Vector2 minPos = Vector2(INFINITY, INFINITY);
	Vector2 maxPos = Vector2(-INFINITY, -INFINITY);

	Vector3 p1 = v1.position * trsMat * camMat;
	Vector3 p2 = v2.position * trsMat * camMat;
	Vector3 p3 = v3.position * trsMat * camMat;

	if (p1.X < minPos.X) minPos.X = p1.X;
	if (p1.Y < minPos.Y) minPos.Y = p1.Y;
	if (p1.X > maxPos.X) maxPos.X = p1.X;
	if (p1.Y > maxPos.Y) maxPos.Y = p1.Y;

	if (p2.X < minPos.X) minPos.X = p2.X;
	if (p2.Y < minPos.Y) minPos.Y = p2.Y;
	if (p2.X > maxPos.X) maxPos.X = p2.X;
	if (p2.Y > maxPos.Y) maxPos.Y = p2.Y;

	if (p3.X < minPos.X) minPos.X = p3.X;
	if (p3.Y < minPos.Y) minPos.Y = p3.Y;
	if (p3.X > maxPos.X) maxPos.X = p3.X;
	if (p3.Y > maxPos.Y) maxPos.Y = p3.Y;

	Vector3 u = p2 - p1;
	Vector3 v = p3 - p1;
	float dotUU = Vector3::Dot(u, u);
	float dotUV = Vector3::Dot(u, v);
	float dotVV = Vector3::Dot(v, v);
	float invDenom = 1.0f / (dotUU * dotVV - dotUV * dotUV);

	IntPoint minPt(minPos);
	IntPoint maxPt(maxPos);

	for (int x = minPt.X; x < maxPt.X; x++)
	{
		for (int y = minPt.Y; y < maxPt.Y; y++)
		{
			IntPoint pt(x, y);
			Vector3 w = pt.ToVector3() - p1;
			float dotUW = Vector3::Dot(u, w);
			float dotVW = Vector3::Dot(v, w);
			float s = (dotVV * dotUW - dotUV * dotVW) * invDenom;
			float t = (dotUU * dotVW - dotUV * dotUW) * invDenom;
			if (s >= 0 && t >= 0 && ((s + t) <= 1))
			{
				ULONG finalColor = RGB32(255, 0, 0);

				if (g_Texture->IsLoaded()) {
					Vector2 baryUV = v1.uv * (1 - s - t) + v2.uv * s + v3.uv * t;
					finalColor = g_Texture->GetTexturePixel(baryUV);
				}
				else {
					BYTE RV1 = GetRValue(v1.color);
					BYTE RV2 = GetRValue(v2.color);
					BYTE RV3 = GetRValue(v3.color);

					BYTE GV1 = GetGValue(v1.color);
					BYTE GV2 = GetGValue(v2.color);
					BYTE GV3 = GetGValue(v3.color);

					BYTE BV1 = GetBValue(v1.color);
					BYTE BV2 = GetBValue(v2.color);
					BYTE BV3 = GetBValue(v3.color);

					BYTE FinalR = (BYTE)(RV1 * (1 - s - t) + RV2 * s + RV3 * t);
					BYTE FinalG = (BYTE)(GV1 * (1 - s - t) + GV2 * s + GV3 * t);
					BYTE FinalB = (BYTE)(BV1 * (1 - s - t) + BV2 * s + BV3 * t);

					finalColor = RGB32(FinalR, FinalG, FinalB);
				}
				SetColor(finalColor);
				PutPixel(pt);
			}
		}
	}
}

void DrawTriangle(const Vector3 &p1, const Vector3 &p2, const Vector3 &p3)
{
	Vector2 minPos = Vector2(INFINITY, INFINITY);
	Vector2 maxPos = Vector2(-INFINITY, -INFINITY);

	if (p1.X < minPos.X) minPos.X = p1.X;
	if (p1.Y < minPos.Y) minPos.Y = p1.Y;
	if (p1.X > maxPos.X) maxPos.X = p1.X;
	if (p1.Y > maxPos.Y) maxPos.Y = p1.Y;

	if (p2.X < minPos.X) minPos.X = p2.X;
	if (p2.Y < minPos.Y) minPos.Y = p2.Y;
	if (p2.X > maxPos.X) maxPos.X = p2.X;
	if (p2.Y > maxPos.Y) maxPos.Y = p2.Y;

	if (p3.X < minPos.X) minPos.X = p3.X;
	if (p3.Y < minPos.Y) minPos.Y = p3.Y;
	if (p3.X > maxPos.X) maxPos.X = p3.X;
	if (p3.Y > maxPos.Y) maxPos.Y = p3.Y;

	Vector3 u = p2 - p1;
	Vector3 v = p3 - p1;
	float dotUU = Vector3::Dot(u, u);
	float dotUV = Vector3::Dot(u, v);
	float dotVV = Vector3::Dot(v, v);
	float invDenom = 1.0f / (dotUU * dotVV - dotUV * dotUV);

	IntPoint minPt(minPos);
	IntPoint maxPt(maxPos);

	for (int x = minPt.X; x < maxPt.X; x++)
	{
		for (int y = minPt.Y; y < maxPt.Y; y++)
		{
			IntPoint pt(x, y);
			Vector3 w = pt.ToVector3() - p1;
			float dotUW = Vector3::Dot(u, w);
			float dotVW = Vector3::Dot(v, w);
			float s = (dotVV * dotUW - dotUV * dotVW) * invDenom;
			float t = (dotUU * dotVW - dotUV * dotUW) * invDenom;
			if (s >= 0 && t >= 0 && ((s + t) <= 1))
			{
				PutPixel(pt);
			}
		}
	}
}

void DrawGameObject(const GameObject2D &obj, const Matrix3 &camMatrix) {
	// Define Matrix
	Matrix3 tMat, rMat, sMat, trsMat;
	tMat.SetTranslation(obj.transform.position.X, obj.transform.position.Y);
	sMat.SetScale(obj.transform.scale.X, obj.transform.scale.Y, 1);
	rMat.SetRotation(obj.transform.angle);
	trsMat = tMat * rMat * sMat;

	for (int i = 0; i < obj.mesh.TriangleCount; i++)
	{
		DrawTriangle(
			obj.mesh.Triangles[i].vt[0],
			obj.mesh.Triangles[i].vt[1],
			obj.mesh.Triangles[i].vt[2],
			trsMat, camMatrix);
	}
}


void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	static Camera2D cam;
	
	static float degree = 0;
	static float posX = 0;
	static float posY = 0;
	static float scale = 1;

	float speed = 5;

	// Input
	bool camUpdate = false;


	if (GetAsyncKeyState(VK_HOME)) {
		degree += 1;
		camUpdate = true;
	}
	if (GetAsyncKeyState(VK_END)) {
		degree -= 1;
		camUpdate = true;
	}

	float radian = Deg2Rad(degree);
	static float nine = Deg2Rad(90);

	if (GetAsyncKeyState(VK_LEFT)) {
		posX -= cosf(radian) * speed;
		posY -= sinf(radian) * speed;
		camUpdate = true;
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		posX += cosf(radian) * speed;
		posY += sinf(radian) * speed;
		camUpdate = true;
	}
	if (GetAsyncKeyState(VK_UP)) {
		posX += cosf(radian + nine) * speed;
		posY += sinf(radian + nine) * speed;
		camUpdate = true;
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		posX -= cosf(radian + nine) * speed;
		posY -= sinf(radian + nine) * speed;
		camUpdate = true;
	}
	

	if (GetAsyncKeyState(VK_PRIOR)) scale += 0.01f;
	if (GetAsyncKeyState(VK_NEXT)) scale -= 0.01f;

	if (camUpdate)
	{
		// Apply Transform
		cam.SetPosition(posX, posY);
		cam.SetRotation(degree);
	}

	// Draw
	SetColor(255, 0, 0);

	DrawGameObject(gameObject1, cam.inverseMatrix);
	DrawGameObject(gameObject2, cam.inverseMatrix);
	DrawGameObject(gameObject3, cam.inverseMatrix);
	DrawGameObject(gameObject4, cam.inverseMatrix);
	DrawGameObject(gameObject5, cam.inverseMatrix);

	
	// Buffer Swap 
	BufferSwap();
}