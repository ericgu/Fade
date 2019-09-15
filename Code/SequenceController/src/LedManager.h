
class ILedManager
{
public:
	virtual void SetDelta(CommandResult commandResult) = 0;

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

		void SetDelta(CommandResult commandResult)
		{
			for (int channel = 0; channel < _channelCount; channel++)
			{
				_deltas[channel] = LedState(channel, 0);
			}

			for (int item = 0; item < commandResult.GetCount(); item++)
			{
				LedState ledState = commandResult.GetTarget(item);

				// TODO: save target so that we get to the exact endpoint?

				float delta = ledState.GetBrightness() - _states[ledState.GetChannel()].GetBrightness();

				_deltas[ledState.GetChannel()] = LedState(ledState.GetChannel(), delta / commandResult.GetCycleCount());
			}
		}

        void Tick()
        {
            for (int i = 0; i < _channelCount; i++)
            {
                _states[i].Update(_deltas[i]);
                _pLedPwm->UpdateLed(_states[i]);
                //_states[i].GetBrightness();
                //Serial.print(_states[i].GetBrightness());
                //Serial.print(" ");
            }
            //Serial.println();
        }
};