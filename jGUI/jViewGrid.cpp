#include "jViewGrid.h"
#include "jUISystem.h"
#include "jViewImage.h"

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

	mStepWidth = 0;
	mStepHeight = 0;
	mPreIdx = -1;
}

jViewGrid::~jViewGrid()
{
}

void jViewGrid::OnLoad()
{
	jView::OnLoad();
	mStepWidth = GetWidth() / ColumnCount;
	mStepHeight = GetHeight() / RowCount;

	mGridChilds.resize(ColumnCount * RowCount);
	for (int y = 0; y < RowCount; ++y)
	{
		for (int x = 0; x < ColumnCount; ++x)
		{
			int localX = int(x * mStepWidth);
			int localY = int(y * mStepHeight);
			int idx = y * ColumnCount + x;
			jViewImage* imageView = (jViewImage*)jUISystem::GetInst()->CreateView(jViewType::Image);
			imageView->LocalX = to_string(localX);
			imageView->LocalY = to_string(localY);
			imageView->Width = to_string((int)mStepWidth);
			imageView->Height = to_string((int)mStepHeight);
			imageView->Image = ImageCell;
			imageView->Enable = false;
			imageView->UserData["Idx"] = idx;
			AddChild(imageView);
			mGridChilds[idx] = imageView;
		}
	}

	mRenderParam.rect = mRectAbsolute;
	mRenderParam.color = jColor(0, 0, 0, 0);
	mRenderParam.texture = nullptr;
	mRenderParam.uv = jRectangle(0, 0, 0, 0);

	EventMouseClick = [&](jView* view, Point2 pt) {
		int idx = ToIndex(pt - mRectAbsolute.GetMin());
		if (EventGridClick != nullptr)
			EventGridClick(mGridChilds[idx], pt);
	};

	EventMouseEnter = [&](jView* view, Point2 pt) {
		int idx = ToIndex(pt - mRectAbsolute.GetMin());
		if (EventGridEnter != nullptr)
			EventGridEnter(mGridChilds[idx], pt);
		mPreIdx = idx;
	};
	EventMouseMove = [&](jView* view, Point2 pt) {
		int idx = ToIndex(pt - mRectAbsolute.GetMin());
		if (idx != mPreIdx)
		{
			if (EventGridLeave != nullptr)
				EventGridLeave(mGridChilds[mPreIdx], pt);
			if (EventGridEnter != nullptr)
				EventGridEnter(mGridChilds[idx], pt);
			mPreIdx = idx;
		}
	};
	EventMouseLeave = [&](jView* view, Point2 pt) {
		if (EventGridLeave != nullptr)
			EventGridLeave(mGridChilds[mPreIdx], pt);
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
	ImageCell.right = imgNode["right"].asFloat();
	ImageCell.top = imgNode["top"].asFloat();
	ImageCell.bottom = imgNode["bottom"].asFloat();
}

jView* jViewGrid::FindEmptyChild()
{
	for (jView* view : mGridChilds)
		if (view->Childs.empty())
			return view;
	return nullptr;
}
jView* jViewGrid::GetChild(unsigned int idx)
{
	_errorIf(mGridChilds.size() <= idx);
	return mGridChilds[idx];
}

bool jViewGrid::IsEmpty(unsigned int idx)
{
	_errorIf(mGridChilds.size() <= idx);
	jView* view = mGridChilds[idx];
	return view->Childs.empty() ? true : false;
}

int jViewGrid::ToIndex(Point2 local)
{
	int idxX = int(local.x / mStepWidth);
	int idxY = int(local.y / mStepHeight);
	return idxY * ColumnCount + idxX;
}
