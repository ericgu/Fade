#pragma once

class CommandSourceSimulator : public ICommandSource
{
	Command _commands[20];
	int _commandCount;
	int _currentCommand;

public:
	CommandSourceSimulator()
	{
		_commandCount = 0;
		_currentCommand = 0;
	}

	void AddCommand(Command command)
	{
		_commands[_commandCount] = command;
		_commandCount++;
	}

	void ClearCommands()
	{
		_commandCount = 0;
	}

	Command GetNextCommand()
	{
		if (_currentCommand == _commandCount)
		{
			return Command(0, -1);
		}

		Command command = _commands[_currentCommand];
		_currentCommand++;

		return command;
	}

	void Reset()
	{
		_currentCommand = 0;
	}

	void SetCommandToSerialNumber(int serialNumber)
	{
		for (_currentCommand = 0; _currentCommand < _commandCount; _currentCommand++)
		{
			if (_commands[_currentCommand].GetSerialNumber() == serialNumber)
			{
				return;
			}
		}
	}
};