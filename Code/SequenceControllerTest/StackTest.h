#pragma once

#include "Stack.h"

class StackTest
{
	static void TestStackFrame()
	{
		StackFrame frame;

		Assert::AreEqual(0, frame.GetInstructionPointer());
		frame.IncrementInstructionPointer("test");
		Assert::AreEqual(1, frame.GetInstructionPointer());

		frame.SetInstructionPointer(12, "test");
		Assert::AreEqual(12, frame.GetInstructionPointer());
	}

	static void TestFrame()
	{
		Stack stack;

		stack.CreateFrame();
		Assert::AreEqual(1, stack.GetFrameCount());

		stack.GetTopFrame()->LineNumberStart = 14;

		stack.CreateFrame();
		Assert::AreEqual(2, stack.GetFrameCount());
		Assert::AreEqual(0, stack.GetTopFrame()->LineNumberStart);

		stack.DestroyFrame();
		Assert::AreEqual(14, stack.GetTopFrame()->LineNumberStart);

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

	static void TestGetCallingFrame()
	{
		Stack stack;

		stack.CreateFrame();
		stack.GetTopFrame()->SetInstructionPointer(15, "test");
		stack.CreateFrame();
		Assert::AreEqual(2, stack.GetFrameCount());

		StackFrame* pCallingFrame = stack.GetCallingFrame();
		
		Assert::AreEqual(15, pCallingFrame->GetInstructionPointer());
	}

	static void TestTooMany()
	{
		Stack stack;
	
		for (int i = 0; i < 10; i++)
		{
			stack.CreateFrame();
		}
		Serial.SetOutput(0);
		stack.CreateFrame();
		Serial.SetOutput(1);

		Assert::AreEqual("Stack Frame count exceeded", Serial.GetLastString());

		Assert::AreEqual(10, stack.GetFrameCount());
	}

	static void TestTooManyDelete()
	{
		Stack stack;

		Serial.SetOutput(0);
		stack.DestroyFrame();
		Serial.SetOutput(1);

		Assert::AreEqual("Can't delete stack frame", Serial.GetLastString());

		Assert::AreEqual(0, stack.GetFrameCount());
	}

	static void TestRecreate()
	{
		Stack stack;

		stack.CreateFrame();
		StackFrame* pStackFrame = stack.GetTopFrame();
		pStackFrame->SetInstructionPointer(15, "test");

		stack.Clear();

		stack.CreateFrame();
		pStackFrame = stack.GetTopFrame();

		Assert::AreEqual(0, pStackFrame->GetInstructionPointer());
	}

public:
	static void Run()
	{
		TestFrame();
		TestClear();
		TestGetCallingFrame();
		TestTooMany();
		TestTooManyDelete();
		TestRecreate();

		TestStackFrame();
	}
};
