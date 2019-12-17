class Loop
{
	char _variableName[64];
	Variable _variableStart;
	Variable _variableEnd;
	Variable _variableInc;
	int _match;

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
			_variableName[0] = 0;
			_variableInc = Variable(1.0F);
			_match = 0;
		}

    static Loop Parse(const char* pCommand, ExecutionContext& executionContent, ParseErrors* pParseErrors, int lineNumber)
    {
		Loop loop;

		if (strncmp(pCommand, "FOR ", 4) != 0)
		{
			return loop;
		}

		pCommand += 4;	// skip 'FOR '

		pCommand = VariableCollection::GetVariableName(pCommand, loop._variableName);

		if (strlen(loop._variableName) == 0)
		{
			pParseErrors->AddError("Error in FOR: ", "missing variable name", lineNumber);
			return loop;
		}

		ListParser listParser(":", pCommand);

		if (listParser.GetCount() < 2)
		{
			pParseErrors->AddError("Error in FOR: ", "missing range value(s)", lineNumber);
			return loop;
		}

		loop._variableStart = *executionContent.Parse(listParser.GetItem(0), pParseErrors, lineNumber);
		loop._variableEnd = *executionContent.Parse(listParser.GetItem(1), pParseErrors, lineNumber);

		if (listParser.GetCount() > 2)
		{
			loop._variableInc = *executionContent.Parse(listParser.GetItem(2), pParseErrors, lineNumber);
		}

		loop._match = 1;

		return loop;
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

	int GetMatch()
	{
		return _match;
	}

	int GetIsInRange(float value)
	{
		float variableStart = _variableStart.GetValueFloat();
		float variableEnd = _variableEnd.GetValueFloat();

		float min = variableStart < variableEnd ? variableStart : variableEnd;
		float max = variableStart < variableEnd ? variableEnd : variableStart;

		return value >= min && value <= max;
	}
};