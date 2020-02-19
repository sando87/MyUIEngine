#include "common.h"
#include "jUISystem.h"

unsigned int ToKey(const char *buf, int size)
{
	unsigned int key = 0;
	int shift = 0;
	for (int i = 0; i < size; ++i)
	{
		key ^= (unsigned int)buf[i] << shift;
		shift = (shift + 8) % 32;
	}
	return key;
}

jUIBitmap::jUIBitmap()
{
	width = 0;;
	height = 0;;
	byteperpixel = 0;
	buf.clear();
	fullname = "";
	texture = nullptr;
}

jUIBitmap::~jUIBitmap()
{
	if (texture)
	{
		jUISystem::GetInst()->OpReleaseTexture(texture);
		texture = nullptr;
	}
}

bool jUIBitmap::Load()
{
	if (texture)
	{
		jUISystem::GetInst()->OpReleaseTexture(texture);
		texture = nullptr;
	}
	texture = jUISystem::GetInst()->OpLoadTexture(this);
	return texture ? true : false;
}

void DrawingParams::Clip(jRectangle _rect)
{
	jRectangle clipRect = rect.Intersection(_rect);
	double rateL = (clipRect.Left() - rect.Left()) / rect.Width();
	double rateR = (clipRect.Right() - rect.Left()) / rect.Width();
	double rateT = (clipRect.Top() - rect.Top()) / rect.Height();
	double rateB = (clipRect.Bottom() - rect.Top()) / rect.Height();
	double uvL = uv.Left() + uv.Width() * rateL;
	double uvR = uv.Left() + uv.Width() * rateR;
	double uvT = uv.Top() + uv.Height() * rateT;
	double uvB = uv.Top() + uv.Height() * rateB;
	jRectangle clipUV;
	clipUV.SetPosSize(Point2(uvL, uvT), Point2(uvR - uvL, uvB - uvT));

	rect = clipRect;
	uv = clipUV;
}
