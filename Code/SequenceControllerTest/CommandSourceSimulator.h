#pragma once

class CommandSourceSimulator : public ICommandSource
{
	Command _commands[10];
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

	Command GetNextCommand(int autoReset = 1)
	{
		Command command = _commands[_currentCommand];
		_currentCommand = (_currentCommand + 1) % _commandCount;

		return command;
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