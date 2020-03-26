#pragma once

#pragma warning(disable: 4267)

#include "jRectangle.h"
#include <string>
#include <stdio.h>
#include <vector>
using namespace std;

#define _errorIf(p)  if(p)printf("error [%s] [%d]\n", __FILE__, __LINE__)
#define _errorTrace() printf("error [%s] [%d]\n", __FILE__, __LINE__)

class jUISystem;

enum jViewType
{
	View, Button, Image, Font, Grid
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
	jUIBitmap(jUISystem* form);
	~jUIBitmap();

	int width;
	int height;
	int byteperpixel;
	vector<jColor> buf;
	string fullname;
	void *texture;
	bool Load();
	static jUIBitmap* Cache(string filename, jUISystem* form);
private:
	jUISystem * mFrom;
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
