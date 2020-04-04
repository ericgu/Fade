

class Supervisor
{
    CommandSource _commandSource;
    ParseErrors _parseErrors;    

	Timebase *_pTimebase = 0;

	char* _pCurrentCommand = 0;
	char* _pNodeName = 0;

    Settings *_pSettings;

	bool _shouldExecuteCode;
	bool _shouldExecuteCodeLoaded;
	int _executionCount = 0;

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
			strcpy(_pNodeName, "Sequencer");
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
		strcpy(_pCurrentCommand, pProgram);

		Serial.println("Program Updated");

		_parseErrors.Clear();
		_commandSource.SetCommand(_pCurrentCommand);
		_pTimebase->ResetExecutionState();

		_pSettings->SaveProgramText(pProgram);
		_pSettings->SaveShouldExecuteCode(false);
		_shouldExecuteCode = true;
		_executionCount = 0;

		Serial.println(_pCurrentCommand);
	}

	void UpdateNodeName(const char* pNodeName)
	{
		_pSettings->SaveNodeName(pNodeName);
		strcpy(_pNodeName, pNodeName);
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
		return _executionCount;
	}

	void Execute()
	{
		StackWatcher::Log("Supervisor::Execute");
		if (_shouldExecuteCode)
		{
			_executionCount++;

			if (_executionCount == 500 && !_shouldExecuteCodeLoaded)
			{
				_pSettings->SaveShouldExecuteCode(true);
			}

			_pTimebase->RunProgram(1);
			if (_parseErrors.GetErrorCount() != 0)
			{
				_shouldExecuteCode = false;
			}
		}
	}

	void ExecuteLoop()
	{
		while (true)
		{
			Serial.println("Supervisor::ExecuteLoop");
			Execute();
		}
	}	
};