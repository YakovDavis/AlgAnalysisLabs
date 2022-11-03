#include "pch.h"
#include "CppUnitTest.h"
#include "../Lab2-DynamicArray/Array.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Lab2UnitTests
{
	TEST_CLASS(Lab2UnitTests)
	{
	public:
		
		TEST_METHOD(StrReadWriteDefaultCapacityInBounds)
		{
			Array<std::string> arr;
			arr.insert("hello");
			arr.insert("world");
			Assert::AreEqual(arr[0], std::string("hello"));
			Assert::AreEqual(arr[1], std::string("world"));
		}

		TEST_METHOD(StrReadWriteDefaultCapacityOutBounds)
		{
			Array<std::string> arr;
			for (int i = 0; i < 30; i++)
				arr.insert("hello");
			arr.insert("world");
			Assert::AreEqual(arr[30], std::string("world"));
		}

		TEST_METHOD(StrInsert)
		{
			Array<std::string> arr;
			for (int i = 0; i < 30; i++)
				arr.insert("good");
			arr.insert("night");
			arr.insert("city");
			arr.insert(30, "morning");
			Assert::AreEqual(arr[29], std::string("good"));
			Assert::AreEqual(arr[30], std::string("morning"));
			Assert::AreEqual(arr[31], std::string("night"));
			Assert::AreEqual(arr[32], std::string("city"));
		}

		TEST_METHOD(StrRemove)
		{
			Array<std::string> arr;
			for (int i = 0; i < 30; i++)
				arr.insert("good");
			arr.insert("morning");
			arr.insert("morning");
			arr.insert("night");
			arr.insert("city");
			arr.remove(30);
			Assert::AreEqual(arr[29], std::string("good"));
			Assert::AreEqual(arr[30], std::string("morning"));
			Assert::AreEqual(arr[31], std::string("night"));
			Assert::AreEqual(arr[32], std::string("city"));
		}

		TEST_METHOD(StrInsertEnd)
		{
			Array<std::string> arr;
			for (int i = 0; i < 30; i++)
				arr.insert("good");
			arr.insert(30, "morning");
			Assert::AreEqual(arr[29], std::string("good"));
			Assert::AreEqual(arr[30], std::string("morning"));
		}
	};
}
