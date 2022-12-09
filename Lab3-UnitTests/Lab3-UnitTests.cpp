#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab3-QuickSort/Lab3-QuickSort.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Lab3UnitTests
{
	TEST_CLASS(Lab3UnitTests)
	{
	public:

		TEST_METHOD(ChoosePivotTest)
		{
			int sample[5] = { 1, 2, 3, 4, 5 };
			int a1[5] = {1, 2, 3, 4, 5};
			int p1 = choosepivot(a1, a1 + 2, a1 + 4, [](int a, int b) { return a < b; });
			Assert::AreEqual(p1, 3);
			for (int i = 0; i < 5; i++)
				Assert::AreEqual(a1[i], sample[i]);
			int a2[5] = { 3, 2, 1, 4, 5 };
			int p2 = choosepivot(a2, a2 + 2, a2 + 4, [](int a, int b) { return a < b; });
			Assert::AreEqual(p2, 3);
			for (int i = 0; i < 5; i++)
				Assert::AreEqual(a2[i], sample[i]);
			int a3[5] = { 1, 2, 5, 4, 3 };
			int p3 = choosepivot(a3, a3 + 2, a3 + 4, [](int a, int b) { return a < b; });
			Assert::AreEqual(p3, 3);
			for (int i = 0; i < 5; i++)
				Assert::AreEqual(a3[i], sample[i]);
		}

		TEST_METHOD(QuickSort10ReverseIntsTest)
		{
			int a[10];
			for (int i = 0; i < 10; i++)
				a[i] = 10 - i;
			quicksort(a, a + 9, [](int a, int b) { return a < b; });
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(a[i], i + 1);
		}

		TEST_METHOD(InsertSort10ReverseIntsTest)
		{
			int a[10];
			for (int i = 0; i < 10; i++)
				a[i] = 10 - i;
			insertionsort(a, a + 9, [](int a, int b) { return a < b; });
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(a[i], i + 1);
		}

		TEST_METHOD(CustomSort10ReverseIntsTest)
		{
			int a[10];
			for (int i = 0; i < 10; i++)
				a[i] = 10 - i;
			sort(a, a + 9, [](int a, int b) { return a < b; });
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(a[i], i + 1);
		}

		TEST_METHOD(QuickSortBackwards10IntsTest)
		{
			int a[10];
			for (int i = 0; i < 10; i++)
				a[i] = i + 1;
			quicksort(a, a + 9, [](int a, int b) { return a > b; });
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(a[i], 10 - i);
		}

		TEST_METHOD(InsertSortBackwards10IntsTest)
		{
			int a[10];
			for (int i = 0; i < 10; i++)
				a[i] = i + 1;
			insertionsort(a, a + 9, [](int a, int b) { return a > b; });
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(a[i], 10 - i);
		}

		TEST_METHOD(CustomSortBackwards10IntsTest)
		{
			int a[10];
			for (int i = 0; i < 10; i++)
				a[i] = i + 1;
			sort(a, a + 9, [](int a, int b) { return a > b; });
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(a[i], 10 - i);
		}

		TEST_METHOD(QuickSort1IntTest)
		{
			int a[1] = {1};
			quicksort(a, a, [](int a, int b) { return a < b; });
			Assert::AreEqual(a[0], 1);
		}

		TEST_METHOD(InsertSort1IntTest)
		{
			int a[1] = { 1 };
			insertionsort(a, a, [](int a, int b) { return a < b; });
			Assert::AreEqual(a[0], 1);
		}

		TEST_METHOD(CustomSort1IntTest)
		{
			int a[1] = { 1 };
			sort(a, a, [](int a, int b) { return a < b; });
			Assert::AreEqual(a[0], 1);
		}

		TEST_METHOD(QuickSort5StringsTest)
		{
			std::string sample[5] = {"1", "11", "111", "1111","11111"};
			std::string a[5] = { "11111", "1111", "111", "11", "1" };
			quicksort(a, a + 4, [](std::string a, std::string b) { return a.length() < b.length(); });
			for (int i = 0; i < 5; i++)
				Assert::AreEqual(a[i], sample[i]);
		}

		TEST_METHOD(InsertSort5StringsTest)
		{
			std::string sample[5] = { "1", "11", "111", "1111","11111" };
			std::string a[5] = { "11111", "1111", "111", "11", "1" };
			insertionsort(a, a + 4, [](std::string a, std::string b) { return a.length() < b.length(); });
			for (int i = 0; i < 5; i++)
				Assert::AreEqual(a[i], sample[i]);
		}

		TEST_METHOD(CustomSort5StringsTest)
		{
			std::string sample[5] = { "1", "11", "111", "1111","11111" };
			std::string a[5] = { "11111", "1111", "111", "11", "1" };
			sort(a, a + 4, [](std::string a, std::string b) { return a.length() < b.length(); });
			for (int i = 0; i < 5; i++)
				Assert::AreEqual(a[i], sample[i]);
		}

		TEST_METHOD(QuickSort10SameIntsTest)
		{
			int a[10];
			for (int i = 0; i < 10; i++) {
				a[i] = 1;
			}
			quicksort(a, a + 9, [](int a, int b) { return a < b; });
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(a[i], 1);
		}

		TEST_METHOD(InsertSort10SameIntsTest)
		{
			int a[10];
			for (int i = 0; i < 10; i++) {
				a[i] = 1;
			}
			insertionsort(a, a + 9, [](int a, int b) { return a < b; });
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(a[i], 1);
		}

		TEST_METHOD(CustomSort10SameIntsTest)
		{
			int a[10];
			for (int i = 0; i < 10; i++) {
				a[i] = 1;
			}
			sort(a, a + 9, [](int a, int b) { return a < b; });
			for (int i = 0; i < 10; i++)
				Assert::AreEqual(a[i], 1);
		}

		TEST_METHOD(QuickSort5IntsWithRepeatsTest)
		{
			int sample[5] = { 2, 3, 3, 5, 5 };
			int a[5] = {5, 3, 3, 2, 5};
			quicksort(a, a + 4, [](int a, int b) { return a < b; });
			for (int i = 0; i < 5; i++)
				Assert::AreEqual(a[i], sample[i]);
		}

		TEST_METHOD(InsertSort5IntsWithRepeatsTest)
		{
			int sample[5] = { 2, 3, 3, 5, 5 };
			int a[5] = { 5, 3, 3, 2, 5 };
			insertionsort(a, a + 4, [](int a, int b) { return a < b; });
			for (int i = 0; i < 5; i++)
				Assert::AreEqual(a[i], sample[i]);
		}

		TEST_METHOD(CustomSort5IntsWithRepeatsTest)
		{
			int sample[5] = { 2, 3, 3, 5, 5 };
			int a[5] = { 5, 3, 3, 2, 5 };
			sort(a, a + 4, [](int a, int b) { return a < b; });
			for (int i = 0; i < 5; i++)
				Assert::AreEqual(a[i], sample[i]);
		}

		TEST_METHOD(QuickSort100RandomIntsTest)
		{
			std::srand(std::time(nullptr));
			int a[100];
			int b[100];
			for (int i = 0; i < 100; i++)
			{
				a[i] = std::rand();
				b[i] = a[i];
			}
			quicksort(a, a + 99, [](int a, int b) { return a < b; });
			std::qsort(b, 100, sizeof(int), [](const void* a, const void* b) { return (*(int*)a - *(int*)b); });
			for (int i = 0; i < 100; i++)
				Assert::AreEqual(a[i], b[i]);
		}

		TEST_METHOD(InsertSort100RandomIntsTest)
		{
			std::srand(std::time(nullptr));
			int a[100];
			int b[100];
			for (int i = 0; i < 100; i++)
			{
				a[i] = std::rand();
				b[i] = a[i];
			}
			insertionsort(a, a + 99, [](int a, int b) { return a < b; });
			std::qsort(b, 100, sizeof(int), [](const void* a, const void* b) { return (*(int*)a - *(int*)b); });
			for (int i = 0; i < 100; i++)
				Assert::AreEqual(a[i], b[i]);
		}

		TEST_METHOD(CustomSort100RandomIntsTest)
		{
			std::srand(std::time(nullptr));
			int a[100];
			int b[100];
			for (int i = 0; i < 100; i++)
			{
				a[i] = std::rand();
				b[i] = a[i];
			}
			sort(a, a + 99, [](int a, int b) { return a < b; });
			std::qsort(b, 100, sizeof(int), [](const void* a, const void* b) { return (*(int*)a - *(int*)b); });
			for (int i = 0; i < 100; i++)
				Assert::AreEqual(a[i], b[i]);
		}
	};
}
