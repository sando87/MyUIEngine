#pragma once
#include "jViewFont.h"

class jViewButton : public jViewFont
{
public:
	jViewButton();
	~jViewButton();

	PropImage Image;
	jColor ColorButton;
	jColor ColorOutline;
protected:
	jUIBitmap *mTexture;

	virtual void OnLoad();
	virtual void OnDraw();
	virtual void OnSerialize(Json::Value & node);
	virtual void OnDeserialize(Json::Value & node);
};

