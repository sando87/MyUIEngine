#include "jRectangle.h"



jRectangle::jRectangle()
{
}


jRectangle::~jRectangle()
{
}

jRectangle::jRectangle(double x, double y, double w, double h)
{
	SetPosSize(Point2(x, y), Point2(w, h));
}

void jRectangle::SetMinMax(Point2 min, Point2 max)
{
	mMin = min;
	mMax = max;
	mMax.x = fmax(mMin.x, mMax.x);
	mMax.y = fmax(mMin.y, mMax.y);
}

void jRectangle::SetPosSize(Point2 pos, Point2 size)
{
	mMin = pos;
	mMax = pos + size;
	mMax.x = fmax(mMin.x, mMax.x);
	mMax.y = fmax(mMin.y, mMax.y);
}

void jRectangle::SetCenter(Point2 pos)
{
	Point2 halfSize = GetSize() / 2;
	mMin = pos - halfSize;
	mMax = pos + halfSize;
	mMax.x = fmax(mMin.x, mMax.x);
	mMax.y = fmax(mMin.y, mMax.y);
}

void jRectangle::Zoom(double rate)
{
	Point2 sized = GetSize() * rate;
	SetPosSize(GetPos(), sized);
}

bool jRectangle::Contains(Point2 point)
{
	return mMin.x <= point.x && point.x < mMax.x && mMin.y <= point.y && point.y < mMax.y;
}

jRectangle jRectangle::Intersection(jRectangle & rect)
{
	jRectangle inter;
	double top = fmax(Top(), rect.Top());
	double bottom = fmin(Bottom(), rect.Bottom());
	double left = fmax(Left(), rect.Left());
	double right = fmin(Right(), rect.Right());
	inter.SetMinMax(Point2(left, top), Point2(right, bottom));
	return inter;
}

jRectangle jRectangle::ClipRate(float rateL, float rateR, float rateT, float rateB)
{
	double left = mMin.x + (Width() * rateL);
	left = left > mMax.x ? mMax.x : left;
	double right = mMax.x - (Width() * rateR);
	right = right < mMin.x ? mMin.x : right;
	double top = mMin.y + (Height() * rateT);
	top = top > mMax.y ? mMax.y : top;
	double bottom = mMax.y - (Height() * rateB);
	bottom = bottom < mMin.y ? mMin.y : bottom;
	return jRectangle(left, top, right - left, bottom - top);
}
