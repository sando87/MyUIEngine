#pragma once
#include "Point2.h"

class jRectangle
{
public:
	jRectangle();
	~jRectangle();
	jRectangle(double x, double y, double w, double h);

	void SetMinMax(Point2 min, Point2 max);
	void SetPosSize(Point2 pos, Point2 size);
	void SetCenter(Point2 pos);
	void Zoom(double rate);
	bool Contains(Point2 point);
	jRectangle Intersection(jRectangle & rect);
	jRectangle ClipRate(float rateL, float rateR, float rateT, float rateB);

	Point2 GetMin() { return mMin; }
	Point2 GetMax() { return mMax; }
	Point2 GetSize() { return mMax - mMin; }
	Point2 GetPos() { return mMin; }
	Point2 GetCenter() { return mMin + (GetSize() / 2); }

	double Top() { return mMin.y; }
	double Bottom() { return mMax.y; }
	double Left() { return mMin.x; }
	double Right() { return mMax.x; }
	double Width() { return mMax.x - mMin.x; }
	double Height() { return mMax.y - mMin.y; }

private:
	Point2 mMin;
	Point2 mMax;
};

