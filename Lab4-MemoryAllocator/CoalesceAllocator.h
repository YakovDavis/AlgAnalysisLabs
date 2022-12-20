#pragma once

#include <windows.h>
#include <new>

#ifdef _DEBUG
#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include <map>
#define assertm(exp, msg) assert(((void)msg, exp))
#define DEBUG_FLAGS_SIZE 8
#define LEFT_DEBUG_FLAG 0xAAAAAAAAAAAAAAAA
#define RIGHT_DEBUG_FLAG 0x5555555555555555
#endif

#ifndef _DEBUG
#define DEBUG_FLAGS_SIZE 0
#endif

class CoalesceAllocator
{
public:
	CoalesceAllocator(size_t pageSize);
	virtual ~CoalesceAllocator();
	virtual void init();
	virtual void destroy();
	virtual void* alloc(size_t size);
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
	size_t numFree_;
	std::map<void*, std::string> occupiedBlocks_;
#endif
	struct FreeBlock // Both occupied and free blocks contain size as a size_t BEFORE pointer, this structure is for free list only 
	{
		FreeBlock* nextBlock_;
		FreeBlock* prevBlock_;
	};
	struct Page
	{
		Page* nextPage_;
		FreeBlock* fh_;
		void* dataStart_;
	};
	void allocPage(Page*& p);
	void freePage(Page*& p);
	Page* firstPage_;
	size_t pageSize_; // Not counting the head
};
