class LedState
{
	int _channel;
	Variable _brightness;
	//float _brightness;
	int _cycleCount;

public:
	LedState()
	{
		_channel = -1;
		_brightness = -1;
		_cycleCount = -1;
	}

	LedState(int channel, Variable *pBrightness, int cycleCount)
	{
		_channel = channel;
		_brightness = *pBrightness;
		_cycleCount = cycleCount;
	}

	LedState(int channel, float brightness, int cycleCount)
	{
		_channel = channel;
		_brightness = brightness;
		_cycleCount = cycleCount;
	}

	int GetChannel()
	{
		return _channel;
	}

	void SetChannel(int channel)
	{
		_channel = channel;
	}

	Variable *GetBrightness()
	{
		return &_brightness;
	}

    void SetBrightness(Variable brightness)
	{
        _brightness = brightness;
	}

	int GetCycleCount()
	{
		return _cycleCount;
	}

	void Update(LedState deltaState)
	{
		if (deltaState._channel == _channel)
		{
			Variable *pDeltaBrightness = deltaState.GetBrightness();
			for (int i = 0; i < deltaState.GetBrightness()->GetValueCount(); i++)
			{
				_brightness.SetValue(i, _brightness.GetValueFloat(i) + pDeltaBrightness->GetValueFloat(i));
			}
		}
	}

	void DecrementCycleCount()
	{
		_cycleCount--;
	}
};