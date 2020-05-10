#pragma once

class TimeServices
{
	static unsigned long _ticks;

public:
	static unsigned long GetTicks()
	{
		return _ticks;
	}

	static void SetTicks(unsigned long ticks)
	{
		_ticks = ticks;
	}

	static void DelayMicroseconds(int delay)
	{
		_ticks += delay;
	}

	static void TaskDelay(int delay)
	{
	}
};

unsigned long TimeServices::_ticks;
