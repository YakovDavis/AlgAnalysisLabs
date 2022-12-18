// Lab4-MemoryAllocator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "MemoryAllocator.h"

int main()
{
    MemoryAllocator allocator;
    allocator.init();
    int* pi = (int*)allocator.alloc(sizeof(int));
    double* pd = (double*)allocator.alloc(sizeof(double));
    int* pa = (int*)allocator.alloc(10 * sizeof(int));
    allocator.dumpStat();
    allocator.dumpBlocks();
    allocator.free(pa);
    allocator.free(pd);
    allocator.free(pi);
    allocator.dumpStat();
    allocator.dumpBlocks();
    allocator.destroy();
}
