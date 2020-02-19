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
	static jUISystem *GetInst() { static jUISystem inst; return &inst; }

	function<void(DrawingParams)> EventDrawFill;
	function<void(DrawingParams)> EventDrawOutline;
	function<void(DrawingParams)> EventDrawTexture;

	function<void*(jUIBitmap *)>	OpLoadTexture;
	function<void(void *)>			OpReleaseTexture;

	void SetResourcePath(string pathname) { mResourcePath = pathname; }
	string GetResourcePath() { return mResourcePath; }
	jView * ParseJson(string filename);
	void LoadViews();
	void MouseEventCall();
	void Draw();
	void SetMouseEvent(Point2 point, bool down, bool triggered);
	string ToJsonString();
	void UpdateView(jView *view, string jsonString);
	jView *CreateView(int mouseX, int mouseY, int type);
	jView *FindTopView(int mouseX, int mouseY);

private:
	EventParams mCurrentEventInfo;
	EventParams mPreviousEventInfo;
	jView *mRootView;
	string mResourcePath;

	jView *CallEventUpDownClick(EventParams & info);
	jView *CallEventEnterLeaveMove(EventParams & info);
	string ReadFile(string fullename);
	Json::Value ToNode(string jsonText);
	jView *Parse(Json::Value & jsonNode);
	jView *CreateView(Json::Value & jsonNode);
	jView *CreateView(jViewType type);
	Json::Value Serialize(jView *view);

	jUISystem();
	~jUISystem();
};
