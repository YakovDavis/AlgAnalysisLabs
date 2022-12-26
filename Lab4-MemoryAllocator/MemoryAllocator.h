#pragma once

#include "FSAllocator.h"
#include "CoalesceAllocator.h"
#include <vector>

#ifdef _DEBUG
#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include <map>
#define assertm(exp, msg) assert(((void)msg, exp))
#endif

constexpr size_t kPageSize = 0xA00000;

class MemoryAllocator
{
public:
	MemoryAllocator();
	virtual ~MemoryAllocator();
	virtual void init();
	virtual void destroy();
	virtual void* alloc(size_t size);
	virtual void free(void* p);
#ifdef _DEBUG
	virtual void dumpStat() const;
	virtual void dumpBlocks() const;
#endif
private:
#ifdef _DEBUG
	bool isInitialized_;
	bool isDestroyed_;
	size_t numOccupied_;
	size_t osBlocksSizeTotal_;
	std::map<void*, std::string> occupiedBlocks_;
#endif
	FSAllocator fsa16;
	FSAllocator fsa32;
	FSAllocator fsa64;
	FSAllocator fsa128;
	FSAllocator fsa256;
	FSAllocator fsa512;
	CoalesceAllocator ca;
	std::vector<void*> osBlocks;
};