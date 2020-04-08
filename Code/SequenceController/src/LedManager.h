
class ILedManager
{
public:
	virtual void SetDelta(CommandResult commandResult) = 0;

	virtual void Tick() = 0;
	virtual void ResetState() = 0;
};

class LedManager: public ILedManager
{
    ILedDevice* _pLedDevice;
    int _channelCount;

    LedState* _pStates;
    LedState* _pDeltas;

    public:
        LedManager(ILedDevice* pLedDevice, int channelCount)
        {
            _channelCount = channelCount;
			_pLedDevice = pLedDevice;
			_pStates = new LedState[channelCount];
			_pDeltas = new LedState[channelCount];

			ResetState();
        }

		~LedManager()
		{
			delete _pStates;
			delete _pDeltas;
		}

		void ResetState()
		{
			for (int i = 0; i < _channelCount; i++)
			{
				Variable zero;
				_pStates[i] = LedState(i, &zero, 0);
				_pDeltas[i] = LedState(i, &zero, 0);
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

				if (ledState.GetChannel() < _channelCount)
				{
					// TODO: save target so that we get to the exact endpoint?
					Variable delta;
					for (int i = 0; i < pTargetBrightness->GetValueCount(); i++)
					{
						float itemDelta = (ledState.GetBrightness()->GetValueFloat(i) - _pStates[ledState.GetChannel()].GetBrightness()->GetValueFloat(i)) / ledState.GetCycleCount();

						delta.SetValue(i, itemDelta);
					}

					_pDeltas[ledState.GetChannel()] = LedState(ledState.GetChannel(), &delta, ledState.GetCycleCount());
				}
			}
		}

        void Tick()
        {
            for (int i = 0; i < _channelCount; i++)
            {
				if (_pDeltas[i].GetCycleCount() > 0)
				{
					_pStates[i].Update(_pDeltas[i]);
					_pDeltas[i].DecrementCycleCount();
				}
				_pLedDevice->UpdateLed(_pStates[i]);
            }
			
			_pLedDevice->Show();
        }
};