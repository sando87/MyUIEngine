#pragma once
#include "jView.h"
class jViewImage :
	public jView
{
public:
	jViewImage();
	virtual ~jViewImage();

	PropImage Image;
protected:
	jUIBitmap *mTexture;

	virtual void OnLoad();
	virtual void OnDraw();
	virtual void OnSerialize(Json::Value & node);
	virtual void OnDeserialize(Json::Value & node);

};

