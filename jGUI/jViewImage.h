#pragma once
#include "jView.h"
class jViewImage :
	public jView
{
public:
	jViewImage();
	virtual ~jViewImage();

	PropImage Image;

	double GetImageWidth() { return mTexture->width * (Image.right - Image.left); }
	double GetImageHeight() { return mTexture->height * (Image.bottom - Image.top); }
protected:
	jUIBitmap *mTexture;

	virtual void OnLoad();
	virtual void OnDraw();
	virtual void OnSerialize(Json::Value & node);
	virtual void OnDeserialize(Json::Value & node);

	virtual Point2 CalcSize();
};

