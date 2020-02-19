#pragma once
#include <string>
#include <functional>
#include <time.h>
#include <unordered_map>
using namespace std;

#include "common.h"

struct jCacheItem {
	unsigned int key;
	void* ptr;
	clock_t hitTime;
};

class jCacheMgr
{
public:
	static jCacheMgr & GetInst() { return *mInst; }
	void * Cache(unsigned int key, function<void*(void)> loader);

private:
	static jCacheMgr * mInst;

	jCacheMgr();
	~jCacheMgr();

	unordered_map<unsigned int, jCacheItem> mCachedItems;

	void ClearItems(int count);
};

