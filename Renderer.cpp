
#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"

#include "Vector.h"
#include "Vertex.h"
#include "IntPoint.h"

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

void DrawTriangle(const Vertex &v1, const Vertex &v2, const Vertex &v3, const Matrix3 trsMat)
{
	Vector2 minPos = Vector2(INFINITY, INFINITY);
	Vector2 maxPos = Vector2(-INFINITY, -INFINITY);

	Vector3 p1 = v1.position * trsMat;
	Vector3 p2 = v2.position * trsMat;
	Vector3 p3 = v3.position * trsMat;

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
				/*BYTE RV1 = GetRValue(v1.color);
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
				BYTE FinalB = (BYTE)(BV1 * (1 - s - t) + BV2 * s + BV3 * t);*/

				if (g_Texture->IsLoaded()) {
					Vector2 baryUV = v1.uv * (1 - s - t) + v2.uv * s + v3.uv * t;
					ULONG color = g_Texture->GetTexturePixel(baryUV);

					BYTE FinalR = GetRValue(color);
					BYTE FinalG = GetGValue(color);
					BYTE FinalB = GetBValue(color);

					SetColor(FinalR, FinalG, FinalB);
					PutPixel(pt);
				}

				
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

void DrawGameObject(const GameObject2D &obj) {
	// Define Matrix
	Matrix3 tMat, rMat, sMat, trsMat;
	sMat.SetScale(obj.Transform.Scale.X, obj.Transform.Scale.Y, 1);
	rMat.SetRotation(obj.Transform.Angle);
	tMat.SetTranslation(obj.Transform.Position.X, obj.Transform.Position.Y);
	trsMat = tMat * rMat * sMat;

	for (int i = 0; i < obj.Mesh.TriangleCount; i++)
	{
		DrawTriangle(
			obj.Mesh.Triangles[i].vt[0],
			obj.Mesh.Triangles[i].vt[1],
			obj.Mesh.Triangles[i].vt[2],
			trsMat);
	}
}


void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	
	static float degree = 0;
	static float posX = 0;
	static float posY = 0;
	static float scale = 1;

	// Input 
	if (GetAsyncKeyState(VK_LEFT)) degree += 1;
	if (GetAsyncKeyState(VK_RIGHT)) degree -= 1;

	float radian = Deg2Rad(degree);
	float speed = 5;

	if (GetAsyncKeyState(VK_UP)) {
		posX += cosf(radian) * speed;
		posY += sinf(radian) * speed;
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		posX -= cosf(radian) * speed;
		posY -= sinf(radian) * speed;
	}
	if (GetAsyncKeyState(VK_PRIOR)) scale += 0.01f;
	if (GetAsyncKeyState(VK_NEXT)) scale -= 0.01f;

	gameObject1.Transform.Angle = degree;
	gameObject1.Transform.Position.X = posX;
	gameObject1.Transform.Position.Y = posY;
	gameObject1.Transform.Scale.X = scale;
	gameObject1.Transform.Scale.Y = scale;

	SetColor(255, 0, 0);

	DrawGameObject(gameObject1);


	/*static float xPos = 0;
	static float yPos = 0;
	static float degree = 0;
	static float scale = 1;*/

	// Input
	/*if (GetAsyncKeyState(VK_LEFT)) degree += 1;
	if (GetAsyncKeyState(VK_RIGHT)) degree -= 1;

	float radian = Deg2Rad(degree);

	if (GetAsyncKeyState(VK_UP)) {
		xPos += cosf(radian);
		yPos += sinf(radian);
	}
	if (GetAsyncKeyState(VK_DOWN)) {
		xPos -= cosf(radian);
		yPos -= sinf(radian);
	}

	if (GetAsyncKeyState(VK_PRIOR)) scale += 0.1f;
	if (GetAsyncKeyState(VK_NEXT)) scale -= 0.1f;*/



	// Define Matrix
	//Matrix3 tMat;
	//tMat.SetTranslation(xPos, yPos);

	//Matrix3 rMat;
	//rMat.SetRotation(degree);

	//Matrix3 sMat;
	//sMat.SetScale(scale, scale, scale);

	//Matrix3 trsMat;
	//trsMat = tMat * rMat * sMat;

	//// Draw
	//SetColor(255, 0, 0);

	// Define Vector Space
	//Vector3 start = Vector3::Make2DPoint(-20.0f, -10.0f) * trsMat;
	//Vector3 end = Vector3::Make2DPoint(10.0f, 10.0f) * trsMat;
	//DrawLine(start, end);


	/*Vector3 p1 = Vector3::Make2DPoint(0, 50);
	Vector3 p2 = Vector3::Make2DPoint(-30, -10);
	Vector3 p3 = Vector3::Make2DPoint(40, 20);
	DrawTriangle(p1, p2, p3);*/

	/*float radius = 100.f;
	int nradius = (int)radius;
	for (int i = -nradius; i <= nradius; i++)
	{
		for (int j = -nradius; j <= nradius; j++)
		{
			PutPixel(Vector3((float)i, (float)j) * tsMat);
		}
	}*/

	// Buffer Swap 
	BufferSwap();
}


