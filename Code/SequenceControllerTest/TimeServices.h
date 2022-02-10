#pragma once

class TimeServices
{
	unsigned long _ticks;
    unsigned long _increment;
    unsigned long _getTicksCount;

public:
    TimeServices()
    {
        _ticks = 0;
        _increment = 10000;
    }

	unsigned long GetTicks()
	{
        unsigned long value = _ticks;

        _ticks += _increment;
        _getTicksCount++;

		return value;
	}

	void SetTicks(unsigned long ticks, unsigned long increment)
	{
		_ticks = ticks;
        _increment = increment;
        _getTicksCount = 0;
	}

    int GetGetTicksCount()
	{
        return _getTicksCount;
	}

	void DelayMicroseconds(int delay)
	{
		_ticks += delay;
	}

	void TaskDelay(int delay)
	{
	}
};

