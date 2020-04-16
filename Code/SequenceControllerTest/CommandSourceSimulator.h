#pragma once

class CommandSourceSimulator : public ICommandSource
{
	Command _commands[50];
	int _commandCount;

public:
	CommandSourceSimulator()
	{
		_commandCount = 0;
	}

	//void AddCommand(Command command)
	//{
	//	command.SetLineNumber(_commandCount);
	//	_commands[_commandCount] = command;
	//	_commandCount++;
	//}

	void AddCommand(const char* pCommand)
	{
		while (*pCommand == ' ' || *pCommand == '\t')
		{
			pCommand++;
		}

		_commands[_commandCount] = Command(pCommand, _commandCount);
		_commandCount++;
	}

	void ClearCommands()
	{
		_commandCount = 0;
	}

	Command* GetCommand(int commandNumber)
	{
		if (commandNumber == _commandCount)
		{
			return 0;
		}

		return &_commands[commandNumber];
	}
};