#include "jViewFont.h"
#include "jUISystem.h"


jViewFont::jViewFont()
{
	Type = jViewType::Font;

	Text = "text";
	FontName = "malgun.ttf";
	FontSize = 20;
	Style = jFontStyle::Regular;
	Align = jFontAlign::Center;
	ColorFont = jColor(0, 0, 0, 255);
}


jViewFont::~jViewFont()
{
}

void jViewFont::OnLoad()
{
	wstring uniText(Text.begin(), Text.end());
	mFont.Load(GetForm()->GetResourcePath() + FontName, FontSize, uniText, GetForm());
	jView::OnLoad();

	Point2 pos;
	Point2 size = mFont.Size();
	double centerOffX = mRectAbsolute.Left() + (mRectAbsolute.Width() * 0.5) - (size.x * 0.5);
	double rightOffX = mRectAbsolute.Right() - size.x;
	double centerOffY = mRectAbsolute.Top() + (mRectAbsolute.Height() * 0.5) - (size.y * 0.5);
	double bottomOffY = mRectAbsolute.Bottom() - size.y;
	switch (Align)
	{
	case jFontAlign::TopLeft:			pos = Point2(mRectAbsolute.Left(), mRectAbsolute.Top()); break;
	case jFontAlign::Top:				pos = Point2(centerOffX, mRectAbsolute.Top()); break;
	case jFontAlign::TopRight:		pos = Point2(rightOffX, mRectAbsolute.Top()); break;
	case jFontAlign::Left:				pos = Point2(mRectAbsolute.Left(), centerOffY); break;
	case jFontAlign::Center:			pos = Point2(centerOffX, centerOffY); break;
	case jFontAlign::Right:			pos = Point2(rightOffX, centerOffY); break;
	case jFontAlign::BottomLeft:		pos = Point2(mRectAbsolute.Left(), bottomOffY); break;
	case jFontAlign::Bottom:			pos = Point2(centerOffX, bottomOffY); break;
	case jFontAlign::BottomRight:	pos = Point2(rightOffX, bottomOffY); break;
	default:
		break;
	}
	mRectText.SetPosSize(pos, size);

	mRenderParam.rect = mRectText;
	mRenderParam.uv = jRectangle(0, 0, 1, 1);
	mRenderParam.color = ColorFont;
	mRenderParam.texture = mFont.Texture();
}

void jViewFont::OnDraw()
{
	GetForm()->EventDrawTexture(mRenderParam);
}

void jViewFont::OnSerialize(Json::Value & node)
{
	jView::OnSerialize(node);

	node["Text"] = Text;
	node["FontName"] = FontName;
	node["FontSize"] = FontSize;
	node["Style"] = Style;
	node["#Style"] = "Enum&Regular,Bold,Italic,Underline,Strikeout";
	node["Align"] = Align;
	node["#Align"] = "Enum&TopLeft,Top,TopRight,Left,Center,Right,BottomLeft,Bottom,BottomRight";
	Json::Value color;
	color.append(ColorFont.r);
	color.append(ColorFont.g);
	color.append(ColorFont.b);
	color.append(ColorFont.a);
	node["ColorFont"] = color;
	node["#ColorFont"] = "Color";
}

void jViewFont::OnDeserialize(Json::Value & node)
{
	jView::OnDeserialize(node);

	Text = node["Text"].asString();
	FontName = node["FontName"].asString();
	FontSize = node["FontSize"].asInt();
	Style = (jFontStyle)node["Style"].asInt();
	Align = (jFontAlign)node["Align"].asInt();
	ColorFont.r = node["ColorFont"][0].asUInt();
	ColorFont.g = node["ColorFont"][1].asUInt();
	ColorFont.b = node["ColorFont"][2].asUInt();
	ColorFont.a = node["ColorFont"][3].asUInt();
}
