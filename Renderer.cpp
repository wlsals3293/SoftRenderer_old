
#include "stdafx.h"
#include "SoftRenderer.h"
#include "GDIHelper.h"
#include "Renderer.h"

#include "Vector.h"
#include "IntPoint.h"
#include "Matrix.h"

bool IsInRange(int x, int y);
void PutPixel(int x, int y);

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


void UpdateFrame(void)
{
	// Buffer Clear
	SetColor(32, 128, 255);
	Clear();

	// Draw
	SetColor(255, 0, 0);
	
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

	static float degree = 0;
	degree += 1.0f;
	degree = fmodf(degree, 360.0f);

	Vector3 moveRadius(150.0f, 150.0f);

	// 원
	Vector3 center(0.0f, 0.0f);
	float radius = 100.0f;
	int nradius = (int)radius;

	static int centerCircleR = 255;
	static int centerCircleG = 0;
	static int centerCircleB = 0;

	centerCircleR += 1;
	centerCircleG += 2;
	centerCircleB += 3;

	if (centerCircleR >= 256)
		centerCircleR = 0;
	if (centerCircleG >= 256)
		centerCircleG = 0;
	if (centerCircleB >= 256)
		centerCircleB = 0;

	SetColor(centerCircleR, centerCircleG, centerCircleB);

	float sinVal = sinf(Deg2Rad(degree));

	float posX = sinVal * moveRadius.X;
	float posY = cosf(Deg2Rad(degree)) * moveRadius.Y;

	Matrix3 translationMat;
	translationMat.SetTranslation(posX, posY);

	Matrix3 rotMat;
	rotMat.SetRotation(degree);

	Matrix3 scaleMat;
	scaleMat.SetScale(sinVal, sinVal, sinVal);


	Matrix3 TRSMat = translationMat * rotMat;


	for (int i = -nradius; i < nradius; i++) {
		for (int j = 0/*-nradius*/; j < nradius; j++) {
			Vector3 ptVec(i, j);
			if (Vector3::DistSquared(center, ptVec) <= radius * radius) {
				PutPixel(ptVec * TRSMat);
			}
		}
	}



	// 사각형
	sinVal = sinf(Deg2Rad(fmodf(degree + 180, 360)));
	posX = sinVal * moveRadius.X;
	posY = cosf(Deg2Rad(fmodf(degree + 180, 360))) * moveRadius.Y;
	translationMat.SetTranslation(posX, posY);

	Vector3 scale(80.0f, 80.0f);
	static float squareDegree = 0;
	squareDegree += 2.0f;

	rotMat.SetRotation(squareDegree);
	rotMat.Transpose();

	scaleMat.SetScale(sinVal, sinVal, sinVal);


	Matrix3 TRSMat2 = translationMat * rotMat * scaleMat;

	for (int i = -scale.X; i < scale.X; i++)
	{
		for (int j = -scale.Y; j <= scale.Y; j++)
		{
			PutPixel(Vector3(i, j) * TRSMat2);
		}
	}

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


	// Buffer Swap 
	BufferSwap();
}
