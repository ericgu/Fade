#pragma once

class TimeServices
{
	static unsigned long _ticks;

public:

	static unsigned long GetTicks()
	{
    LARGE_INTEGER frequency;
	  LARGE_INTEGER startingTime;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&startingTime);

    int countPerMicrosecond = (int) (frequency.QuadPart / 1000000);

    LARGE_INTEGER bigTicks;
    bigTicks.QuadPart = startingTime.QuadPart / countPerMicrosecond;
    unsigned long ticks = (unsigned long) (startingTime.QuadPart / countPerMicrosecond);

	  return bigTicks.LowPart;
	}

	static void DelayMicroseconds(int delay)
	{
    unsigned long current = GetTicks();
    unsigned long target = current + delay;

        // TODO: Sometimes target is bigger than the value GetTicks() will every return -
        // something more than FF000000 (value I saw was FF42bA5A) - because of the resolution of the counter.
        // that can cause a hang here.
        // Hack fix is to add a counter here after too long...

    int loopCount = 0;
    while (GetTicks() < target)
    {
        loopCount++;

    }
    int k = loopCount;
	}

	static void TaskDelay(int delay)
	{
	}
};

unsigned long TimeServices::_ticks;
