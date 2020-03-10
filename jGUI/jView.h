#pragma once
#include <string>
#include <list>
#include <functional>
using namespace std;

#include "json\json.h"
#include "common.h"

class jView
{
	friend class jUISystem;
public:
	jView();
	virtual ~jView();

	string Name;
	jViewType Type;
	int LocalX;
	int LocalY;
	int Width;
	int Height;
	jColor Color;
	bool Visiable;
	bool Enable;
	list<jView *> Childs;

	function<void(Point2)> EventMouseDown;
	function<void(Point2)> EventMouseUp;
	function<void(Point2)> EventMouseClick;
	function<void(Point2)> EventMouseEnter;
	function<void(Point2)> EventMouseMove;
	function<void(Point2)> EventMouseLeave;

	int GetID() { return mID; }
	void LoadAll();
	void DrawAll();
	jView *FindTopView(int worldX, int worldY);
	void ChangeNeighbor(jView *toNeighbor);
	virtual void AddChild(jView *child);
	virtual void SubChild(jView *child);
	jView * FindChild(string name);
	string ToJsonString();

	jRectangle GetRectAbsolute() { return mRectAbsolute; }

protected:
	int mID;
	jView * mParent;
	DrawingParams mRenderParam;
	bool mDowned;
	bool mHovered;
	jRectangle mRectRelative;
	jRectangle mRectAbsolute;

	virtual void OnLoad();
	virtual void OnDraw();
	virtual void OnSerialize(Json::Value & node);
	virtual void OnDeserialize(Json::Value & node);

};

