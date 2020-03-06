
class ILedManager
{
public:
	virtual void SetDelta(CommandResult commandResult) = 0;

	virtual void Tick() = 0;
	virtual void ResetState() = 0;
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

			ResetState();
        }

		void ResetState()
		{
			for (int i = 0; i < _channelCount; i++)
			{
				Variable zero;
				_states[i] = LedState(i, &zero, 0);
				_deltas[i] = LedState(i, &zero, 0);
			}
		}

		void SetDelta(CommandResult commandResult)
		{
			//for (int channel = 0; channel < _channelCount; channel++)
			//{
			//	_deltas[channel] = LedState(channel, 0, 0);
			//}

			for (int item = 0; item < commandResult.GetCount(); item++)
			{
				LedState ledState = commandResult.GetTarget(item);
				Variable* pTargetBrightness = ledState.GetBrightness();

				// TODO: save target so that we get to the exact endpoint?
				Variable delta;
				for (int i = 0; i < pTargetBrightness->GetValueCount(); i++)
				{
					float itemDelta = (ledState.GetBrightness()->GetValueFloat(i) - _states[ledState.GetChannel()].GetBrightness()->GetValueFloat(i)) / ledState.GetCycleCount();

					delta.SetValue(i, itemDelta);
				}

				_deltas[ledState.GetChannel()] = LedState(ledState.GetChannel(), &delta, ledState.GetCycleCount());
			}
		}

        void Tick()
        {
            for (int i = 0; i < _channelCount; i++)
            {
				if (_deltas[i].GetCycleCount() > 0)
				{
					_states[i].Update(_deltas[i]);
					_deltas[i].DecrementCycleCount();
				}
                _pLedPwm->UpdateLed(_states[i]);
                //_states[i].GetBrightness();
                //Serial.print(_states[i].GetBrightness());
                //Serial.print(" ");
            }
            //Serial.println();
        }
};