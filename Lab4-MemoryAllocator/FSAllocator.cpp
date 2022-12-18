#include "FSAllocator.h"

FSAllocator::FSAllocator(size_t blockSize, size_t pageBlockAmount)
{
	blockSize_ = blockSize;
	pageBlockAmount_ = pageBlockAmount;
	firstPage_ = nullptr;

#ifdef _DEBUG
	isInitialized_ = false;
	isDestroyed_ = false;
	numOccupied_ = 0;
#endif
}

FSAllocator::~FSAllocator()
{
#ifdef _DEBUG
	assertm(isDestroyed_, "FSAllocator: not destroyed before calling destructor");
#endif
}

void FSAllocator::init()
{
#ifdef _DEBUG
	isInitialized_ = true;
	isDestroyed_ = false;
#endif
	allocPage(firstPage_);
}

void FSAllocator::destroy()
{
#ifdef _DEBUG
	assertm(isInitialized_, "FSAllocator: not initialized before calling destroy()");
	assertm(numOccupied_ == 0, "FSAllocator: trying to destroy allocator when some blocks are not free");
#endif
	freePage(firstPage_);
	firstPage_ = nullptr;
#ifdef _DEBUG
	isDestroyed_ = true;
#endif
}

void* FSAllocator::alloc()
{
#ifdef _DEBUG
	assertm(isInitialized_, "FSAllocator: not initialized before calling alloc()");
	numOccupied_ += 1;
#endif
	Page* p = firstPage_;
	while ((int)p->fh_ < 0) // Find or create page with some free space
	{
		if (p->nextPage_ == nullptr)
		{
			Page* tmpPage = p;
			allocPage(p);
			tmpPage->nextPage_ = p;
		}
		else
			p = p->nextPage_;
	}
	if (p->numInitialized_ < pageBlockAmount_) // If possible, initialize a new block in the page
	{
		size_t* tmp = (size_t*)((char*)p->dataStart_ + blockSize_ * p->numInitialized_);
		new(tmp) size_t;
		p->numInitialized_ += 1;
		if (p->numInitialized_ < pageBlockAmount_) // If there are more blocks to be initialized, write the next index, otherwise -1
			*tmp = p->numInitialized_;
		else
			*tmp = -1;
	}
	void* pRes = (void*)((char*)(p->dataStart_) + p->fh_ * blockSize_); // Get the block by free-list head
	p->fh_ = *((size_t*)pRes); // Record the new index from the block to free-list head
	return pRes;
}

bool FSAllocator::free(void* p)
{
#ifdef _DEBUG
	assertm(isInitialized_, "FSAllocator: not initialized before calling free()");
	numOccupied_ -= 1;
#endif
	Page* page = firstPage_;
	while (page != nullptr)
	{
		if ((char*)(page->dataStart_) <= (char*)p && (char*)p <= (char*)(page->dataStart_) + blockSize_ * pageBlockAmount_) // Compare data ranges with the pointer to find the correct page
		{
			*((size_t*)p) = page->fh_; // Record current free-list head into the freed block
			page->fh_ = ((char*)p - (char*)(page->dataStart_)) / blockSize_; // Record freed block's index into free list head
			return true;
		}
		page = page->nextPage_;
	}
#ifdef _DEBUG
	numOccupied_ += 1;
#endif
	return false;
}

void FSAllocator::allocPage(Page*& p)
{
#ifdef _DEBUG
	assertm(isInitialized_, "FSAllocator: not initialized before calling allocPage()");
#endif
	p = (Page*)(VirtualAlloc(NULL, sizeof(Page) + pageBlockAmount_ * blockSize_, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE));
	new(p) Page;
	p->nextPage_ = nullptr;
	p->fh_ = 0;
	p->dataStart_ = (char*)p + sizeof(Page);
	p->numInitialized_ = 0;
}

void FSAllocator::freePage(Page*& p)
{
#ifdef _DEBUG
	assertm(isInitialized_, "FSAllocator: not initialized before calling freePage()");
#endif
	if (p == nullptr)
		return;
	freePage(p->nextPage_);
	VirtualFree((void*)p, 0, MEM_RELEASE);
}

#ifdef _DEBUG
void FSAllocator::dumpStat() const
{
	assertm(isInitialized_, "FSAllocator: not initialized before calling dumpStat()");
	Page* p = firstPage_;
	size_t numInitialized = 0;
	size_t numPages = 0;
	while (p != nullptr)
	{
		numInitialized += p->numInitialized_;
		numPages += 1;
		p = p->nextPage_;
	}
	std::cout << numPages << " pages allocated with " << pageBlockAmount_
		<< " blocks in each (total " << numPages * pageBlockAmount_ << " blocks, "
		<< numPages * pageBlockAmount_ * blockSize_ << " bytes)\n"
		<< numOccupied_ << " blocks occupied, " << numPages * pageBlockAmount_ - numOccupied_ << " blocks free, " << numInitialized << " blocks initialized.\n";

}

void FSAllocator::dumpBlocks() const
{
	assertm(isInitialized_, "FSAllocator: not initialized before calling dumpBlocks()");
	Page* p = firstPage_;
	size_t pageNumber = 0;
	while (p != nullptr)
	{
		for (size_t i = 0; i < p->numInitialized_; i++)
		{
			bool flag = true;
			size_t tmp = p->fh_;
			while (tmp >= 0 && tmp < p->numInitialized_)
			{
				if (i == tmp)
				{
					flag = false;
					break;
				}
				tmp = *((size_t*)((char*)(p->dataStart_) + tmp * blockSize_));
			}
			if (flag)
				std::cout << "Page: " << pageNumber << ", address: " << (void*)((char*)(p->dataStart_) + i * blockSize_) << ", size: " << blockSize_ << "\n";
		}
		p = p->nextPage_;
		pageNumber += 1;
	}
}
#endif