// Draw a filled circle with radius 100
/*Vector2 center(0.0f, 0.0f);
float radius = 100.0f;
int nradius = (int)radius;

static float degree = 0;
degree += 0.1f;
degree = fmodf(degree, 360.0f);

Matrix3 rotMat;
rotMat.SetRotation(degree);
rotMat.Transpose();


static float move = 1;
move += 1;
float theta = fmodf(move, 360);
float pos = sinf(Deg2Rad(theta)) * 150;


float scale = sinf(Deg2Rad(theta)) + 1;
Matrix3 scaleMat;
scaleMat.SetScale(scale, scale, scale);


Matrix3 translationMat;
translationMat.SetTranslation(pos, pos);

Matrix3 SR = scaleMat * rotMat;
Matrix3 TRS = translationMat * rotMat * scaleMat;

for (int i = -nradius; i < nradius; i++)
{
for (int j = -nradius; j <= nradius; j++)
{
PutPixel(Vector3(i, j) * TRS);
}
}*/

//static float degree = 0;
//degree += 1.0f;
//degree = fmodf(degree, 360.0f);

//Vector3 moveRadius(150.0f, 150.0f);

//// 원
//Vector3 center(0.0f, 0.0f);
//float radius = 100.0f;
//int nradius = (int)radius;

//static int centerCircleR = 255;
//static int centerCircleG = 0;
//static int centerCircleB = 0;

//centerCircleR += 1;
//centerCircleG += 2;
//centerCircleB += 3;

//if (centerCircleR >= 256)
//	centerCircleR = 0;
//if (centerCircleG >= 256)
//	centerCircleG = 0;
//if (centerCircleB >= 256)
//	centerCircleB = 0;

//SetColor(centerCircleR, centerCircleG, centerCircleB);

//float sinVal = sinf(Deg2Rad(degree));

//float posX = sinVal * moveRadius.X;
//float posY = cosf(Deg2Rad(degree)) * moveRadius.Y;

//Matrix3 translationMat;
//translationMat.SetTranslation(posX, posY);

//Matrix3 rotMat;
//rotMat.SetRotation(degree);

//Matrix3 scaleMat;
//scaleMat.SetScale(sinVal, sinVal, sinVal);


//Matrix3 TRSMat = translationMat * rotMat;


//for (int i = -nradius; i < nradius; i++) {
//	for (int j = 0/*-nradius*/; j < nradius; j++) {
//		Vector3 ptVec(i, j);
//		if (Vector3::DistSquared(center, ptVec) <= radius * radius) {
//			PutPixel(ptVec * TRSMat);
//		}
//	}
//}



//// 사각형
//sinVal = sinf(Deg2Rad(fmodf(degree + 180, 360)));
//posX = sinVal * moveRadius.X;
//posY = cosf(Deg2Rad(fmodf(degree + 180, 360))) * moveRadius.Y;
//translationMat.SetTranslation(posX, posY);

//Vector3 scale(80.0f, 80.0f);
//static float squareDegree = 0;
//squareDegree += 2.0f;

//rotMat.SetRotation(squareDegree);
//rotMat.Transpose();

//scaleMat.SetScale(sinVal, sinVal, sinVal);


//Matrix3 TRSMat2 = translationMat * rotMat * scaleMat;

//for (int i = -scale.X; i < scale.X; i++)
//{
//	for (int j = -scale.Y; j <= scale.Y; j++)
//	{
//		PutPixel(Vector3(i, j) * TRSMat2);
//	}
//}

//Matrix2 scaleMat;
//scaleMat.SetScale(2.0f, 0.5f);

//Matrix2 rotMat;
//rotMat.SetRotation(30.0f);

//Matrix2 SRMat = scaleMat * rotMat;
//Matrix2 RSMat = rotMat * scaleMat;

//
//for (int i = -nradius; i < nradius; i++) {
//	for (int j = 0/*-nradius*/; j < nradius; j++) {
//		IntPoint pt(i, j);
//		Vector2 ptVec = pt.ToVector2();
//		if (Vector2::DistSquared(center, ptVec) <= radius * radius) {
//			//IntPoint scaledPt(ptVec * scaleMat);
//			//IntPoint rotateedPt(scaledPt.ToVector2() * rotMat);
//			//IntPoint SRPt(ptVec * SRMat);
//			IntPoint RSPt(ptVec * RSMat);
//			PutPixel(RSPt);
//		}
//	}
//}