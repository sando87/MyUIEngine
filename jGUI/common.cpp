#include "common.h"
#include "jUISystem.h"
#include "jCacheMgr.h"

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

jUIBitmap * jUIBitmap::Cache(string filename)
{
	if (filename.length() <= 0)
		return nullptr;

	unsigned int key = ToKey(filename.c_str(), filename.length());
	jUIBitmap *texture = (jUIBitmap *)jCacheMgr::GetInst().Cache(key, [&]() {
		jUIBitmap *bitmap = new jUIBitmap();
		bitmap->fullname = jUISystem::GetInst()->GetResourcePath() + filename;
		if (bitmap->Load() == false)
		{
			delete bitmap;
			bitmap = nullptr;
		}
		return bitmap;
	});
	return texture;
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
