#pragma once

#include <windows.h>
#include <new>

#ifdef _DEBUG
#include <iostream>
#include <cassert>
#define assertm(exp, msg) assert(((void)msg, exp))
#define DEBUG_FLAGS_SIZE 8 // The neighbor blocks are used with their whole size, this size is just for the control data
#define LEFT_DEBUG_FLAG 0xAAAAAAAAAAAAAAAA
#define RIGHT_DEBUG_FLAG 0x5555555555555555
#define BLOCK_AMOUNT_MULTIPLIER 3
#endif

#ifndef _DEBUG
#define DEBUG_FLAGS_SIZE 0
#define BLOCK_AMOUNT_MULTIPLIER 1
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
