#include <algorithm>
#include "jCacheMgr.h"


#define CacheMaxCount 1000

jCacheMgr * jCacheMgr::mInst = new jCacheMgr();

jCacheMgr::jCacheMgr()
{
}
jCacheMgr::~jCacheMgr()
{
	ClearItems(mCachedItems.size());
}
void jCacheMgr::ClearItems(int count)
{
	vector<jCacheItem> items;
	for (auto item : mCachedItems)
		items.push_back(item.second);

	sort(items.begin(), items.end(), [](jCacheItem& left, jCacheItem& right) {
		return left.hitTime < right.hitTime;
	});

	int minCount = ((int)items.size() >= count) ? count : items.size();
	for (int i = 0; i < minCount; ++i)
	{
		delete items[i].ptr;
		mCachedItems.erase(items[i].key);
	}
}
void * jCacheMgr::Cache(unsigned int key, function<void*(void)> loader)
{
	if (mCachedItems.find(key) == mCachedItems.end())
	{
		if (mCachedItems.size() >= CacheMaxCount)
			ClearItems(CacheMaxCount / 2);

		void *ptr = loader();
		if (ptr == nullptr)
			return nullptr;

		jCacheItem info;
		info.key = key;
		info.ptr = ptr;
		info.hitTime = clock();
		mCachedItems[key] = info;
	}

	mCachedItems[key].hitTime = clock();
	return mCachedItems[key].ptr;
}
