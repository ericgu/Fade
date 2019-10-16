#pragma once

#include "Stack.h"

class StackTest
{
	static void TestFrame()
	{
		Stack stack;

		stack.CreateFrame();
		Assert::AreEqual(1, stack.GetFrameCount());

		stack.GetTopFrame().SerialNumberStart = 14;
		stack.GetTopFrame().SerialNumberEnd = 15;

		stack.CreateFrame();
		Assert::AreEqual(2, stack.GetFrameCount());
		Assert::AreEqual(0, stack.GetTopFrame().SerialNumberStart);
		Assert::AreEqual(0, stack.GetTopFrame().SerialNumberEnd);

		stack.DestroyFrame();
		Assert::AreEqual(14, stack.GetTopFrame().SerialNumberStart);
		Assert::AreEqual(15, stack.GetTopFrame().SerialNumberEnd);

		stack.DestroyFrame();
		Assert::AreEqual(0, stack.GetFrameCount());
	}

	static void TestClear()
	{
		Stack stack;

		stack.CreateFrame();
		Assert::AreEqual(1, stack.GetFrameCount());

		stack.Clear();

		Assert::AreEqual(0, stack.GetFrameCount());
	}

public:
	static void Run()
	{
		TestFrame();
		TestClear();
	}
};
