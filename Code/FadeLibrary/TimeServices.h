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

    while (GetTicks() < target)
    {
      
    }
	}

	static void TaskDelay(int delay)
	{
	}
};

unsigned long TimeServices::_ticks;
