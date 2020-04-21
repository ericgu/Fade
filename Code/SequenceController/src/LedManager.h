
class ILedManager
{
public:
	virtual void SetDelta(CommandResult *commandResult) = 0;

	virtual void Tick() = 0;
	virtual void ResetState() = 0;

	virtual void Configure(const char *pLedType, int ledCount, int ledPin) = 0;
};

class LedManager : public ILedManager
{
	ILedDeviceCreator *_pLedDeviceCreator;
	ILedDevice *_pLedDevice = 0;
	int _channelCount;

	LedState *_pStates;
	LedState *_pDeltas;

	void Cleanup()
	{
		if (_pStates != 0)
		{
			delete _pStates;
			delete _pDeltas;
			_pStates = 0;
			_pDeltas = 0;
		}
	}

public:
	LedManager(ILedDeviceCreator *pLedDeviceCreator)
	{
		_pLedDeviceCreator = pLedDeviceCreator;

		_channelCount = 0;
		_pStates = 0;
		_pDeltas = 0;
		_pLedDevice = 0;

		Configure("RGB", 1, 13);
	}

	~LedManager()
	{
		Cleanup();
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

	void Configure(const char *pLedType, int ledCount, int ledPin)
	{
		ILedDevice *pLedDevice = _pLedDeviceCreator->Create(pLedType, ledCount, ledPin);

		if (pLedDevice != 0)
		{

			_pLedDevice = pLedDevice;
			_channelCount = ledCount;

			Cleanup();
			_pStates = new LedState[_channelCount];
			_pDeltas = new LedState[_channelCount];

			ResetState();
		}
	}

	void SetDelta(CommandResult *pCommandResult)
	{
		//CommandResult commandResult = *pcommandResult;
		//for (int channel = 0; channel < _channelCount; channel++)
		//{
		//	_deltas[channel] = LedState(channel, 0, 0);
		//}

		for (int item = 0; item < pCommandResult->GetCount(); item++)
		{
			LedState ledState = pCommandResult->GetTarget(item);
			Variable *pTargetBrightness = ledState.GetBrightness();

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
			//Serial.print("LedManager::Tick -> ");
			//Serial.println(i);
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