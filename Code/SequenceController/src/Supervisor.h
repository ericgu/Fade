

class Supervisor
{
    CommandSource _commandSource;
    ParseErrors _parseErrors;    

	Timebase *_pTimebase = 0;

	char* _pCurrentCommand = 0;
	char* _pNodeName = 0;

    Settings *_pSettings;

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
			delete _pCurrentCommand;
		}

		if (_pNodeName)
		{
			delete _pNodeName;
		}
	}

	const static int MaxProgramSize = 16636;
	const static int MaxNodeNameSize = 128;

	void Init(ILedManager* pLedManager, Settings* pSettings, TimebaseCallback timebaseCallback)
	{
		_pCurrentCommand = new char[MaxProgramSize];
		*_pCurrentCommand = '\0';

		_pNodeName = new char[MaxNodeNameSize];
		*_pNodeName = '\0';

		_pTimebase = new Timebase(&_commandSource, pLedManager, &_parseErrors, timebaseCallback);

		_pSettings = pSettings;
		_pSettings->LoadProgramText(_pCurrentCommand, MaxProgramSize);
		_pSettings->LoadNodeName(_pNodeName, MaxNodeNameSize);
		if (*_pNodeName == '\0')
		{
			SafeString::StringCopy(_pNodeName, "Sequencer", MaxNodeNameSize);
		}

		_shouldExecuteCode = _pSettings->LoadShouldExecuteCode();
		_shouldExecuteCodeLoaded = _shouldExecuteCode;

		Serial.println("Startup state: ");

		Serial.print("Program will execute: ");
		Serial.println(_shouldExecuteCode);
		Serial.println("Program:");
		Serial.println(_pCurrentCommand);

		if (_shouldExecuteCode)
		{
			_commandSource.SetCommand(_pCurrentCommand);
		}
	}

	void UpdateProgram(const char* pProgram)
	{
		_pTimebase->Abort();

		while (_shouldExecuteCode)
		{
			Serial.println("Waiting for abort to finish");
			TimeServices::TaskDelay(10); 
		}

		SafeString::StringCopy(_pCurrentCommand, pProgram, MaxProgramSize);

		Serial.print("Program Updated: ");
		Serial.println((int) strlen(pProgram));

		_parseErrors.Clear();
		_commandSource.SetCommand(_pCurrentCommand);
		_pTimebase->ResetExecutionState();

		_pSettings->SaveProgramText(pProgram);
		_pSettings->SaveShouldExecuteCode(false);
		_shouldExecuteCode = true;
		_executionCount = 0;

		Serial.println(_pCurrentCommand);

		Serial.println("Starting execution");
	}

	void UpdateNodeName(const char* pNodeName)
	{
		_pSettings->SaveNodeName(pNodeName);
		SafeString::StringCopy(_pNodeName, pNodeName, MaxNodeNameSize);
	}

	const char* GetNodeName()
	{
		return _pNodeName;
	}

	const char* GetCurrentProgram()
	{
		return _pCurrentCommand;
	}

	bool GetExecutingProgramState()
	{
		return _shouldExecuteCode;
	}

	const char* GetCurrentErrors()
	{
		return _parseErrors.FormatErrors();
	}

	CommandSource* GetCommandSource()
	{
		return &_commandSource;
	}

	int GetExecutionCount()
	{
		return _pTimebase->GetExecutionCount();
	}

	void Execute()
	{
		// StackWatcher::Log("Supervisor::Execute");
		if (_shouldExecuteCode)
		{
			_executionCount++;
			//Serial.print("ExecutionCount: "); Serial.println(_executionCount);

			_pTimebase->RunProgram(1);
			if (_parseErrors.GetErrorCount() != 0)
			{
				_shouldExecuteCode = false;
				Serial.println("Error detected; disabling execution...");
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
		}
	}	
};