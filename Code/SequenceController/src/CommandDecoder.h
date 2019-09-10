#include "Loop.h"

class CommandDecoder
{
	static CommandResult DecodeDirect(ExecutionContext& executionContext, Command command)
	{
		CommandResult commandResult;

		if (!command.StartsWith("D")) { return commandResult; }

		ListParser listParser(',', command.GetString() + 1);

		for (int i = 0; i < listParser.GetCount(); i += 2)
		{
			Variable channel = executionContext._variables.ParseFloatOrVariable(listParser.GetItem(i));
			Variable brightness = executionContext._variables.ParseFloatOrVariable(listParser.GetItem(i + 1));

			commandResult.AddTarget(LedState(channel.GetValueInt(), brightness.GetValueFloat()));
		}
		commandResult.SetStatus(CommandResultStatus::CommandExecute);

		return commandResult;
	}

	static CommandResult DecodeSequential(ExecutionContext& executionContext, Command command)
	{
		CommandResult commandResult;

		if (!command.StartsWith("S")) { return commandResult; }

		ListParser listParser(',', command.GetString() + 1);

		for (int channel = 0; channel < listParser.GetCount(); channel++)
		{
			Variable brightness = executionContext._variables.ParseFloatOrVariable(listParser.GetItem(channel));
			commandResult.AddTarget(LedState(channel, brightness.GetValueFloat()));
		}
		commandResult.SetStatus(CommandResultStatus::CommandExecute);

		return commandResult;
	}

	static CommandResult DecodeLoopStart(ExecutionContext& executionContext, Command command)
	{
		CommandResult commandResult;

		Loop loop = Loop::Parse(command.GetString());

		if (!loop.GetMatch())
		{
			return commandResult;
		}

		// check top of stack to see if we're in process for this loop
		if (executionContext._stack.GetFrameCount() != 0 && 
			executionContext._stack.GetTopFrame().SerialNumberStart == command.GetSerialNumber())
		{
			executionContext._variables.Get(loop.GetVariableNumber()).Increment(loop.GetVariableInc());

			if (!loop.GetIsInRange(executionContext._variables.Get(loop.GetVariableNumber()).GetValueFloat()))
			{
				executionContext._variables.Get(loop.GetVariableNumber()).SetActiveFlag(false);
				commandResult.SetStatus(CommandResultStatus::CommandExitLoopBody);
				return commandResult;
			}

			commandResult.SetStatus(CommandResultStatus::CommandSkipToNext);
		}
		else // first time
		{
			executionContext._variables.Get(loop.GetVariableNumber()).SetActiveFlag(true);
			executionContext._variables.Get(loop.GetVariableNumber()).SetValue(loop.GetVariableStart());

			executionContext._stack.CreateFrame();
			executionContext._stack.GetTopFrame().SerialNumberStart = command.GetSerialNumber();

			commandResult.SetStatus(CommandResultStatus::CommandSkipToNext);
		}
		return commandResult;
	}

	static CommandResult DecodeLoopEnd(ExecutionContext& executionContext, Command command)
	{
		CommandResult commandResult;

		if (command.StartsWith("ENDLOOP"))
		{
			commandResult.SetStatus(CommandResultStatus::CommandEndOfLoop);
		}
		else
		{
			commandResult.SetStatus(CommandResultStatus::CommandNone);
		}

		return commandResult;
	}

    public:
    static CommandResult Decode(ExecutionContext& executionContext, Command command)
    {
		CommandResult commandResult;

		commandResult = DecodeDirect(executionContext, command);
		if (commandResult.HasStatus()) { return commandResult; }

		commandResult = DecodeSequential(executionContext, command);
		if (commandResult.HasStatus()) { return commandResult; }

		commandResult = DecodeLoopStart(executionContext, command);
		if (commandResult.HasStatus()) { return commandResult; }

		commandResult = DecodeLoopEnd(executionContext, command);
		if (commandResult.HasStatus()) { return commandResult; }

		return commandResult;
	}
};