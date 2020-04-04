class Delayer
{
	TickSource* _pTickSource;
	unsigned long _ticks;
	unsigned long _interval;
	unsigned long _offset;

public:
	Delayer(TickSource* pTickSource)
	{
		_pTickSource = pTickSource;
	}

	void Snapshot(unsigned long interval) 
	{
		_ticks = _pTickSource->GetTicks();
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

	bool CheckIfDone()
	{
		unsigned long currentTicks = _pTickSource->GetTicks() + _offset;

		unsigned long target = _ticks + _interval + _offset;
		if (currentTicks >= target)
		{
			return true;
		}
		
		return false;
	}

	void Wait()
	{
		while (!CheckIfDone())
		{
			delayMicroseconds(50);
		}
	}
};