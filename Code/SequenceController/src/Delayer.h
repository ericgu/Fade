class Delayer
{
	unsigned long _ticks;
	unsigned long _interval;
	unsigned long _offset;

public:
	Delayer()
	{
	}

	void Snapshot(unsigned long currentTicks, unsigned long interval) 
	{
		_ticks = currentTicks;
		_interval = interval;

		// if _ticks + interval is going to overflow, we will fail. In that case, we figure out how far away we are from rolling over and add that value to both our snapshot and the value we are checking...

		if (_ticks + _interval < _ticks)
		{
			_offset = 0 - _ticks; 
		}
		else
		{
			_offset = 0;
		}
	}

	bool CheckIfDone(unsigned long currentTicks)
	{
		unsigned long currentTicksWithOffset = currentTicks + _offset;

		unsigned long target = _ticks + _interval + _offset;
		if (currentTicksWithOffset >= target)
		{
			return true;
		}
		
		return false;
	}

};