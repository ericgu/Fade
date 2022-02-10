class Delayer
{
	unsigned long _startTicks;
	//unsigned long _interval;
	//unsigned long _offset;
	//unsigned long _target;
	TimeServices *_pTimeServices;

public:
	Delayer(TimeServices *pTimeServices)
	{
		_pTimeServices = pTimeServices;
		_startTicks = _pTimeServices->GetTicks();
	}

	void Snapshot2(unsigned long currentTicks, unsigned long interval)
	{
		return;
#if fred
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
#endif
		//_startTicks = currentTicks;
		//_interval = interval;
	}

	void DelayFromLastDelay(unsigned long interval)
	{
		while (1)
		{
			unsigned long ticks = _pTimeServices->GetTicks();
			unsigned long value = ticks - _startTicks;
			if (value >= interval)
			{
				_startTicks = ticks;
				return;
			}
		}
	}
};