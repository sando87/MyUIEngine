#include "jViewButton.h"
#include "jCacheMgr.h"
#include "jUISystem.h"

jViewButton::jViewButton()
{
	Type = jViewType::Button;

	Image.filename = "defaultImage.png";
	Image.left = 0.0f;
	Image.right = 1.0f;
	Image.top = 0.0f;
	Image.bottom = 1.0f;
	ColorButton = jColor(225, 225, 225, 255);
	ColorOutline = jColor(173, 173, 173, 255);

	mTexture = nullptr;
}


jViewButton::~jViewButton()
{
}

void jViewButton::OnLoad()
{
	mTexture = jUIBitmap::Cache(Image.filename);
	jViewFont::OnLoad();

	if (mTexture)
	{
		mRenderParam.rect = mRectAbsolute;
		mRenderParam.color = jColor(220, 220, 220, 255);
		mRenderParam.texture = mTexture->texture;
		mRenderParam.uv = jRectangle(Image.left, Image.top, Image.right - Image.left, Image.bottom - Image.top);
	}
	else
	{
		mRenderParam.rect = mRectAbsolute;
		mRenderParam.color = ColorButton;
		mRenderParam.texture = mFont.Texture();
		mRenderParam.uv = jRectangle(0, 0, 1, 1);
	}
}

void jViewButton::OnDraw()
{
	if (mTexture)
	{
		DrawingParams params = mRenderParam;
		if (mDowned)
		{
			Point2 downedPos = mRectAbsolute.GetPos() + Point2(1, 1);
			params.rect.SetPosSize(downedPos, mRectAbsolute.GetSize());
			params.color = jColor(200, 200, 200, 255);
		}
		else if (mHovered)
		{
			mRenderParam.rect = mRectAbsolute;
			params.color = jColor(255, 255, 255, 255);
		}
		else
		{
			mRenderParam.rect = mRectAbsolute;
			params.color = jColor(220, 220, 220, 255);
		}

		jUISystem::GetInst()->EventDrawTexture(params);
	}
	else
	{
		DrawingParams params = mRenderParam;
		if (mDowned)
		{
			params.color = jColor(ColorButton.r - 20, ColorButton.g - 20, ColorButton.b - 20, ColorButton.a);
			jUISystem::GetInst()->EventDrawFill(params);
			params.color = ColorOutline;
			jUISystem::GetInst()->EventDrawOutline(params);
		}
		else if (mHovered)
		{
			params.color = jColor(ColorButton.r + 5 , ColorButton.g + 5, ColorButton.b + 5, ColorButton.a);
			jUISystem::GetInst()->EventDrawFill(params);
			params.color = jColor(ColorOutline.r + 5, ColorOutline.g + 5, ColorOutline.b + 5, ColorOutline.a);;
			jUISystem::GetInst()->EventDrawOutline(params);
		}
		else
		{
			jUISystem::GetInst()->EventDrawFill(params);
			params.color = ColorOutline;
			jUISystem::GetInst()->EventDrawOutline(params);
		}

		if (mFont.Texture() != nullptr)
		{
			params.rect = mRectText;
			params.color = ColorFont;
			jUISystem::GetInst()->EventDrawTexture(params);
		}
	}
}

void jViewButton::OnSerialize(Json::Value & node)
{
	jViewFont::OnSerialize(node);

	Json::Value imgNode;
	imgNode["filename"] = Image.filename;
	imgNode["left"] = Image.left;
	imgNode["right"] = Image.right;
	imgNode["top"] = Image.top;
	imgNode["bottom"] = Image.bottom;
	node["Image"] = imgNode;

	Json::Value colorBtn;
	colorBtn.append(ColorButton.r);
	colorBtn.append(ColorButton.g);
	colorBtn.append(ColorButton.b);
	colorBtn.append(ColorButton.a);
	node["ColorButton"] = colorBtn;

	Json::Value colorOutline;
	colorOutline.append(ColorOutline.r);
	colorOutline.append(ColorOutline.g);
	colorOutline.append(ColorOutline.b);
	colorOutline.append(ColorOutline.a);
	node["ColorOutline"] = colorOutline;
}

void jViewButton::OnDeserialize(Json::Value & node)
{
	jViewFont::OnDeserialize(node);

	Json::Value imgNode = node["Image"];
	Image.filename = imgNode["filename"].asString();
	Image.left = imgNode["left"].asFloat();
	Image.right = imgNode["right"].asFloat();
	Image.top = imgNode["top"].asFloat();
	Image.bottom = imgNode["bottom"].asFloat();

	ColorButton.r = node["ColorButton"][0].asUInt();
	ColorButton.g = node["ColorButton"][1].asUInt();
	ColorButton.b = node["ColorButton"][2].asUInt();
	ColorButton.a = node["ColorButton"][3].asUInt();

	ColorOutline.r = node["ColorOutline"][0].asUInt();
	ColorOutline.g = node["ColorOutline"][1].asUInt();
	ColorOutline.b = node["ColorOutline"][2].asUInt();
	ColorOutline.a = node["ColorOutline"][3].asUInt();
}
