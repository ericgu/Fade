 typedef void (*SupervisorCallback)();

class Supervisor
{
    CommandSource _commandSource;
    ParseErrors _parseErrors;    

    Timebase *_pTimebase;

    char* _pCurrentCommand;
	char* _pNodeName;

    Settings *_pSettings;

	bool _shouldExecuteCode;
	bool _shouldExecuteCodeLoaded;
	int _executionCount = 0;

public:

	void Init(ILedManager* pLedManager, Settings* pSettings)
	{
		_pCurrentCommand = new char[16636];
		*_pCurrentCommand = '\0';

		_pNodeName = new char[128];
		*_pNodeName = '\0';

		_pTimebase = new Timebase(&_commandSource, pLedManager, &_parseErrors);

		_pSettings = pSettings;
		_pSettings->LoadProgramText(_pCurrentCommand, 16636);
		_pSettings->LoadNodeName(_pNodeName, 128);
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

	void ExecuteLoop(SupervisorCallback supervisorCallback)
	{
		while (true)
		{
			Execute();
			(*supervisorCallback)();
		}
	}	
};