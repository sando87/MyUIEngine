#include "jViewImage.h"
#include "jCacheMgr.h"
#include "jUISystem.h"


jViewImage::jViewImage()
{
	Type = jViewType::Image;

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
	jView::OnLoad();
	mTexture = jUIBitmap::Cache(Image.filename);

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
