

class Supervisor
{
	CommandSource _commandSource;
	ParseErrors _parseErrors;

	Timebase *_pTimebase = 0;

	char *_pCurrentCommand = 0;
	char *_pNodeName = 0;

	Settings *_pSettings;

	bool _logInformation = true;

	volatile bool _shouldExecuteCode;
	volatile bool _shouldExecuteCodeLoaded;
	volatile int _executionCount;

public:
	~Supervisor()
	{
		if (_pTimebase)
		{
			delete _pTimebase;
		}

		if (_pCurrentCommand)
		{
			delete[] _pCurrentCommand;
		}

		if (_pNodeName)
		{
			delete[] _pNodeName;
		}
	}

	const static int MaxProgramSize = 16636;
	const static int MaxNodeNameSize = 128;

	void DisableLogInformation()
	{
		_logInformation = false;
	}

	void Init(ILedManager *pLedManager, Settings *pSettings, TimebaseCallback timebaseCallback, IButtonCreator *pButtonCreator)
	{
		_pCurrentCommand = new char[MaxProgramSize];
		*_pCurrentCommand = '\0';

		_pNodeName = new char[MaxNodeNameSize];
		*_pNodeName = '\0';

		_pTimebase = new Timebase(&_commandSource, pLedManager, &_parseErrors, timebaseCallback, pButtonCreator);

		_pSettings = pSettings;
		_pSettings->LoadProgramText(_pCurrentCommand, MaxProgramSize);
		_pSettings->LoadNodeName(_pNodeName, MaxNodeNameSize);
		if (*_pNodeName == '\0')
		{
			SafeString::StringCopy(_pNodeName, "Sequencer", MaxNodeNameSize);
		}

		_shouldExecuteCode = _pSettings->LoadShouldExecuteCode();
		_shouldExecuteCodeLoaded = _shouldExecuteCode;

		if (_logInformation)
		{
			Serial.println("Startup: ");

			Serial.print("Program will execute: ");
			Serial.println(_shouldExecuteCode);
			Serial.println("Program:");
			Serial.println(_pCurrentCommand);
		}
		if (_shouldExecuteCode)
		{
			_commandSource.SetCommand(_pCurrentCommand);
		}
	}

	void UpdateProgram(const char *pProgram)
	{
		// Initial code had support to abort execution, change the program, and then go off and start executing
		// with the new program. This has been unreliable; sometimes the abort fails and seems to corrupt saved data.
		// Current code performs a software reset.
		//_pTimebase->Abort();

		//while (_shouldExecuteCode)
		//{
		//Serial.println("Waiting for abort to finish");
		//TimeServices::TaskDelay(10);
		//}

		SafeString::StringCopy(_pCurrentCommand, pProgram, MaxProgramSize);

		if (_logInformation)
		{
			Serial.print("Program Updated: ");
			Serial.println((int)strlen(pProgram));
		}

		//_parseErrors.Clear();
		//_commandSource.SetCommand(_pCurrentCommand);
		//_pTimebase->ResetExecutionState();

		// Save the new program, and tell the system to start executing on restart.
		// If the reboot was because of an exception, program will not be started.
		_pSettings->SaveProgramText(pProgram);
		_pSettings->SaveShouldExecuteCode(true); // was false

		// delete the timebase. It holds onto led resource, and this will call the destructor
		_pTimebase->FreeDevices();

		EspFunctions::Restart();

		// this code will not execute; it ran in the old version that did on-the-fly update.

		_shouldExecuteCode = true;
		_executionCount = 0;

		if (_logInformation)
		{
			Serial.println(_pCurrentCommand);

			Serial.println("Starting execution");
		}
	}

	void UpdateNodeName(const char *pNodeName)
	{
		_pSettings->SaveNodeName(pNodeName);
		SafeString::StringCopy(_pNodeName, pNodeName, MaxNodeNameSize);
	}

	const char *GetNodeName()
	{
		return _pNodeName;
	}

	const char *GetCurrentProgram()
	{
		return _pCurrentCommand;
	}

	bool GetExecutingProgramState()
	{
		return _shouldExecuteCode;
	}

	const char *GetCurrentErrors()
	{
		return _parseErrors.FormatErrors();
	}

	CommandSource *GetCommandSource()
	{
		return &_commandSource;
	}

	int GetExecutionCount()
	{
		return _pTimebase->GetExecutionCount();
	}

	void Execute()
	{
		StackWatcher::Log("Supervisor::Execute");
		if (_shouldExecuteCode)
		{
			_executionCount++;
			if (_executionCount % 1000 == 0)
			{
				//Serial.print("ExecutionCount: ");
				//Serial.println(_executionCount);
			}

			//_pTimebase->RunProgram(1);
			bool keepRunning = _pTimebase->RunProgram(_pCurrentCommand);

			if (!keepRunning)
			{
				_shouldExecuteCode = false;
				return;
			}

			if (_parseErrors.GetErrorCount() != 0)
			{
				for (int i = 0; i < _parseErrors.GetErrorCount(); i++)
				{
					ParseErrorFound(_parseErrors.GetError(0)->_errorText, _parseErrors.GetError(0)->_lineNumber);
				}

				_shouldExecuteCode = false;
				Serial.println("Error detected; disabling execution...");
				Serial.println(_parseErrors.GetError(0)->_errorText);
				Serial.println(_parseErrors.GetError(0)->_lineNumber);
				return;
			}

			if (!_shouldExecuteCodeLoaded)
			{
				_pSettings->SaveShouldExecuteCode(true);
				_shouldExecuteCodeLoaded = true;
			}
		}
		else
		{
			_pTimebase->TaskDelay();
		}
	}

	void ExecuteLoop()
	{
		while (true)
		{
			Execute();
			_pTimebase->TaskDelay(); // ensures that we allow other tasks to run if there is a program with no animation keepRunning
									 //Serial.println(VariableStore::VariableStoreInstance.GetInUseCount());
									 //Serial.println(ESP.getFreeHeap());
		}
	}

	void PressButton(int buttonNumber)
	{
		_pTimebase->PressButton(buttonNumber);
	}
};