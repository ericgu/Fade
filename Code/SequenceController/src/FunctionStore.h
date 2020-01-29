class FunctionDefinition
{
public:
	FunctionDefinition()
	{
		SerialNumberStart = 0;
		SerialNumberEnd = 0;
		Name[0] = '\0';
	}

	FunctionDefinition(const char* pName, int serialNumberStart)
	{
		strcpy(Name, pName);
		SerialNumberStart = serialNumberStart;
		SerialNumberEnd = -1;
	}

	char Name[128];
	int SerialNumberStart;
	int SerialNumberEnd;
};

class FunctionStore
{
	int _functionDefinitionCount = 0;
	FunctionDefinition _functionDefinitions[10];

	bool _isCurrentlyParsingFunction;
	FunctionDefinition* _pCurrentFunctionDefinition;

public:
	FunctionStore()
	{
		_isCurrentlyParsingFunction = false;
		_pCurrentFunctionDefinition = 0;
	}

	bool GetIsCurrentlyParsingFunction()
	{
		return _isCurrentlyParsingFunction;
	}

	int GetCount() { return _functionDefinitionCount; }

	void DefineStart(const char* pFunctionName, int serialNumberStart)
	{
		FunctionDefinition* pLookup = Lookup(pFunctionName);

		if (pLookup != 0)
		{
			return;
		}

		FunctionDefinition* pCurrent = _functionDefinitions + _functionDefinitionCount;
		strcpy(pCurrent->Name, pFunctionName);
		pCurrent->SerialNumberStart = serialNumberStart;
		pCurrent->SerialNumberEnd = -1;

		_functionDefinitionCount++;

		_pCurrentFunctionDefinition = pCurrent;
		_isCurrentlyParsingFunction = true;
	}

	void DefineEnd(int serialNumberEnd)
	{
		_pCurrentFunctionDefinition->SerialNumberEnd = serialNumberEnd;
		_pCurrentFunctionDefinition = 0;
		_isCurrentlyParsingFunction = false;
	}

	FunctionDefinition* Lookup(const char* pFunctionName)
	{
		FunctionDefinition* pCurrent = _functionDefinitions;

		for (int i = 0; i < _functionDefinitionCount; i++)
		{
			if (strcmp(pFunctionName, pCurrent->Name) == 0)
			{
				return pCurrent;
			}
			pCurrent++;
		}

		return 0;
	}
};