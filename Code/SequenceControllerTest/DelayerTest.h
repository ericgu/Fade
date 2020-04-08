#pragma once

#include "Delayer.h"

class DelayerTest
{
	static void TestSimpleCase()
	{
		Delayer delayer;

		delayer.Snapshot(0, 100);

		Assert::AreEqual(false, delayer.CheckIfDone(0));

		Assert::AreEqual(false, delayer.CheckIfDone(99));

		Assert::AreEqual(true, delayer.CheckIfDone(100));
	}

	// test case where ticks < maxlong but ticks + wait > maxlong
	static void TestOverflowCase()
	{
		Delayer delayer;

		delayer.Snapshot(-50, 100);

		Assert::AreEqual(false, delayer.CheckIfDone(-50));

		Assert::AreEqual(false, delayer.CheckIfDone(49));

		Assert::AreEqual(true, delayer.CheckIfDone(50));
	}

public:

	static void Run()
	{
		TestSimpleCase();
		TestOverflowCase();
	}
};