#include "jView.h"
#include "jViewButton.h"
#include "jUISystem.h"

jView::jView()
{
	Name = "Name";
	Type = jViewType::View;
	LocalFromX = "0";
	LocalFromY = "0";
	LocalToX = "0";
	LocalToY = "0";
	LocalX = "0";
	LocalY = "0";
	Width = "80";
	Height = "30";
	Color = jColor(240, 240, 240, 255);
	Visiable = true;
	Enable = true;

	mRectAbsolute = jRectangle(0, 0, 80, 30);

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
void jView::ChangeNeighbor(jView * toNeighbor)
{
	if (toNeighbor->mParent != mParent)
		return;

	list<jView *>::iterator iterMe = find(Childs.begin(), Childs.end(), this);
	list<jView *>::iterator iterTo = find(Childs.begin(), Childs.end(), toNeighbor);
	Childs.erase(iterMe);
	Childs.insert(iterTo, this);
}
void jView::AddChild(jView *child)
{
	child->mParent = this;
	Childs.push_back(child);
}
void jView::SubChild(jView *child)
{
	auto iter = std::find(Childs.begin(), Childs.end(), child);
	if (iter == Childs.end())
		return;

	child->mParent = nullptr;
	Childs.erase(iter);
}
void jView::ClearChilds()
{
	for (auto iter : Childs)
		delete iter;
	Childs.clear();
}
void jView::Detach()
{
	if(mParent != nullptr)
		mParent->SubChild(this);
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

	Point2 abSize = CalcSize();
	Point2 abPos = CalcPosition(abSize);
	mRectAbsolute.SetPosSize(abPos, abSize);

	mRenderParam.rect = mRectAbsolute;
	mRenderParam.color = Color;
}

void jView::OnDraw()
{
	jUISystem::GetInst()->EventDrawFill(mRenderParam);
}

void jView::OnSerialize(Json::Value & node)
{
	node["#ID"] = mID;
	node["Name"] = Name;
	node["Type"] = Type;
	node["#Type"] = "Enum&View,Button,Font,Image,Grid";
	node["LocalFromX"] = LocalFromX;
	node["LocalFromY"] = LocalFromY;
	node["LocalToX"] = LocalToX;
	node["LocalToY"] = LocalToY;
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
	LocalFromX = node["LocalFromX"].asString();
	LocalFromY = node["LocalFromY"].asString();
	LocalToX = node["LocalToX"].asString();
	LocalToY = node["LocalToY"].asString();
	LocalX = node["LocalX"].asString();
	LocalY = node["LocalY"].asString();
	Width = node["Width"].asString();
	Height = node["Height"].asString();
	Color.r = node["Color"][0].asUInt();
	Color.g = node["Color"][1].asUInt();
	Color.b = node["Color"][2].asUInt();
	Color.a = node["Color"][3].asUInt();
	Visiable = node["Visiable"].asBool();
	Enable = node["Enable"].asBool();
}

Point2 jView::CalcSize()
{
	if (mParent == nullptr)
		return Point2(atoi(Width.c_str()), atoi(Height.c_str()));

	Point2 size;
	if (Width.length() == 0 && Height.length() == 0)
	{
		size.x = mRectAbsolute.Width();
		size.y = mRectAbsolute.Height();
	}
	else if (Height.length() == 0)
	{
		size.x = StringToPixel(Width, mParent->GetRectAbsolute().Width());
		size.y = size.x * (mRectAbsolute.Height() / mRectAbsolute.Width());
	}
	else if (Width.length() == 0)
	{
		size.y = StringToPixel(Height, mParent->GetRectAbsolute().Height());
		size.x = size.y * (mRectAbsolute.Width() / mRectAbsolute.Height());
	}
	else
	{
		size.x = StringToPixel(Width, mParent->GetRectAbsolute().Width());
		size.y = StringToPixel(Height, mParent->GetRectAbsolute().Height());
	}
	return size;
}

Point2 jView::CalcPosition(Point2 alculatedMySize)
{
	if (mParent == nullptr)
		return Point2(atoi(LocalX.c_str()), atoi(LocalY.c_str()));

	Point2 pPos = mParent->GetRectAbsolute().GetMin();
	Point2 pSize = mParent->GetRectAbsolute().GetSize();
	Point2 from;
	from.x = StringToPixel(LocalFromX, pSize.x);
	from.y = StringToPixel(LocalFromY, pSize.y);
	Point2 to;
	to.x = StringToPixel(LocalToX, alculatedMySize.x);
	to.y = StringToPixel(LocalToY, alculatedMySize.y);
	Point2 local;
	local.x = StringToPixel(LocalX, pSize.x);
	local.y = StringToPixel(LocalY, pSize.y);
	Point2 pos;
	pos = pPos + from + local - to;
	return pos;
}

bool jView::HaveDot(string str)
{
	for (char ch : str)
		if (ch == '.')
			return true;
	return false;
}

double jView::StringToPixel(string val, double ref, double defaultValue)
{
	if (val.length() == 0)
		return defaultValue;
	
	if (HaveDot(val)) //rate mode
	{
		double rate = atof(val.c_str());
		return ref * rate;
	}
	return atoi(val.c_str());
}

