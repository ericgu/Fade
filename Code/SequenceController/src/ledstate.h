class LedState
{
    int _channel;
    float _brightness;

    public:
        LedState()
        {
            _channel = -1;
            _brightness = -1;
        }

        LedState(int channel, float brightness)
        {
            _channel = channel;
            _brightness = brightness;
        }

        int GetChannel()
        {
            return _channel;
        }

        float GetBrightness()
        {
            return _brightness;
        }

        void Update(LedState deltaState)
        {
            if (deltaState._channel == _channel)
            {   
                _brightness += deltaState._brightness;
            }
        }
};