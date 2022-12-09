#include <iostream>
#include <windows.h>
#include "profileapi.h"
#include <fstream>

constexpr int kSwitchToInsertion = 16;

template<typename T, typename Compare>
void insertionsort(T* first, T* last, Compare comp)
{
    if (first == last)
        return;
    for (T* i = first + 1; i <= last; ++i)
    {
        T val = std::move(*i);
        T* j = i - 1;
        while (j >= first && comp(val, *j))
        {
            *(j + 1) = std::move(*j);
            j -= 1;
        }
        *(j + 1) = std::move(val);
    }
}

template<typename T, typename Compare>
T choosepivot(T* l, T* m, T* r, Compare comp)
{
    if (comp(*r, *l))
        std::swap(*l, *r);
    if (comp(*m, *l))
        std::swap(*l, *m);
    if (comp(*r, *m))
        std::swap(*r, *m);
    return *m;
}

template<typename T, typename Compare>
T* partition(T* first, T* last, Compare comp)
{
    T p = choosepivot(first, (first + (last - first) / 2), last, comp);
    T* l = first;
    T* r = last;
    while (true)
    {
        while (comp(*l, p))
            l += 1;
        while (comp(p, *r))
            r -= 1;
        if (l >= r)
            break;
        std::swap(*l, *r);
        l += 1;
        r -= 1;
    }
    return r;
}

template<typename T, typename Compare>
void quicksort(T* first, T* last, Compare comp)
{
    T* p;
    while (last - first > 0)
    {
        p = partition(first, last, comp);

        if (p - first < last - p - 1)
        {
            quicksort(first, p, comp);
            first = p + 1;
        }
        else
        {
            quicksort(p + 1, last, comp);
            last = p;
        }
    }
}

template<typename T, typename Compare>
void sort(T* first, T* last, Compare comp)
{
    T* p;
    while (last - first > 0)
    {
        if (last - first <= kSwitchToInsertion)
        {
            insertionsort(first, last, comp);
            return;
        }

        p = partition(first, last, comp);

        if (p - first < last - p)
        {
            sort(first, p, comp);
            first = p + 1;
        }
        else
        {
            sort(p + 1, last, comp);
            last = p;
        }
    }
}

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        std::cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart) / 1000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart - CounterStart) / PCFreq;
}

constexpr int kMaxArraySize = 50;
constexpr int kTestRepeats = 1e5;

int main()
{
    int arr[kMaxArraySize];
    double insertTime = 0.0;
    double quickTime = 0.0;
    double sortTime = 0.0;
    for (int i = 1; i < kMaxArraySize; ++i)
    {
        insertTime = 0.0;
        quickTime = 0.0;
        sortTime = 0.0;
        for (int k = 0; k < kTestRepeats; ++k)
        {
            for (int j = 0; j < i; ++j)
                arr[j] = i - j;
            StartCounter();
            insertionsort(arr, arr + i - 1, [](int a, int b) { return a < b; });
            insertTime += GetCounter();
        }
        for (int k = 0; k < kTestRepeats; ++k)
        {
            for (int j = 0; j < i; ++j)
                arr[j] = i - j;
            StartCounter();
            quicksort(arr, arr + i - 1, [](int a, int b) { return a < b; });
            quickTime += GetCounter();
        }
        for (int k = 0; k < kTestRepeats; ++k)
        {
            for (int j = 0; j < i; ++j)
                arr[j] = i - j;
            StartCounter();
            sort(arr, arr + i - 1, [](int a, int b) { return a < b; });
            sortTime += GetCounter();
        }
        std::cout << i << "," << insertTime << "," << quickTime << "," << sortTime << std::endl;
    }
}
