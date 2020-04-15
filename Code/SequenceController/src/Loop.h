class Loop
{
	static const int VariableNameMax = 128;

	char _variableName[VariableNameMax];
	Variable _variableStart;
	Variable _variableEnd;
	Variable _variableInc;
	ListParser* _pListParser;

	static const char* FindColonOrEnd(const char *pCommand)
	{
		while (*pCommand != ':' && *pCommand != '\0')
		{
			pCommand++;
		}
		return pCommand;
	}

    public:
		Loop()
		{
			_pListParser = new ListParser(1023, 1024);
		}

		~Loop()
		{
			delete _pListParser;
			_pListParser = 0;
		}

    bool Parse(const char* pCommand, ExecutionContext* pExecutionContent, ParseErrors* pParseErrors, int lineNumber, IExecutionFlow* pExecutionFlow = 0)
    {
		StackWatcher::Log("Loop::Parse");

		if (strncmp(pCommand, "FOR ", 4) != 0)
		{
			return false;
		}
		_variableName[0] = 0;
		_variableInc = Variable(1.0F);

		pCommand += 4;	// skip 'FOR '
		pCommand = VariableCollection::GetVariableName(pCommand, _variableName, VariableNameMax);

		if (strlen(_variableName) == 0)
		{
			pParseErrors->AddError("Error in FOR: ", "missing variable name", lineNumber);
			return false;
		}

		_pListParser->Parse(": ", pCommand);

		if (_pListParser->GetCount() < 2)
		{
			pParseErrors->AddError("Error in FOR: ", "missing range value(s)", lineNumber);
			return false;
		}

		_variableStart = pExecutionContent->Evaluate(_pListParser->GetItem(0), pParseErrors, lineNumber, pExecutionFlow);
		_variableEnd = pExecutionContent->Evaluate(_pListParser->GetItem(1), pParseErrors, lineNumber, pExecutionFlow);

		if (_pListParser->GetCount() > 2)
		{
			_variableInc = pExecutionContent->Evaluate(_pListParser->GetItem(2), pParseErrors, lineNumber, pExecutionFlow);
		}

		return true;
    }

    char* GetVariableName()
    {
		return _variableName;
    }

	Variable GetVariableStart()
    {
		return _variableStart;
    }

	Variable GetVariableEnd()
	{
		return _variableEnd;
	}

	Variable GetVariableInc()
	{
		return _variableInc;
	}

	static int GetIsInRange(float variableStart, float variableEnd, float value)
	{
		float min = variableStart < variableEnd ? variableStart : variableEnd;
		float max = variableStart < variableEnd ? variableEnd : variableStart;

		return value >= min && value <= max;
	}
};