#pragma once

#pragma warning(disable: 4267)

#include "jRectangle.h"
#include <string>
#include <stdio.h>
#include <vector>
using namespace std;

#define _errorTrace() printf("error [%s] [%d]\n", __FILE__, __LINE__)


enum jViewType
{
	View, Button, Image, Font
};
enum jFontAlign
{
	TopLeft, Top, TopRight,
	Left, Center, Right,
	BottomLeft, Bottom, BottomRight,
};
enum jFontStyle
{
	Regular,
	Bold,
	Italic,
	Underline,
	Strikeout
};

struct jColor
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
	jColor() { r = 0; g = 0; b = 0; a = 0; }
	jColor(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) { r = _r; g = _g; b = _b; a = _a; }
};

struct jUIBitmap
{
public:
	int width;
	int height;
	int byteperpixel;
	vector<jColor> buf;
	string fullname;
	void *texture;
	jUIBitmap();
	~jUIBitmap();
	bool Load();
};

struct PropImage
{
	string filename;
	float left;
	float right;
	float top;
	float bottom;
};

struct DrawingParams
{
public:
	jRectangle rect;
	jRectangle uv;
	jColor color;
	void * texture;
	void Clip(jRectangle _rect);
};

unsigned int ToKey(const char *buf, int size);
