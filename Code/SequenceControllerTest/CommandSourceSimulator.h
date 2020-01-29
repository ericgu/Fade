#pragma once

class CommandSourceSimulator : public ICommandSource
{
	Command _commands[20];
	int _commandCount;

public:
	CommandSourceSimulator()
	{
		_commandCount = 0;
	}

	//void AddCommand(Command command)
	//{
	//	command.SetSerialNumber(_commandCount);
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

	Command GetCommand(int commandNumber)
	{
		if (commandNumber == _commandCount)
		{
			return Command(0, -1);
		}

		Command command = _commands[commandNumber];

		return command;
	}
};