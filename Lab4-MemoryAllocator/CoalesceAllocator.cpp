#include "CoalesceAllocator.h"

CoalesceAllocator::CoalesceAllocator(size_t pageSize)
{
	pageSize_ = pageSize;
	firstPage_ = nullptr;

#ifdef _DEBUG
	isInitialized_ = false;
	isDestroyed_ = false;
	numOccupied_ = 0;
	numFree_ = 0;
#endif
}

CoalesceAllocator::~CoalesceAllocator()
{
#ifdef _DEBUG
	assertm(isDestroyed_, "CoalesceAllocator: not destroyed before calling destructor");
#endif
}

void CoalesceAllocator::init()
{
#ifdef _DEBUG
	isInitialized_ = true;
	isDestroyed_ = false;
#endif
	allocPage(firstPage_);
}

void CoalesceAllocator::destroy()
{
#ifdef _DEBUG
	assertm(isInitialized_, "CoalesceAllocator: not initialized before calling destroy()");
	assertm(numOccupied_ == 0, "CoalesceAllocator: trying to destroy allocator when some blocks are not free");
#endif
	freePage(firstPage_);
	firstPage_ = nullptr;
#ifdef _DEBUG
	isDestroyed_ = true;
#endif
}

void* CoalesceAllocator::alloc(size_t size)
{
#ifdef _DEBUG
	assertm(isInitialized_, "CoalesceAllocator: not initialized before calling alloc()");
	numOccupied_ += 1;
	numFree_ -= 1;
	int pageNumber = 0;
#endif
	Page* p = firstPage_;
	while (p->fh_ == nullptr) // Find or create page with some free space
	{
		if (p->nextPage_ == nullptr)
		{
			Page* tmpPage = p;
			allocPage(p);
			tmpPage->nextPage_ = p;
		}
		else
			p = p->nextPage_;
#ifdef _DEBUG
		pageNumber += 1;
#endif
	}
	while (true)
	{
		FreeBlock* tmp = p->fh_;
		while (tmp != nullptr)
		{
			if (*((size_t*)tmp - 1) >= size + sizeof(size_t)) // Checks if block size is enough
			{
				if (*((size_t*)tmp - 1) > size + (sizeof(size_t)) * 2 + 2 * DEBUG_FLAGS_SIZE + 512) // Checks if block size is enough to split it into two (using 512 as the smallest size)
				{
					FreeBlock* addedBlock = new((FreeBlock*)((char*)tmp + size + 2 * DEBUG_FLAGS_SIZE + sizeof(size_t))) FreeBlock;
					addedBlock->nextBlock_ = tmp->nextBlock_;
					addedBlock->prevBlock_ = tmp->prevBlock_;
					if (addedBlock->prevBlock_ != nullptr)
						addedBlock->prevBlock_->nextBlock_ = addedBlock;
					else
						p->fh_ = addedBlock;
					if (addedBlock->nextBlock_ != nullptr)
						addedBlock->nextBlock_->prevBlock_ = addedBlock;
					*((size_t*)addedBlock - 1) = *((size_t*)tmp - 1) - (size + 2 * DEBUG_FLAGS_SIZE + sizeof(size_t));
#ifdef _DEBUG
					*((size_t*)((char*)addedBlock - sizeof(size_t) - DEBUG_FLAGS_SIZE)) = LEFT_DEBUG_FLAG;
					*((size_t*)((char*)addedBlock - sizeof(size_t) - 2 * DEBUG_FLAGS_SIZE)) = RIGHT_DEBUG_FLAG;
					*((size_t*)tmp - 1) = size;
					numFree_ += 1;
					std::ostringstream oss;
					oss << "Page: " << pageNumber << ", address: " << (void*)tmp << ", size: " << *((size_t*)tmp - 1);
					occupiedBlocks_.insert({ (void*)tmp,  oss.str()});
#endif
					return (void*)tmp;
				}
				else
				{
					if (tmp->nextBlock_ != nullptr)
						tmp->nextBlock_->prevBlock_ = tmp->prevBlock_;
					if (tmp->prevBlock_ != nullptr)
						tmp->prevBlock_->nextBlock_ = tmp->nextBlock_;
					else
						p->fh_ = tmp->nextBlock_;
#ifdef _DEBUG
					std::ostringstream oss;
					oss << "Page: " << pageNumber << ", address: " << (void*)tmp << ", size: " << *((size_t*)tmp - 1);
					occupiedBlocks_.insert({ (void*)tmp,  oss.str() });
#endif
					return (void*)tmp;
				}
			}
			tmp = tmp->nextBlock_;
		}
		if (p->nextPage_ == nullptr)
		{
			Page* tmpPage = p;
			allocPage(p);
			tmpPage->nextPage_ = p;
		}
		else
			p = p->nextPage_;
	}
}

