#pragma once
#include "jView.h"
#include "jFontCapture.h"

class jViewFont :
	public jView
{
public:
	jViewFont();
	virtual ~jViewFont();

	string Text;
	string FontName;
	int FontSize;
	jFontStyle Style;
	jFontAlign Align;
	jColor ColorFont;

protected:
	jFontCapture mFont;
	jRectangle mRectText;

	virtual void OnLoad();
	virtual void OnDraw();
	virtual void OnSerialize(Json::Value & node);
	virtual void OnDeserialize(Json::Value & node);
};

