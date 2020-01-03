#pragma once

#include "Delayer.h"

class DelayerTest
{
	static void TestSimpleCase()
	{
		TickSource tickSource;
		Delayer delayer(&tickSource);

		tickSource.SetTicks(0);
		delayer.Snapshot(100);

		Assert::AreEqual(false, delayer.CheckIfDone());

		tickSource.SetTicks(99);
		Assert::AreEqual(false, delayer.CheckIfDone());

		tickSource.SetTicks(100);
		Assert::AreEqual(true, delayer.CheckIfDone());
	}

	// test case where ticks < maxlong but ticks + wait > maxlong
	static void TestOverflowCase()
	{
		TickSource tickSource;
		Delayer delayer(&tickSource);

		tickSource.SetTicks(-50);
		delayer.Snapshot(100);

		Assert::AreEqual(false, delayer.CheckIfDone());

		tickSource.SetTicks(49);
		Assert::AreEqual(false, delayer.CheckIfDone());

		tickSource.SetTicks(50);
		Assert::AreEqual(true, delayer.CheckIfDone());
	}

public:

	static void Run()
	{
		TestSimpleCase();
		TestOverflowCase();
	}
};