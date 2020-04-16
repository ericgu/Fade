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
	IExecutionContext* _pExecutionContext;
	ParseErrors* _pParseErrors;
	IExecutionFlow* _pExecutionFlow;

public:

	static const char* GenerateArgumentName(int argumentNumber)
	{
		snprintf(_argumentNameBuffer, sizeof(_argumentNameBuffer), "#A%d", argumentNumber);

		return _argumentNameBuffer;
	}

	FunctionCaller(FunctionStore* pFunctionStore, IExecutionContext* pExecutionContext, ParseErrors* pParseErrors, IExecutionFlow* pExecutionFlow)
	{
		_pFunctionStore = pFunctionStore;
		_pExecutionContext = pExecutionContext;
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
			StackFrame* pStackFrame = _pExecutionContext->StackTopFrame();
			pStackFrame->LineNumberStart = pFunctionDefinition->LineNumberStart;
			pStackFrame->SetInstructionPointer(pFunctionDefinition->LineNumberStart, "DoFunctionCall");

    		StackWatcher::Log("FunctionCaller::Call");

			_pExecutionFlow->RunProgram(1);

			if (_pExecutionFlow->GetCommandResult()->GetAbort())
			{
				return returnValue;
			}

			Variable* pReturnValue = _pExecutionContext->GetVariableWithoutErrorCheck("<ReturnValue>");
			if (pReturnValue)
			{
				returnValue = *pReturnValue;
			}

			return returnValue;
		}

		if (!BuiltInFunctions::HandleBuiltInFunctions(pFunctionName, _pExecutionContext, _pParseErrors, lineNumber, _pExecutionFlow, &returnValue))
		{
			_pParseErrors->AddError("Unrecognized function: ", pFunctionName, lineNumber);

			return Variable::Empty();
		}

		return returnValue;
	}
};

char FunctionCaller::_argumentNameBuffer[] = "";

