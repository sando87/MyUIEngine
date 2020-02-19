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
	return mMin.x <= point.x && point.x <= mMax.x && mMin.y <= point.y && point.y <= mMax.y;
}

jRectangle jRectangle::Intersection(jRectangle & rect)
{
	jRectangle inter;
	double top = fmax(Top(), rect.Top());
	double bottom = fmax(Bottom(), rect.Bottom());
	double left = fmax(Left(), rect.Left());
	double right = fmax(Right(), rect.Right());
	inter.SetMinMax(Point2(left, top), Point2(right, bottom));
	return inter;
}
