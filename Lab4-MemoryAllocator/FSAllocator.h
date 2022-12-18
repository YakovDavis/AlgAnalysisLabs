#pragma once

#include <windows.h>

#ifdef _DEBUG
#include <iostream>
#include <cassert>
#define assertm(exp, msg) assert(((void)msg, exp))
#endif

class FSAllocator
{
public:
	FSAllocator(size_t blockSize, size_t pageBlockAmount);
	virtual ~FSAllocator();
	virtual void init();
	virtual void destroy();
	virtual void* alloc();
	virtual bool free(void* p);
#ifdef _DEBUG
	virtual void dumpStat() const;
	virtual void dumpBlocks() const;
#endif
private:
#ifdef _DEBUG
	bool isInitialized_;
	bool isDestroyed_;
	size_t numOccupied_;
#endif
	struct Page
	{
		Page* nextPage_;
		size_t fh_;
		size_t numInitialized_;
		void* dataStart_;
	};
	void allocPage(Page*& p);
	void freePage(Page*& p);
	Page* firstPage_;
	size_t blockSize_;
	size_t pageBlockAmount_;
};
