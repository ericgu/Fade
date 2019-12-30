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
					FunctionDefinition* pFunctionDefinition = pFunctionStore->Lookup(parts.GetItem(0));

					if (pFunctionDefinition != 0)
					{
						pStack->CreateFrame();

						int argumentCount = 0;
						if (parts.GetCount() > 1)
						{
							ListParser arguments(",", parts.GetItem(1));
							argumentCount = arguments.GetCount();

							for (int i = 0; i < arguments.GetCount(); i++)
							{
								Variable* argument = Parse(arguments.GetItem(i), pVariableCollection, pFunctionStore, pStack, pParseErrors, lineNumber);
								char argumentName[10];
								sprintf(argumentName, "#A%d", i);

								pVariableCollection->AddAndSet(argumentName, argument->GetValueFloat(), pStack->GetFrameCount());
							}

						}
						pVariableCollection->AddAndSet("#A", (float) argumentCount, pStack->GetFrameCount());	// marks a function call. 

						StackFrame* pStackFrame = pStack->GetTopFrame();
						pStackFrame->SerialNumberStart = pFunctionDefinition->SerialNumberStart;
						pStackFrame->SerialNumberEnd = pFunctionDefinition->SerialNumberEnd;
						pStackFrame->InstructionPointer = pFunctionDefinition->SerialNumberStart;
						_result.SetToNan();
						return &_result;
					}
				}

				return 0;
			}
			else
			{
				return pVariableCollection->Lookup(pCommand, pStack->GetFrameCount(), pParseErrors, lineNumber);
			}
		}
	}
};