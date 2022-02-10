#pragma once

#include "Delayer.h"

class DelayerTest
{
	static void TestSimpleCase()
	{
        TimeServices timeServices;
        timeServices.SetTicks(0, 1000);
		Delayer delayer(&timeServices);

        delayer.DelayFromLastDelay(5000);

        Assert::AreEqual(6, timeServices.GetGetTicksCount());
	}

	// test case where ticks < maxlong but ticks + wait > maxlong
	static void TestOverflowCase()
	{
        TimeServices timeServices;
        timeServices.SetTicks(-2000, 1000);
        Delayer delayer(&timeServices);

        delayer.DelayFromLastDelay(5000);

        Assert::AreEqual(6, timeServices.GetGetTicksCount());
	}

    static void TestDoubleCase()
    {
        TimeServices timeServices;
        timeServices.SetTicks(0, 1000);
        Delayer delayer(&timeServices);

        delayer.DelayFromLastDelay(5000);

        Assert::AreEqual(6, timeServices.GetGetTicksCount());

        delayer.DelayFromLastDelay(3000);

        Assert::AreEqual(9, timeServices.GetGetTicksCount());
    }

public:

	static void Run()
	{
		TestSimpleCase();
        TestDoubleCase();
		TestOverflowCase();
	}
};