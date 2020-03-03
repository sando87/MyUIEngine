#include <string>
#include <fstream>
#include <streambuf>

#include "jUISystem.h"
#include "jView.h"
#include "jViewButton.h"
#include "jViewFont.h"
#include "jViewImage.h"


jUISystem::jUISystem()
{
	mRootView = nullptr;
	mResourcePath = "./res/";
}


jUISystem::~jUISystem()
{
}

jView * jUISystem::ParseJson(string filename)
{
	if (mRootView)
		delete mRootView;

	string fullname = mResourcePath + filename;
	string jsonText = ReadFile(fullname);
	Json::Value rootNode = ToNode(jsonText);
	mRootView = Parse(rootNode);
	return mRootView;
}

jView * jUISystem::ParseJsonString(string json)
{
	if (mRootView)
		delete mRootView;

	Json::Value rootNode = ToNode(json);
	mRootView = Parse(rootNode);
	return mRootView;
}

void jUISystem::LoadViews()
{
	if (mRootView)
		mRootView->LoadAll();
}

void jUISystem::MouseEventCall()
{
	if (mRootView == nullptr)
		return;

	jView *retViewDown = CallEventUpDownClick(mCurrentEventInfo);
	jView *retViewHover = CallEventEnterLeaveMove(mCurrentEventInfo);
	mPreviousEventInfo = mCurrentEventInfo;
	mPreviousEventInfo.viewDown = retViewDown;
	mPreviousEventInfo.viewHovered = retViewHover;
}

void jUISystem::Draw()
{
	if(mRootView != nullptr)
		mRootView->DrawAll();
}

void jUISystem::SetMouseEvent(Point2 point, bool down, bool triggered)
{
	mCurrentEventInfo.mouseX = (int)point.x;
	mCurrentEventInfo.mouseY = (int)point.y;
	if (triggered)
		mCurrentEventInfo.mouseDownKeep = down;
}

string jUISystem::ToJsonString()
{
	Json::StreamWriterBuilder builder;
	Json::Value node = Serialize(mRootView);
	return Json::writeString(builder, node);
}
void jUISystem::UpdateView(jView *view, string jsonString)
{
	Json::Value node = ToNode(jsonString);
	view->OnDeserialize(node);
	view->LoadAll();
}
jView *jUISystem::CreateView(int mouseX, int mouseY, int type)
{
	if (mRootView == nullptr)
		return nullptr;

	jView *parent = mRootView->FindTopView(mouseX, mouseY);
	if (parent == nullptr)
		return nullptr;

	jViewType viewType = (jViewType)type;
	jView *newView = CreateView(viewType);
	newView->mParent = parent;
	parent->Childs.push_back(newView);
	Point2 parentAbPos = parent->mRectAbsolute.GetMin();
	newView->LocalX = (int)(mouseX - parentAbPos.x);
	newView->LocalY = (int)(mouseY - parentAbPos.y);
	newView->OnLoad();
	return newView;
}
jView *jUISystem::FindTopView(int mouseX, int mouseY)
{
	if (mRootView != nullptr)
		return mRootView->FindTopView(mouseX, mouseY);

	return nullptr;
}
jView *jUISystem::FindView(int id)
{
	if (mViews.find(id) == mViews.end())
		return nullptr;

	return mViews[id];
}
void jUISystem::DeleteView(int id)
{
	jView* view = mViews[id];
	view->UnLink();
	delete view;
	mViews.erase(id);
}
void jUISystem::ChangeParent(int id, int parentID)
{
	jView* me = mViews[id];
	if (me == mRootView)
		return;

	jView* parent = mViews[parentID];
	me->ChangeParent(parent);
}

void jUISystem::ChangeNeighbor(int id, int neighborID)
{
	jView* me = mViews[id];
	jView* neighbor = mViews[neighborID];
	if (me == mRootView || neighbor == mRootView)
		return;

	me->ChangeNeighbor(neighbor);
}

jView * jUISystem::CallEventUpDownClick(EventParams & info)
{
	jView *returnView = mPreviousEventInfo.viewDown;
	Point2 pt(info.mouseX, info.mouseY);
	if (!mPreviousEventInfo.mouseDownKeep && info.mouseDownKeep) //Mouse Down Triggered
	{
		if (mPreviousEventInfo.viewDown != nullptr)
		{
			mPreviousEventInfo.viewDown->mDowned = false;
			if(mPreviousEventInfo.viewDown->EventMouseUp)
				mPreviousEventInfo.viewDown->EventMouseUp(pt);
		}

		jView *downView = mRootView->FindTopView(info.mouseX, info.mouseY);
		if (downView != nullptr)
		{
			downView->mDowned = true;
			if (downView->EventMouseDown)
				downView->EventMouseDown(pt);
		}

		returnView = downView;
	}
	else if (mPreviousEventInfo.mouseDownKeep && !info.mouseDownKeep) //Mouse Up Triggered
	{
		if (mPreviousEventInfo.viewDown != nullptr)
		{
			mPreviousEventInfo.viewDown->mDowned = false;
			if (mPreviousEventInfo.viewDown->EventMouseUp)
				mPreviousEventInfo.viewDown->EventMouseUp(pt);
		}

		jView *upView = mRootView->FindTopView(info.mouseX, info.mouseY);
		if (upView != nullptr && upView == mPreviousEventInfo.viewDown)
		{
			if(upView->EventMouseClick)
				upView->EventMouseClick(pt); //Mouse Click Invoked in case of same view...
		}

		returnView = nullptr;
	}

	return returnView;
}

