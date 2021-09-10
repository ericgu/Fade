class LedManager : public ILedManager
{
	ILedDeviceCreator *_pLedDeviceCreator;
	LedGroups _ledGroups;
	//ILedDevice *_pLedDevice = 0;
	//int _channelCount;

	LedState *_pStates;
	LedState *_pDeltas;

	void Cleanup()
	{
		if (_pStates != 0)
		{
			delete[] _pStates;
			delete[] _pDeltas;
			_pStates = 0;
			_pDeltas = 0;
		}
	}

public:
	LedManager(ILedDeviceCreator *pLedDeviceCreator)
	{
		_pLedDeviceCreator = pLedDeviceCreator;

		_pStates = 0;
		_pDeltas = 0;

		//Configure("RGB", 1, 13);
	}

	~LedManager()
	{
		Cleanup();
	}

	int GetLedCount()
	{
		return _ledGroups.GetLedTotal();
	}

	void FreeDevices()
	{
		_ledGroups.FreeDevices();
	}

	void ResetState()
	{
		Variable zero;
		zero.SetValue(0, 0.0F);
		zero.SetValue(1, 0.0F);
		zero.SetValue(2, 0.0F);
		zero.SetValue(3, 0.0F);

		int size = _ledGroups.GetLedTotal();
		_pStates = new LedState[size];
		_pDeltas = new LedState[size];

		for (int i = 0; i < _ledGroups.GetLedTotal(); i++)
		{
			_pStates[i] = LedState(i, &zero, 0);
			_pDeltas[i] = LedState(i, &zero, 0);
		}
	}

	bool Configure(int ledGroupNumber, const char *pLedType, int ledCount, int ledPin1, int ledPin2, int ledPin3, int ledPin4)
	{
		if (strcmp(pLedType, "FREE") == 0)
		{
			_ledGroups.DeleteGroup(ledGroupNumber);
			return true;
		}

		// check whether group exists, skip if it doesn't...

		if (_ledGroups.GetGroupCount() > ledGroupNumber)
		{
			return true;
		}

		LedType ledType(pLedType);

		if (!ledType.IsValid())
		{
			return false;
		}

		ILedDevice *pLedDevice = _pLedDeviceCreator->Create(ledType, ledCount, ledPin1, ledPin2, ledPin3, ledPin4, this);
		if (!pLedDevice)
		{
			return false;
		}

		if (_ledGroups.AddGroup(ledGroupNumber, pLedDevice, ledCount))
		{

			Cleanup();
			ResetState();
		}

		return true;
	}

    void SetLedState(int channel, Variable *pBrightness)
	{
		//Serial.print("SetLedState: ");
		//Serial.print(channel);
		//Serial.print(" ");
		//Serial.print(pBrightness->GetValueFloat(0));

		//Serial.print(" Actual Channel: ");
		//Serial.println(_pStates[channel].GetChannel());

		_pStates[channel] = LedState(channel, pBrightness, 0);
	}


	void SetDelta(CommandResult *pCommandResult)
	{
		for (int item = 0; item < pCommandResult->GetCount(); item++)
		{
			LedState ledState = pCommandResult->GetTarget(item);
			Variable *pTargetBrightness = ledState.GetBrightness();

			if (ledState.GetChannel() < _ledGroups.GetLedTotal())
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
		for (int i = 0; i < _ledGroups.GetLedTotal(); i++)
		{
			//Serial.print("LedManager::Tick -> ");
			//Serial.println(i);
			//Serial.flush();
			//Serial.println((int)_pDeltas);
			if (_pDeltas[i].GetCycleCount() > 0)
			{
				//Serial.println("A1");
				//Serial.flush();
				_pStates[i].Update(_pDeltas[i]);
				//Serial.println("A2");
				//Serial.flush();
				_pDeltas[i].DecrementCycleCount();
				//Serial.println("A3");
				//Serial.flush();
			}
			//Serial.println("A3.5");
			//Serial.flush();

			_ledGroups.UpdateLed(_pStates[i].GetChannel(), _pStates[i].GetBrightness());
			//_ledGroups.UpdateLed(_pStates + i);
			//Serial.println("A4");
			//Serial.flush();
		}

		//Serial.println("A5");
		//Serial.flush();
		_ledGroups.Show();
		//Serial.println("A6");
		//Serial.flush();
	}
};