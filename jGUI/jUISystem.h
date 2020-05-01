#pragma once
#include <functional>
#include <unordered_map>
using namespace std;

#include "common.h"
#include "json\json.h"

class jView;


struct EventParams
{
	int mouseX;
	int mouseY;
	bool mouseDownKeep;
	char key;
	bool keyDown;
	jView *viewDown;
	jView *viewHovered;
	EventParams() { memset(this, 0x00, sizeof(EventParams)); }
};

class jUISystem
{
public:
	jUISystem();
	~jUISystem();

	function<void(DrawingParams)> EventDrawFill;
	function<void(DrawingParams)> EventDrawOutline;
	function<void(DrawingParams)> EventDrawTexture;

	function<void*(jUIBitmap *)>	OpLoadTexture;
	function<void(void *)>			OpReleaseTexture;

	void SetResourcePath(string pathname) { mResourcePath = pathname; }
	string GetResourcePath() { return mResourcePath; }
	jView * GetRootView() { return mRootView; }
	jView * ParseJson(string filename);
	jView * ParseJsonString(string json);
	void LoadViews();
	void MouseEventCall();
	void Draw();
	void SetMouseEvent(Point2 point, bool down, bool triggered);
	string ToJsonString();
	void UpdateView(jView *view, string jsonString);
	jView *CreateView(jViewType type);
	jView *FindTopView(int mouseX, int mouseY);
	jView *FindView(int id);
	jView *FindView(string name);
	void DeleteView(int id);
	void ChangeParent(int id, int parentID);
	void ChangeNeighbor(int id, int neighborID);

private:
	unsigned int mNextViewID;
	EventParams mCurrentEventInfo;
	EventParams mPreviousEventInfo;
	jView *mRootView;
	string mResourcePath;
	unordered_map<int, jView*> mViews;

	jView *CallEventUpDownClick(EventParams & info);
	jView *CallEventEnterLeaveMove(EventParams & info);
	string ReadFile(string fullename);
	Json::Value ToNode(string jsonText);
	jView *Parse(Json::Value & jsonNode);
	Json::Value Serialize(jView *view);

};

