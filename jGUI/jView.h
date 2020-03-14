#pragma once
#include <string>
#include <list>
#include <functional>
#include <unordered_map>
using namespace std;

#include "json\json.h"
#include "common.h"
#include "jGenericValue.h"

class jView
{
	friend class jUISystem;
public:
	jView();
	virtual ~jView();

	string Name;
	jViewType Type;
	string LocalFromX;
	string LocalFromY;
	string LocalToX;
	string LocalToY;
	string LocalX;
	string LocalY;
	string Width;
	string Height;
	jColor Color;
	bool Visiable;
	bool Enable;
	list<jView *> Childs;
	unordered_map<string, jGenericValue> UserData;

	function<void(jView*, Point2)> EventMouseDown;
	function<void(jView*, Point2)> EventMouseUp;
	function<void(jView*, Point2)> EventMouseClick;
	function<void(jView*, Point2)> EventMouseEnter;
	function<void(jView*, Point2)> EventMouseMove;
	function<void(jView*, Point2)> EventMouseLeave;

	int GetID() { return mID; }
	void LoadAll();
	void DrawAll();
	jView *FindTopView(int worldX, int worldY);
	void ChangeNeighbor(jView *toNeighbor);
	void AddChild(jView *child);
	void SubChild(jView *child);
	void ClearChilds();
	void Detach();
	jView * FindChild(string name);
	string ToJsonString();
	bool HaveDot(string str);
	jView* GetParent() { return mParent; }
	jRectangle GetRectAbsolute() { return mRectAbsolute; }
	Point2 GetLocalPos() { return mParent==nullptr ? mRectAbsolute.GetMin() : (mRectAbsolute.GetMin() - mParent->GetRectAbsolute().GetMin()); }
	double GetWidth() { return mRectAbsolute.Width(); }
	double GetHeight() { return mRectAbsolute.Height(); }

protected:
	int mID;
	jView * mParent;
	DrawingParams mRenderParam;
	bool mDowned;
	bool mHovered;
	jRectangle mRectAbsolute;

	virtual void OnLoad();
	virtual void OnDraw();
	virtual void OnSerialize(Json::Value & node);
	virtual void OnDeserialize(Json::Value & node);

	virtual Point2 CalcSize();
	virtual Point2 CalcPosition(Point2 alculatedMySize);
	
	double StringToPixel(string val, double ref, double defaultValue = 0);

};

