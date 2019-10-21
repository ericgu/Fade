#pragma once

#include "Supervisor.h"

class SupervisorTest
{
	static void TestInitRunningCode()
	{
		Supervisor supervisor;
		LedPwmSimulator ledPwm(100);
		LedManager ledManager(&ledPwm, 1);

		Settings settings;
		settings.SaveProgramText("A(55)");
		settings.SaveShouldExecuteCode(true);

		Serial.SetOutput(false);
		supervisor.Init(&ledManager, &settings);
		Serial.SetOutput(true);

		Assert::AreEqual("A(55)", supervisor.GetCurrentProgram());
		Assert::AreEqual(1, supervisor.GetExecutingProgramState());

		CommandSource* pCommandSource = supervisor.GetCommandSource();
		Command command = pCommandSource->GetNextCommand();
		Assert::AreEqual("A(55)", command.GetString());

		Assert::AreEqual(0, supervisor.GetExecutionCount());
		supervisor.Execute();
		Assert::AreEqual(1, supervisor.GetExecutionCount());
	}

	static void TestInitNonRunningCode()
	{
		Supervisor supervisor;
		LedPwmSimulator ledPwm(100);
		LedManager ledManager(&ledPwm, 1);

		Settings settings;
		settings.SaveProgramText("A(55)");
		settings.SaveShouldExecuteCode(false);

		Serial.SetOutput(false);
		supervisor.Init(&ledManager, &settings);
		Serial.SetOutput(true);

		Assert::AreEqual("A(55)", supervisor.GetCurrentProgram());
		Assert::AreEqual(0, supervisor.GetExecutingProgramState());

		CommandSource* pCommandSource = supervisor.GetCommandSource();
		Command command = pCommandSource->GetNextCommand();
		Assert::AreEqual(-1, command.GetSerialNumber());

		Assert::AreEqual(0, supervisor.GetExecutionCount());
		supervisor.Execute();
		Assert::AreEqual(0, supervisor.GetExecutionCount());
	}

	static void TestNoProgram()
	{
		Supervisor supervisor;
		LedPwmSimulator ledPwm(100);
		LedManager ledManager(&ledPwm, 1);

		Settings settings;
		settings.SaveShouldExecuteCode(false);

		Serial.SetOutput(false);
		supervisor.Init(&ledManager, &settings);
		Serial.SetOutput(true);

		Assert::AreEqual("", supervisor.GetCurrentProgram());
		Assert::AreEqual(0, supervisor.GetExecutingProgramState());

		CommandSource* pCommandSource = supervisor.GetCommandSource();
		Command command = pCommandSource->GetNextCommand();
		Assert::AreEqual(-1, command.GetSerialNumber());
	}

	static void TestUpdatedProgram()
	{
		Supervisor supervisor;
		LedPwmSimulator ledPwm(100);
		LedManager ledManager(&ledPwm, 1);

		Settings settings;
		settings.SaveProgramText("A(55)");
		settings.SaveShouldExecuteCode(true);

		Serial.SetOutput(false);
		supervisor.Init(&ledManager, &settings);

		Assert::AreEqual("A(55)", supervisor.GetCurrentProgram());
		Assert::AreEqual(1, supervisor.GetExecutingProgramState());

		supervisor.Execute();
		Assert::AreEqual(1, supervisor.GetExecutionCount());

		supervisor.UpdateProgram("A(333)");
		Serial.SetOutput(true);

		Assert::AreEqual("A(333)", supervisor.GetCurrentProgram());
		Assert::AreEqual(1, supervisor.GetExecutingProgramState());

		CommandSource* pCommandSource = supervisor.GetCommandSource();
		Command command = pCommandSource->GetNextCommand();
		Assert::AreEqual("A(333)", command.GetString());

		Assert::AreEqual(0, settings.LoadShouldExecuteCode());
		char buffer[1024];
		settings.LoadProgramText(buffer, sizeof(buffer));
		Assert::AreEqual("A(333)", buffer);

		Assert::AreEqual(0, supervisor.GetExecutionCount());
		supervisor.Execute();
		Assert::AreEqual(1, supervisor.GetExecutionCount());
	}

	static void TestUpdateCodeSavesShouldExecuteAfter500Loops()
	{
		Supervisor supervisor;
		LedPwmSimulator ledPwm(1000);
		LedManager ledManager(&ledPwm, 1);

		Settings settings;

		Serial.SetOutput(false);
		supervisor.Init(&ledManager, &settings);

		supervisor.UpdateProgram("A(333)");

		// initially, we are executing locally but have not saved that state...
		Assert::AreEqual(1, supervisor.GetExecutingProgramState());
		Assert::AreEqual(0, settings.LoadShouldExecuteCode());

		for (int i = 0; i < 500; i++)
		{
			supervisor.Execute();
		}
		Assert::AreEqual(1, settings.LoadShouldExecuteCode());

		Serial.SetOutput(true);
	}

	static void TestCodeWithErrorsDisablesExecution()
	{
		Supervisor supervisor;
		LedPwmSimulator ledPwm(1000);
		LedManager ledManager(&ledPwm, 1);

		Settings settings;

		Serial.SetOutput(false);
		supervisor.Init(&ledManager, &settings);

		supervisor.UpdateProgram("B(333)");
		Assert::AreEqual(1, supervisor.GetExecutingProgramState());

		supervisor.Execute();
		Assert::AreEqual(0, supervisor.GetExecutingProgramState());
		Assert::AreEqual("Unrecognized command: B(333)\n", supervisor.GetCurrentErrors());

		Serial.SetOutput(true);
	}

	static void TestSavedRunningProgramDoesNotUpdateSettingsAfter500Loops()
	{
		Supervisor supervisor;
		LedPwmSimulator ledPwm(1000);
		LedManager ledManager(&ledPwm, 1);

		Settings settings;
		settings.SaveProgramText("A(3)");
		settings.SaveShouldExecuteCode(1);

		Serial.SetOutput(false);
		supervisor.Init(&ledManager, &settings);

		settings.SaveShouldExecuteCode(false);	// change value to ensure no update

		for (int i = 0; i < 501; i++)
		{
			supervisor.Execute();
		}
		Assert::AreEqual(0, settings.LoadShouldExecuteCode());

		Serial.SetOutput(true);
	}

	static void TestUpdatedNodeName()
	{
		Supervisor supervisor;
		LedPwmSimulator ledPwm(100);
		LedManager ledManager(&ledPwm, 1);

		Settings settings;

		Serial.SetOutput(false);
		supervisor.Init(&ledManager, &settings);

		supervisor.UpdateNodeName("MyNameIsFred");

		char value[128];
		settings.LoadNodeName(value, 128);
		Assert::AreEqual("MyNameIsFred", value);

		Assert::AreEqual("MyNameIsFred", supervisor.GetNodeName());
	}


public:
	static void Run()
	{
		TestInitRunningCode();
		TestInitNonRunningCode();
		TestNoProgram();
		TestUpdatedProgram();

		TestUpdateCodeSavesShouldExecuteAfter500Loops();
		TestCodeWithErrorsDisablesExecution();
		TestSavedRunningProgramDoesNotUpdateSettingsAfter500Loops();

		TestUpdatedNodeName();
	}
};
