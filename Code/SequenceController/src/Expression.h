class Expression
{
	Variable _result;

public:
	Variable* Parse(const char* pCommand, VariableCollection* pVariableCollection, FunctionStore* pFunctionStore, Stack* pStack, ParseErrors* pParseErrors, int lineNumber)
	{
		while (*pCommand == ' ')
		{
			pCommand++;
		}

		if ((*pCommand >= '0' && *pCommand <= '9') || *pCommand == '-')
		{
			_result = Variable::ParseFloat(pCommand);
			return &_result;
		}
		else
		{
			//Serial.print("Variable or function call: "); Serial.println(pCommand);

			if (strchr(pCommand, '(') != 0)
			{
				ListParser parts("()", pCommand);

				if (strcmp("R", parts.GetItem(0)) == 0)
				{
					//Serial.println("    found random: ");
					ListParser listParser(":", parts.GetItem(1));

					Variable minValue = Variable::ParseFloat(listParser.GetItem(0));
					Variable maxValue = Variable::ParseFloat(listParser.GetItem(1));

					_result = MyRandom::GetValue(minValue.GetValueInt(), maxValue.GetValueInt());

					return &_result;
				}
				else
				{
					int i = 12; // function call
					FunctionDefinition* pFunctionDefinition = pFunctionStore->Lookup(parts.GetItem(0));

					pStack->CreateFrame();
					StackFrame* pStackFrame = pStack->GetTopFrame();
					pStackFrame->SerialNumberStart = pFunctionDefinition->SerialNumberStart + 1;
					pStackFrame->SerialNumberEnd = pFunctionDefinition->SerialNumberEnd;
					pStackFrame->InstructionPointer = pFunctionDefinition->SerialNumberStart + 1;

					_result.SetValue(-1000000);
					return &_result;
				}

				return 0;
			}
			else
			{
				return pVariableCollection->Lookup(pCommand, pParseErrors, lineNumber);
			}
		}
	}
};