#include "BuiltInFunctions.h"

class IFunctionCaller
{
    public:
    virtual Variable Call(const char* pFunctionName, int lineNumber) = 0;
};

class FunctionCaller : public IFunctionCaller
{
	static char _argumentNameBuffer[10];

	FunctionStore* _pFunctionStore;
	Stack* _pStack;
	VariableCollection* _pVariableCollection;
	ParseErrors* _pParseErrors;
	IExecutionFlow* _pExecutionFlow;

public:

	static const char* GenerateArgumentName(int argumentNumber)
	{
		snprintf(_argumentNameBuffer, sizeof(_argumentNameBuffer), "#A%d", argumentNumber);

		return _argumentNameBuffer;
	}

	FunctionCaller(FunctionStore* pFunctionStore, Stack* pStack, VariableCollection* pVariableCollection, ParseErrors* pParseErrors, IExecutionFlow* pExecutionFlow)
	{
		_pFunctionStore = pFunctionStore;
		_pStack = pStack;
		_pVariableCollection = pVariableCollection;
		_pExecutionFlow = pExecutionFlow;
		_pParseErrors = pParseErrors;
	}

	Variable Call(const char* pFunctionName, int lineNumber)
	{
		Variable returnValue;
		returnValue.SetToNan();

		FunctionDefinition* pFunctionDefinition = _pFunctionStore->Lookup(pFunctionName);
		if (pFunctionDefinition != 0)
		{
			StackFrame* pStackFrame = _pStack->GetTopFrame();
			pStackFrame->SerialNumberStart = pFunctionDefinition->SerialNumberStart;
			pStackFrame->SerialNumberEnd = pFunctionDefinition->SerialNumberEnd;
			pStackFrame->SetInstructionPointer(pFunctionDefinition->SerialNumberStart, "DoFunctionCall");

    		StackWatcher::Log("FunctionCaller::Call");

			_pExecutionFlow->RunProgram(1);

			if (_pExecutionFlow->GetCommandResult()->GetAbort())
			{
				return returnValue;
			}

			Variable* pReturnValue = _pVariableCollection->GetWithoutErrorCheck("<ReturnValue>", _pStack->GetFrameCount());
			if (pReturnValue)
			{
				returnValue = *pReturnValue;
			}

			return returnValue;
		}

		if (!BuiltInFunctions::HandleBuiltInFunctions(pFunctionName, _pVariableCollection, _pStack, _pParseErrors, lineNumber, _pExecutionFlow, &returnValue))
		{
			_pParseErrors->AddError("Unrecognized function: ", pFunctionName, lineNumber);

			return Variable::Empty();
		}

		return returnValue;
	}
};

char FunctionCaller::_argumentNameBuffer[] = "";