jView * jUISystem::CallEventEnterLeaveMove(EventParams & info)
{
	jView *returnView = mPreviousEventInfo.viewHovered;
	if (info.mouseDownKeep) //마우스 클릭상태에서 이동시 Mouse Enter/Leave/Move는 안먹히도록 수정
		return returnView;

	Point2 pt_pre(mPreviousEventInfo.mouseX, mPreviousEventInfo.mouseY);
	Point2 pt(info.mouseX, info.mouseY);
	if ((int)pt.x == (int)pt_pre.x && (int)pt.y == (int)pt_pre.y && mPreviousEventInfo.mouseDownKeep == info.mouseDownKeep)
		return returnView;

	jView *hoverView = mRootView->FindTopView(info.mouseX, info.mouseY);
	if (hoverView == nullptr)
	{
		if (mPreviousEventInfo.viewHovered != nullptr)
		{
			mPreviousEventInfo.viewHovered->mHovered = false;
			if(mPreviousEventInfo.viewHovered->EventMouseLeave)
				mPreviousEventInfo.viewHovered->EventMouseLeave(pt);
		}

		returnView = nullptr;
	}
	else
	{
		if (mPreviousEventInfo.viewHovered == nullptr)
		{
			hoverView->mHovered = true;
			if (hoverView->EventMouseEnter)
				hoverView->EventMouseEnter(pt);
		}
		else if (mPreviousEventInfo.viewHovered == hoverView)
		{
			if(mPreviousEventInfo.viewHovered->EventMouseMove)
				mPreviousEventInfo.viewHovered->EventMouseMove(pt);
		}
		else if (mPreviousEventInfo.viewHovered != nullptr)
		{
			mPreviousEventInfo.viewHovered->mHovered = false;
			if (mPreviousEventInfo.viewHovered->EventMouseLeave)
				mPreviousEventInfo.viewHovered->EventMouseLeave(pt);
			hoverView->mHovered = true;
			if (hoverView->EventMouseEnter)
				hoverView->EventMouseEnter(pt);
		}

		returnView = hoverView;
	}

	return returnView;
}

string jUISystem::ReadFile(string fullename)
{
	ifstream t(fullename);
	string str((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
	return str;
}

Json::Value jUISystem::ToNode(string jsonText)
{
	JSONCPP_STRING err;
	Json::Value node;
	Json::CharReaderBuilder chbuilder;
	const std::unique_ptr<Json::CharReader> reader(chbuilder.newCharReader());
	if (!reader->parse(jsonText.c_str(), jsonText.c_str() + jsonText.length(), &node, &err))
		_errorTrace();

	return node;
}

jView * jUISystem::Parse(Json::Value & jsonNode)
{
	jView *parent = CreateView(jsonNode);
	
	Json::Value& childs = jsonNode["Childs"];
	int count = childs.size();
	for (int i = 0; i < count; ++i)
	{
		jView *child = Parse(childs[i]);
		child->mParent = parent;
		parent->Childs.push_back(child);
	}
	return parent;
}

jView * jUISystem::CreateView(Json::Value & jsonNode)
{
	jViewType type = (jViewType)jsonNode["Type"].asInt();
	jView *view = CreateView(type);
	if (view == nullptr)
		_errorTrace();

	view->OnDeserialize(jsonNode);
	return view;
}
jView *jUISystem::CreateView(jViewType type)
{
	jView *view = nullptr;
	switch (type)
	{
	case View: view = new jView(); break;
	case Button: view = new jViewButton(); break;
	case Font: view = new jViewFont(); break;
	case Image: view = new jViewImage(); break;
	default: break;
	}
	int id = view->GetID();
	mViews[id] = view;
	return view;
}

Json::Value jUISystem::Serialize(jView * view)
{
	Json::Value node;
	view->OnSerialize(node);
	Json::Value childs;
	for (auto iter : view->Childs)
	{
		Json::Value childNode = Serialize(iter);
		childs.append(childNode);
	}
	node["Childs"] = childs;
	return node;
}
