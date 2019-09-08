
class ILedManager
{
public:
	virtual void SetDelta(BrightnessTarget brightnessTarget, int steps) = 0;

	virtual void Tick() = 0;
};

class LedManager: public ILedManager
{
    static const int MaxChannelCount = 16;

    ILedPwm* _pLedPwm;
    int _channelCount;

    LedState _states[MaxChannelCount];
    LedState _deltas[MaxChannelCount];

    public:
        LedManager(ILedPwm* pLedPwm, int channelCount)
        {
            _channelCount = channelCount;

            _pLedPwm = pLedPwm;

            for (int i = 0; i < _channelCount; i++)
            {
                _states[i] = LedState(i, 0);
                _deltas[i] = LedState(i, 0);
            }
        }

		void SetDelta(BrightnessTarget brightnessTarget, int steps)
		{
			for (int item = 0; item < brightnessTarget.GetCount(); item++)
			{
				LedState ledState = brightnessTarget.GetTarget(item);

				// TODO: save target so that we get to the exact endpoint?

				_deltas[ledState.GetChannel()] = LedState(ledState.GetChannel(), ledState.GetBrightness() / steps);
			}
		}

        void Tick()
        {
            for (int i = 0; i < _channelCount; i++)
            {
                _states[i].Update(_deltas[i]);
                _pLedPwm->UpdateLed(_states[i]);
            }
        }
};