#pragma once
#include "CommandSourceSimulator.h"

class CommandSourceSimulatorTest
{
	static void TestSingle()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command(15, "D11,1.0", 0));

		Command command = commandSource.GetNextCommand();

		Assert::AreEqual(15, command.GetCount());
		Assert::AreEqual("D11,1.0", command.GetString());
	}

	static void TestMultiple()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command(15, "D11,1.0", 0));
		commandSource.AddCommand(Command(5, "D15,0.5", 1));

		Command command = commandSource.GetNextCommand();
		Assert::AreEqual(15, command.GetCount());
		Assert::AreEqual("D11,1.0", command.GetString());
		Assert::AreEqual(0, command.GetSerialNumber());

		command = commandSource.GetNextCommand();
		Assert::AreEqual(5, command.GetCount());
		Assert::AreEqual("D15,0.5", command.GetString());
		Assert::AreEqual(1, command.GetSerialNumber());

		command = commandSource.GetNextCommand();
		Assert::AreEqual(15, command.GetCount());
		Assert::AreEqual("D11,1.0", command.GetString());
		Assert::AreEqual(0, command.GetSerialNumber());
	}

	static void TestReset()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand(Command(15, "D11,1.0", 0));
		commandSource.AddCommand(Command(5, "D15,0.5", 1));
		commandSource.AddCommand(Command(9, "D9,0.5", 2));
		commandSource.AddCommand(Command(6, "D6,0.5", 3));

		Command command = commandSource.GetNextCommand();
		Assert::AreEqual(15, command.GetCount());
		Assert::AreEqual("D11,1.0", command.GetString());
		Assert::AreEqual(0, command.GetSerialNumber());

		command = commandSource.GetNextCommand();
		Assert::AreEqual(5, command.GetCount());
		Assert::AreEqual("D15,0.5", command.GetString());
		Assert::AreEqual(1, command.GetSerialNumber());

		command = commandSource.GetNextCommand();
		Assert::AreEqual(9, command.GetCount());
		Assert::AreEqual("D9,0.5", command.GetString());
		Assert::AreEqual(2, command.GetSerialNumber());

		commandSource.SetCommandToSerialNumber(1);

		command = commandSource.GetNextCommand();
		Assert::AreEqual(5, command.GetCount());
		Assert::AreEqual("D15,0.5", command.GetString());
		Assert::AreEqual(1, command.GetSerialNumber());
	}

public:
	static void Test()
	{
		TestSingle();
		TestMultiple();
		TestReset();
	}
};
