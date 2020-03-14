#pragma once
#include "jView.h"
#include <unordered_map>

class jViewGrid :
	public jView
{
public:
	jViewGrid();
	virtual ~jViewGrid();

	function<void(jView*, Point2)> EventGridClick;
	function<void(jView*, Point2)> EventGridEnter;
	function<void(jView*, Point2)> EventGridLeave;

	PropImage ImageCell;
	int ColumnCount;
	int RowCount;
	
	jView* FindEmptyChild();
	jView* GetChild(unsigned int idx);
	bool IsEmpty(unsigned int idx);
	int ToIndex(Point2 local);

protected:
	vector<jView*> mGridChilds;
	double mStepWidth;
	double mStepHeight;
	int mPreIdx;

	virtual void OnLoad();
	virtual void OnDraw();
	virtual void OnSerialize(Json::Value & node);
	virtual void OnDeserialize(Json::Value & node);

	
};

