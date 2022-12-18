#include "MemoryAllocator.h"

MemoryAllocator::MemoryAllocator() : fsa16 (16, kPageSize / 16),
									 fsa32(32, kPageSize / 32),
									 fsa64(64, kPageSize / 64),
									 fsa128(128, kPageSize / 128),
									 fsa256(256, kPageSize / 256),
									 fsa512(512, kPageSize / 512),
									 ca(kPageSize)
{
#ifdef _DEBUG
	isInitialized_ = false;
	isDestroyed_ = false;
	numOccupied_ = 0;
	osBlocksSizeTotal_ = 0;
#endif
}

MemoryAllocator::~MemoryAllocator()
{
#ifdef _DEBUG
	assertm(isDestroyed_, "MemoryAllocator: not destroyed before calling destructor");
#endif
}

void MemoryAllocator::init()
{
#ifdef _DEBUG
	isInitialized_ = true;
	isDestroyed_ = false;
#endif
	fsa16.init();
	fsa32.init();
	fsa64.init();
	fsa128.init();
	fsa256.init();
	fsa512.init();
	ca.init();
}

void MemoryAllocator::destroy()
{
#ifdef _DEBUG
	assertm(isInitialized_, "MemoryAllocator: not initialized before calling destroy()");
	assertm(numOccupied_ == 0, "MemoryAllocator: trying to destroy allocator when some blocks are not free");
	isDestroyed_ = true;
#endif
	fsa16.destroy();
	fsa32.destroy();
	fsa64.destroy();
	fsa128.destroy();
	fsa256.destroy();
	fsa512.destroy();
	ca.destroy();
}

void* MemoryAllocator::alloc(size_t size)
{
#ifdef _DEBUG
	assertm(isInitialized_, "MemoryAllocator: not initialized before calling alloc()");
#endif

	if (size <= 16)
		return fsa16.alloc();

	if (size <= 32)
		return fsa32.alloc();

	if (size <= 64)
		return fsa64.alloc();

	if (size <= 128)
		return fsa128.alloc();

	if (size <= 256)
		return fsa256.alloc();

	if (size <= 512)
		return fsa512.alloc();

	if (size < kPageSize)
		return ca.alloc(size);

	void* tmp = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	osBlocks.push_back(tmp);
#ifdef _DEBUG
	osBlocksSizeTotal_ += size;
	numOccupied_ += 1;
	std::ostringstream oss;
	oss << "Page: N/A, " << ", address: " << tmp << ", size: " << size;
	occupiedBlocks_.insert({ tmp,  oss.str() });
#endif
	return tmp;
}

void MemoryAllocator::free(void* p)
{
#ifdef _DEBUG
	assertm(isInitialized_, "MemoryAllocator: not initialized before calling free()");
#endif

	if (fsa16.free(p))
		return;

	if (fsa32.free(p))
		return;

	if (fsa64.free(p))
		return;

	if (fsa128.free(p))
		return;

	if (fsa256.free(p))
		return;

	if (fsa512.free(p))
		return;

	if (ca.free(p))
		return;

#ifdef _DEBUG
	occupiedBlocks_.erase(p);
#endif
	if (VirtualFree(p, 0, MEM_RELEASE))
	{
#ifdef _DEBUG
		numOccupied_ -= 1;
#endif
		return;
	}
#ifdef _DEBUG
	assertm(true, "MemoryAllocator: Invalid pointer.");
#endif
}

#ifdef _DEBUG
void MemoryAllocator::dumpStat() const
{
	assertm(isInitialized_, "MemoryAllocator: not initialized before calling dumpStat()");

	std::cout << "FSAllocator (16 bytes) stats:\n";
	fsa16.dumpStat();
	std::cout << "FSAllocator (32 bytes) stats:\n";
	fsa32.dumpStat();
	std::cout << "FSAllocator (64 bytes) stats:\n";
	fsa64.dumpStat();
	std::cout << "FSAllocator (128 bytes) stats:\n";
	fsa128.dumpStat();
	std::cout << "FSAllocator (256 bytes) stats:\n";
	fsa256.dumpStat();
	std::cout << "FSAllocator (512 bytes) stats:\n";
	fsa512.dumpStat();
	std::cout << "CoalesceAllocator stats:\n";
	ca.dumpStat();

	std::cout << "MemoryAllocator (OS Allocated) stats:\n";
	std::cout << numOccupied_ << " blocks allocated and occupied at " << osBlocksSizeTotal_ << " bytes total\n";
}

void MemoryAllocator::dumpBlocks() const
{
	assertm(isInitialized_, "MemoryAllocator: not initialized before calling dumpBlocks()");

	std::cout << "FSAllocator (16 bytes) occupied blocks:\n";
	fsa16.dumpBlocks();
	std::cout << "FSAllocator (32 bytes) occupied blocks:\n";
	fsa32.dumpBlocks();
	std::cout << "FSAllocator (64 bytes) occupied blocks:\n";
	fsa64.dumpBlocks();
	std::cout << "FSAllocator (128 bytes) occupied blocks:\n";
	fsa128.dumpBlocks();
	std::cout << "FSAllocator (256 bytes) occupied blocks:\n";
	fsa256.dumpBlocks();
	std::cout << "FSAllocator (512 bytes) occupied blocks:\n";
	fsa512.dumpBlocks();
	std::cout << "CoalesceAllocator occupied blocks:\n";
	ca.dumpBlocks();

	std::cout << "MemoryAllocator (OS Allocated) occupied blocks:\n";
	for (auto it = occupiedBlocks_.begin(); it != occupiedBlocks_.end(); ++it)
	{
		std::cout << it->second << '\n';
	}
}
#endif
