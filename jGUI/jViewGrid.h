#pragma once
#include "jView.h"
#include <unordered_map>

class jViewGrid :
	public jView
{
public:
	jViewGrid();
	virtual ~jViewGrid();

	function<void(int)> EventGridClick;
	function<void(int)> EventGridHover;

	PropImage ImageCell;
	int ColumnCount;
	int RowCount;
	
	virtual void AddChild(jView *child);
	virtual void SubChild(jView *child);
	void MoveTo(int idx, jView *movingChild);
	jView* GetChild(int idx);
	jView* GetChild(int idxX, int idxY);
	jView* GetChild(Point2 pt);
	void ClearChild();

protected:
	unordered_map<int, jView*> mGridChilds;
	jUIBitmap *mTexture;
	int mStepWidth;
	int mStepHeight;

	virtual void OnLoad();
	virtual void OnDraw();
	virtual void OnSerialize(Json::Value & node);
	virtual void OnDeserialize(Json::Value & node);

	int FindViewOnGrid(jView* view);

};

