#pragma once

class TickSource
{
	unsigned long _ticks;

public:
	unsigned long GetTicks()
	{
		return _ticks;
	}

	void SetTicks(unsigned long ticks)
	{
		_ticks = ticks;
	}

};
