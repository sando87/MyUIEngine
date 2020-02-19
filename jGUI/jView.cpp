#include "jView.h"
#include "jViewButton.h"
#include "jUISystem.h"

jView::jView()
{
	Name = "Name";
	Type = jViewType::View;
	LocalX = 0;
	LocalY = 0;
	Width = 80;
	Height = 30;
	Color = jColor(240, 240, 240, 255);
	Visiable = true;
	Enable = true;

	mParent = nullptr;
	mDowned = false;
	mHovered = false;
}


jView::~jView()
{
	for (auto iter : Childs)
		delete iter;
}

void jView::LoadAll()
{
	OnLoad();
	for (auto iter : Childs)
		iter->LoadAll();
}

void jView::DrawAll()
{
	if (!Visiable)
		return;

	OnDraw();
	for (auto iter : Childs)
		iter->DrawAll();
}

jView * jView::FindTopView(int worldX, int worldY)
{
	if (!Enable || !mRectAbsolute.Contains(Point2(worldX, worldY)))
		return nullptr;

	jView *findView = this;
	for (auto iter = Childs.rbegin(); iter != Childs.rend(); ++iter)
	{
		jView *child = (*iter)->FindTopView(worldX, worldY);
		if (child != nullptr)
			return child;
	}
	return findView;
}

void jView::ChangeParent(jView * newParent)
{
	if (mParent == newParent)
		return;

	for (auto iter = mParent->Childs.begin(); iter != mParent->Childs.end(); ++iter)
	{
		if (*iter == this)
		{
			mParent->Childs.erase(iter);
			break;
		}
	}
	newParent->Childs.push_back(this);
	mParent = newParent;
	LocalX = (int)(mRectAbsolute.Left() - newParent->mRectAbsolute.Left());
	LocalY = (int)(mRectAbsolute.Top() - newParent->mRectAbsolute.Top());
	LoadAll();
}

void jView::AddChild(jView * child)
{
	child->mParent = this;
	Childs.push_back(child);
}

jView * jView::FindChild(string name)
{
	for (jView *child : Childs)
		if (child->Name == name)
			return child;

	return nullptr;
}
string jView::ToJsonString()
{
	Json::Value node;
	OnSerialize(node);
	Json::StreamWriterBuilder builder;
	return Json::writeString(builder, node);
}

void jView::OnLoad()
{
	//여기 오기전에 JsonNode와 Parent는 세팅이 되어 있어야 함
	mDowned = false;
	mHovered = false;

	mRectRelative.SetPosSize(Point2(LocalX, LocalY), Point2(Width, Height));
	Point2 parentAbPt = mParent == nullptr ? Point2() : mParent->mRectAbsolute.GetPos();
	mRectAbsolute.SetPosSize(parentAbPt + Point2(LocalX, LocalY), Point2(Width, Height));

	mRenderParam.rect = mRectAbsolute;
	mRenderParam.color = Color;
}

void jView::OnDraw()
{
	jUISystem::GetInst()->EventDrawFill(mRenderParam);
}

void jView::OnSerialize(Json::Value & node)
{
	node["Name"] = Name;
	node["Type"] = Type;
	node["#Type"] = "Enum&View,Button,Font,Image";
	node["LocalX"] = LocalX;
	node["LocalY"] = LocalY;
	node["Width"] = Width;
	node["Height"] = Height;
	Json::Value color;
	color.append(Color.r); 
	color.append(Color.g);
	color.append(Color.b);
	color.append(Color.a);
	node["Color"] = color;
	node["#Color"] = "Color";
	node["Visiable"] = Visiable;
	node["Enable"] = Enable;
}

void jView::OnDeserialize(Json::Value & node)
{
	Name = node["Name"].asString();
	Type = (jViewType)node["Type"].asInt();
	LocalX = node["LocalX"].asInt();
	LocalY = node["LocalY"].asInt();
	Width = node["Width"].asInt();
	Height = node["Height"].asInt();
	Color.r = node["Color"][0].asUInt();
	Color.g = node["Color"][1].asUInt();
	Color.b = node["Color"][2].asUInt();
	Color.a = node["Color"][3].asUInt();
	Visiable = node["Visiable"].asBool();
	Enable = node["Enable"].asBool();
}


