class LedState
{
    int _channel;
    float _brightness;
	int _cycleCount;

    public:
        LedState()
        {
            _channel = -1;
            _brightness = -1;
			_cycleCount = -1;
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

        float GetBrightness()
        {
            return _brightness;
        }

		int GetCycleCount()
		{
			return _cycleCount;
		}

        void Update(LedState deltaState)
        {
            if (deltaState._channel == _channel)
            {   
                _brightness += deltaState._brightness;
            }
        }

		void DecrementCycleCount()
		{
			_cycleCount--;
		}
};