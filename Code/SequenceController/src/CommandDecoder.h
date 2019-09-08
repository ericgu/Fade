#include "Loop.h"

class CommandDecoder
{
	static Variable ParseIntOrVariable(ExecutionContext& executionContext, const char* pCommand)
	{
		if (*pCommand == '%')
		{
			pCommand++;

			int variableNumber = *pCommand - 'A';

			return executionContext._variables.Get(variableNumber);

			// TODO: error check.
		}
		else
		{
			return Variable::ParseInt(pCommand);
		}
	}

	static Variable ParseFloatOrVariable(ExecutionContext& executionContext, const char* pCommand)
	{
		if (*pCommand == '%')
		{
			pCommand++;

			int variableNumber = *pCommand - 'A';

			return executionContext._variables.Get(variableNumber);

			// TODO: error check.
		}
		else
		{
			return Variable::ParseFloat(pCommand);
		}
	}

	static const char* SkipToCharOrNull(const char* pCommand, char c)
	{
		while (*pCommand != c && *pCommand != '\0')
		{
			pCommand++;
		}

		return pCommand;
	}

	static bool DecodeDirect(ExecutionContext& executionContext, Command command)
	{
		const char* pCommand = command.GetString();

		if (*pCommand != 'D')
		{
			return false;
		}

		pCommand++;

		while (true)
		{
			Variable channel = ParseIntOrVariable(executionContext, pCommand);
			pCommand = SkipToCharOrNull(pCommand, ',') + 1;

			Variable brightness = ParseFloatOrVariable(executionContext, pCommand);

			executionContext._brightnessTarget.AddTarget(LedState(channel.GetValueInt(), brightness.GetValueFloat()));

			pCommand = SkipToCharOrNull(pCommand, ',');

			if (*pCommand == '\0')
			{
				break;
			}
			pCommand++;
		}
		executionContext._commandResult = CommandResult::CommandExecute;

		return true;
	}

	static bool DecodeSequential(ExecutionContext& executionContext, Command command)
	{
		const char* pCommand = command.GetString();

		if (*pCommand != 'S')
		{
			return false;
		}
		pCommand++;

		int channel = 0;
		while (true)
		{
			Variable brightness = ParseFloatOrVariable(executionContext, pCommand);
			executionContext._brightnessTarget.AddTarget(LedState(channel, brightness.GetValueFloat()));
			channel++;

			pCommand = SkipToCharOrNull(pCommand, ',');

			if (*pCommand == '\0')
			{
				break;
			}
			pCommand++;
		}
		executionContext._commandResult = CommandResult::CommandExecute;

		return true;
	}

	static bool DecodeLoopStart(ExecutionContext& executionContext, Command command)
	{
		Loop loop = Loop::Parse(command.GetString());

		if (!loop.GetMatch())
		{
			return false;
		}

		// check top of stack to see if we're in process for this loop
		if (executionContext._stack.GetFrameCount() != 0 && 
			executionContext._stack.GetTopFrame().SerialNumberStart == command.GetSerialNumber())
		{
			executionContext._variables.Get(loop.GetVariableNumber()).Increment();

			if (executionContext._variables.Get(loop.GetVariableNumber()).GetValueInt() > loop.GetVariableMax())
			{
				executionContext._commandResult = CommandResult::CommandExitLoopBody;
				executionContext._variables.Get(loop.GetVariableNumber()).SetActiveFlag(false);
			}
			else
			{
				executionContext._commandResult = CommandResult::CommandSkipToNext;
			}
		}
		else // first time
		{
			executionContext._variables.Get(loop.GetVariableNumber()).SetActiveFlag(true);
			executionContext._variables.Get(loop.GetVariableNumber()).SetValueInt(loop.GetVariableMin());

			executionContext._commandResult = CommandResult::CommandSkipToNext;

			executionContext._stack.CreateFrame();
			executionContext._stack.GetTopFrame().SerialNumberStart = command.GetSerialNumber();
		}

		return true;
	}

	static bool DecodeLoopEnd(ExecutionContext& executionContext, Command command)
	{
		if (strncmp(command.GetString(), "ENDLOOP", 7) == 0)
		{
			executionContext._commandResult = CommandResult::CommandEndOfLoop;
		}

		return true;
	}

    public:
    static void Decode(ExecutionContext& executionContext, Command command)
    {
		if (DecodeDirect(executionContext, command)) { return; }

		if (DecodeSequential(executionContext, command)) { return; }

		if (DecodeLoopStart(executionContext, command)) { return; }

		if (DecodeLoopEnd(executionContext, command)) { return; }
	}

};