bool CoalesceAllocator::free(void* p)
{
#ifdef _DEBUG
	assertm(isInitialized_, "CoalesceAllocator: not initialized before calling free()");
	numOccupied_ -= 1;
	numFree_ += 1;
	occupiedBlocks_.erase(p);
#endif
	Page* page = firstPage_;
	while (page != nullptr)
	{
		if ((char*)(page->dataStart_) <= (char*)p && (char*)p <= (char*)(page->dataStart_) + pageSize_) // Compares data ranges with the pointer to find the correct page
		{
#ifdef _DEBUG
			assertm(*((size_t*)((char*)p - sizeof(size_t) - DEBUG_FLAGS_SIZE)) == LEFT_DEBUG_FLAG, "CoalesceAllocator: memory corruption detected");
			assertm(*((size_t*)((char*)p + *((size_t*)p - 1))) == RIGHT_DEBUG_FLAG, "CoalesceAllocator: memory corruption detected");
#endif
			size_t size = *((size_t*)p - 1);
			new(p) FreeBlock; // Creates the supporting structure in case no free left neighbor exists
			((FreeBlock*)p)->nextBlock_ = nullptr;
			((FreeBlock*)p)->prevBlock_ = nullptr;
			FreeBlock* tmp = page->fh_;
			while (tmp != nullptr)
			{
				if ((void*)((char*)tmp + *((size_t*)tmp - 1) + sizeof(size_t) + 2 * DEBUG_FLAGS_SIZE) == p) // Tries to find free left neighbor of freed block
				{
					*((size_t*)tmp - 1) += size + sizeof(size_t) + 2 * DEBUG_FLAGS_SIZE; // If found, changes the neighbor's size to include the freed block + its size property and two safety flags
					p = (void*)tmp; // Then changes the pointer to point to the new bigger block
					size = *((size_t*)p - 1);
#ifdef _DEBUG
					numFree_ -= 1;
#endif
				}
				if ((void*)((char*)tmp - sizeof(size_t) - 2 * DEBUG_FLAGS_SIZE - size) == p) // Tries to find free right neighbor of freed block
				{
					*((size_t*)p - 1) += sizeof(size_t) + 2 * DEBUG_FLAGS_SIZE + *((size_t*)tmp - 1); // If found, changes the freed block size to include the neighbor block + its size property and two safety flags
					((FreeBlock*)p)->prevBlock_ = ((FreeBlock*)tmp)->prevBlock_; // Copies the neighbors previous block to freed block
					if (((FreeBlock*)p)->nextBlock_ == nullptr && ((FreeBlock*)tmp)->nextBlock_ != (FreeBlock*)p)
						((FreeBlock*)p)->nextBlock_ = ((FreeBlock*)tmp)->nextBlock_;
					size = *((size_t*)p - 1);
#ifdef _DEBUG
					numFree_ -= 1;
#endif
				}
				tmp = tmp->nextBlock_;
			}
			if (((FreeBlock*)p)->prevBlock_ == nullptr) // If after coalesce no previous block assigned, update next block pointer to fh, put the new block in fh
			{
				if (page->fh_ != ((FreeBlock*)p))
					((FreeBlock*)p)->nextBlock_ = page->fh_;
				page->fh_ = (FreeBlock*)p;
			}
			return true;
		}
		page = page->nextPage_;
	}
#ifdef _DEBUG
	numOccupied_ += 1; 
#endif
	return false;
}

void CoalesceAllocator::allocPage(Page*& p)
{
#ifdef _DEBUG
	assertm(isInitialized_, "CoalesceAllocator: not initialized before calling allocPage()");
	numFree_ += 1;
#endif
	p = (Page*)(VirtualAlloc(NULL, sizeof(Page) + pageSize_, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
	new(p) Page;
	p->nextPage_ = nullptr;
#ifdef _DEBUG
	*((size_t*)((char*)p + sizeof(Page))) = LEFT_DEBUG_FLAG;
	*((size_t*)((char*)p + sizeof(Page) + pageSize_ - DEBUG_FLAGS_SIZE)) = RIGHT_DEBUG_FLAG;
#endif
	p->fh_ = (FreeBlock*)((char*)p + sizeof(Page) + sizeof(size_t) + DEBUG_FLAGS_SIZE);
	new(p->fh_) FreeBlock;
	p->fh_->prevBlock_ = nullptr;
	p->fh_->nextBlock_ = nullptr;
	*((size_t*)(p->fh_) - 1) = pageSize_ - sizeof(size_t) - 2 * DEBUG_FLAGS_SIZE;
	p->dataStart_ = (char*)p + sizeof(Page);
}

void CoalesceAllocator::freePage(Page*& p)
{
#ifdef _DEBUG
	assertm(isInitialized_, "CoalesceAllocator: not initialized before calling freePage()");
	numFree_ -= 1;
#endif
	if (p == nullptr)
		return;
	freePage(p->nextPage_);
	VirtualFree((void*)p, 0, MEM_RELEASE);
}

#ifdef _DEBUG
void CoalesceAllocator::dumpStat() const
{
	assertm(isInitialized_, "CoalesceAllocator: not initialized before calling dumpStat()");
	Page* p = firstPage_;
	size_t numPages = 0;
	while (p != nullptr)
	{
		numPages += 1;
		p = p->nextPage_;
	}
	std::cout << numPages << " pages allocated with a total of "
		<< numFree_ + numOccupied_ << " blocks (" << numPages * pageSize_ << " bytes)\n"
		<< numOccupied_ << " blocks occupied, " << numFree_ << " blocks free, " << "\n";

}

void CoalesceAllocator::dumpBlocks() const
{
	assertm(isInitialized_, "CoalesceAllocator: not initialized before calling dumpBlocks()");
	for (auto it = occupiedBlocks_.begin(); it != occupiedBlocks_.end(); ++it)
	{
		std::cout << it->second << '\n';
	}
}
#endif
