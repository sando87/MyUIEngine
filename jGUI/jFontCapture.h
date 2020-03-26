#pragma once
#include <string>
#include <vector>
#include "common.h"
using namespace std;

class jUISystem;

class jFontCapture
{
public:
	jFontCapture();
	~jFontCapture();

	void Load(string fontFullname, int fontSize, wstring text_unicode, jUISystem* form);
	Point2 Size() { return Point2(mImage->width, mImage->height); }
	void * Texture() { return mImage->texture; }

private:
	jUIBitmap *mImage;

	string mFontname;
	int mFontsize;
	wstring mText;
	unsigned int mKey;

	unsigned int CalcKey();
	
};

