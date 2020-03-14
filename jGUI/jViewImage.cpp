#include "jViewImage.h"
#include "jCacheMgr.h"
#include "jUISystem.h"


jViewImage::jViewImage()
{
	Type = jViewType::Image;

	//default size is same with image.
	Width = "";
	Height = "";

	Image.filename = "defaultImage.png";
	Image.left = 0.0f;
	Image.right = 1.0f;
	Image.top = 0.0f;
	Image.bottom = 1.0f;

	mTexture = nullptr;
}


jViewImage::~jViewImage()
{
}

void jViewImage::OnLoad()
{
	mTexture = jUIBitmap::Cache(Image.filename);
	jView::OnLoad();

	mRenderParam.rect = mRectAbsolute;
	mRenderParam.color = jColor(255, 255, 255, 255);
	mRenderParam.texture = mTexture ? mTexture->texture : nullptr;
	mRenderParam.uv = jRectangle(Image.left, Image.top, Image.right - Image.left, Image.bottom - Image.top);
}

void jViewImage::OnDraw()
{
	jUISystem::GetInst()->EventDrawTexture(mRenderParam);
}

void jViewImage::OnSerialize(Json::Value & node)
{
	jView::OnSerialize(node);

	Json::Value imgNode;
	imgNode["filename"] = Image.filename;
	imgNode["left"] = Image.left;
	imgNode["right"] = Image.right;
	imgNode["top"] = Image.top;
	imgNode["bottom"] = Image.bottom;
	node["Image"] = imgNode;
}

void jViewImage::OnDeserialize(Json::Value & node)
{
	jView::OnDeserialize(node);

	Json::Value imgNode = node["Image"];
	Image.filename = imgNode["filename"].asString();
	Image.left = imgNode["left"].asFloat();
	Image.right = imgNode["right"].asFloat();
	Image.top = imgNode["top"].asFloat();
	Image.bottom = imgNode["bottom"].asFloat();
}

Point2 jViewImage::CalcSize()
{
	if (mParent == nullptr)
		return Point2(atoi(Width.c_str()), atoi(Height.c_str()));

	if (mTexture == nullptr)
		return jView::CalcSize();

	Point2 size;
	if (Width.length() == 0 && Height.length() == 0)
	{
		size.x = GetImageWidth();
		size.y = GetImageHeight();
	}
	else if (Height.length() == 0)
	{
		size.x = StringToPixel(Width, mParent->GetRectAbsolute().Width());
		size.y = size.x * (GetImageHeight() / GetImageWidth());
	}
	else if (Width.length() == 0)
	{
		size.y = StringToPixel(Height, mParent->GetRectAbsolute().Height());
		size.x = size.y * (GetImageWidth() / GetImageHeight());
	}
	else
	{
		size.x = StringToPixel(Width, mParent->GetRectAbsolute().Width());
		size.y = StringToPixel(Height, mParent->GetRectAbsolute().Height());
	}
	return size;
}
