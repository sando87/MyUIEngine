#include "jViewGrid.h"
#include "jUISystem.h"

jViewGrid::jViewGrid()
{
	Type = jViewType::Grid;

	ImageCell.filename = "defaultImage.png";
	ImageCell.left = 0.0f;
	ImageCell.right = 1.0f;
	ImageCell.top = 0.0f;
	ImageCell.bottom = 1.0f;
	ColumnCount = 1;
	RowCount = 1;

	mTexture = nullptr;
	mStepWidth = Width;
	mStepHeight = Height;
}

jViewGrid::~jViewGrid()
{
}

void jViewGrid::OnLoad()
{
	jView::OnLoad();
	mTexture = jUIBitmap::Cache(ImageCell.filename);
	mStepWidth = Width / ColumnCount;
	mStepHeight = Height / RowCount;

	mRenderParam.rect = mRectAbsolute;
	mRenderParam.color = jColor(255, 255, 255, 255);
	mRenderParam.texture = mTexture ? mTexture->texture : nullptr;
	mRenderParam.uv = jRectangle(0, 0, ColumnCount, RowCount);

	EventMouseClick = [&](Point2 pt) {
		int idxX = (int)((pt.x - mRectAbsolute.GetMin().x) / mStepWidth);
		int idxY = (int)((pt.y - mRectAbsolute.GetMin().y) / mStepHeight);
		int idx = idxY * ColumnCount + idxX;
		if (EventGridClick != nullptr)
			EventGridClick(idx);
	};
	EventMouseMove = [&](Point2 pt) {
		int idxX = (int)((pt.x - mRectAbsolute.GetMin().x) / mStepWidth);
		int idxY = (int)((pt.y - mRectAbsolute.GetMin().y) / mStepHeight);
		int idx = idxY * ColumnCount + idxX;
		if (EventGridHover != nullptr)
			EventGridHover(idx);
	};
}

void jViewGrid::OnDraw()
{
	jUISystem::GetInst()->EventDrawTexture(mRenderParam);
}

void jViewGrid::OnSerialize(Json::Value & node)
{
	jView::OnSerialize(node);

	Json::Value imgNode;
	imgNode["filename"] = ImageCell.filename;
	imgNode["left"] = ImageCell.left;
	imgNode["right"] = ImageCell.right;
	imgNode["top"] = ImageCell.top;
	imgNode["bottom"] = ImageCell.bottom;
	node["ImageCell"] = imgNode;
	node["ColumnCount"] = ColumnCount;
	node["RowCount"] = RowCount;
}

void jViewGrid::OnDeserialize(Json::Value & node)
{
	jView::OnDeserialize(node);

	ColumnCount = node["ColumnCount"].asInt();
	RowCount = node["RowCount"].asInt();
	Json::Value imgNode = node["ImageCell"];
	ImageCell.filename = imgNode["filename"].asString();
	ImageCell.left = imgNode["left"].asFloat();
	ImageCell.right = (float)ColumnCount;
	ImageCell.top = imgNode["top"].asFloat();
	ImageCell.bottom = (float)RowCount;
}


void jViewGrid::AddChild(jView *child)
{
	int idxX = child->LocalX / mStepWidth;
	int idxY = child->LocalY / mStepHeight;
	int idx = idxY * ColumnCount + idxX;
	_errorIf(mGridChilds.find(idx) != mGridChilds.end());

	mGridChilds[idx] = child;
	child->LocalX = idxX * mStepWidth;
	child->LocalY = idxY * mStepHeight;
	jView::AddChild(child);
		
}
void jViewGrid::SubChild(jView *child)
{
	int idx = FindViewOnGrid(child);
	_errorIf(idx < 0);

	mGridChilds.erase(idx);
	jView::SubChild(child);
}
void jViewGrid::MoveTo(int idx, jView *movingChild)
{
	int preIdx = FindViewOnGrid(movingChild);
	_errorIf(preIdx < 0);

	mGridChilds.erase(preIdx);
	
	if (mGridChilds.find(idx) != mGridChilds.end())
		mGridChilds[preIdx] = mGridChilds[idx];

	mGridChilds[idx] = movingChild;
}
jView* jViewGrid::GetChild(int idx)
{
	if (mGridChilds.find(idx) == mGridChilds.end())
		return nullptr;

	return mGridChilds[idx];
}
jView* jViewGrid::GetChild(int idxX, int idxY)
{
	int idx = idxY * ColumnCount + idxX;
	if (mGridChilds.find(idx) == mGridChilds.end())
		return nullptr;

	return mGridChilds[idx];
}
jView* jViewGrid::GetChild(Point2 pt)
{
	int idxX = (int)(pt.x / mStepWidth);
	int idxY = (int)(pt.y / mStepHeight);
	int idx = idxY * ColumnCount + idxX;
	if (mGridChilds.find(idx) == mGridChilds.end())
		return nullptr;

	return mGridChilds[idx];
}
int jViewGrid::FindViewOnGrid(jView* view)
{
	int idxX = view->LocalX / mStepWidth;
	int idxY = view->LocalY / mStepHeight;
	int idx = idxY * ColumnCount + idxX;
	if (mGridChilds.find(idx) == mGridChilds.end())
		return -1;

	return idx;
}
