#pragma once
#include "CommandSourceSimulator.h"

class CommandSourceSimulatorTest
{
	static void TestSingle()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("D 15 11,1.0");

		Command command = commandSource.GetCommand(0);

		Assert::AreEqual("D 15 11,1.0", command.GetString());
	}

	static void TestMultiple()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("D 15 11,1.0");
		commandSource.AddCommand("D 5 15,0.5");

		Command command = commandSource.GetCommand(0);
		Assert::AreEqual("D 15 11,1.0", command.GetString());
		Assert::AreEqual(0, command.GetSerialNumber());

		command = commandSource.GetCommand(1);
		Assert::AreEqual("D 5 15,0.5", command.GetString());
		Assert::AreEqual(1, command.GetSerialNumber());

		command = commandSource.GetCommand(2);
		Assert::AreEqual(-1, command.GetSerialNumber());
	}

	static void TestReset()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("D 15 11,1.0");
		commandSource.AddCommand("D 5 15,0.5");
		commandSource.AddCommand("D 9 9,0.5");
		commandSource.AddCommand("D 6 6,0.5");

		Command command = commandSource.GetCommand(0);
		Assert::AreEqual("D 15 11,1.0", command.GetString());
		Assert::AreEqual(0, command.GetSerialNumber());

		command = commandSource.GetCommand(1);
		Assert::AreEqual("D 5 15,0.5", command.GetString());
		Assert::AreEqual(1, command.GetSerialNumber());

		command = commandSource.GetCommand(2);
		Assert::AreEqual("D 9 9,0.5", command.GetString());
		Assert::AreEqual(2, command.GetSerialNumber());

		command = commandSource.GetCommand(1);
		Assert::AreEqual("D 5 15,0.5", command.GetString());
		Assert::AreEqual(1, command.GetSerialNumber());
	}

	static void TestWhitespace()
	{
		CommandSourceSimulator commandSource;

		commandSource.AddCommand("  D 15 11,1.0");

		Command command = commandSource.GetCommand(0);

		Assert::AreEqual("D 15 11,1.0", command.GetString());
	}

public:
	static void Test()
	{
		TestSingle();
		TestMultiple();
		TestReset();
		TestWhitespace();
	}
};
