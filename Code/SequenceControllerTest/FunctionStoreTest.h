#pragma once

#include "FunctionStore.h"

class FunctionStoreTest
{
	static void Assert(FunctionStore& functionStore, const char* pName, int serialNumberStart, int serialNumberend)
	{
		FunctionDefinition* pFunctionDefinition;
		pFunctionDefinition = functionStore.Lookup(pName);

		Assert::AreEqual(pName, pFunctionDefinition->Name);
		Assert::AreEqual(serialNumberStart, pFunctionDefinition->SerialNumberStart);
		Assert::AreEqual(serialNumberend, pFunctionDefinition->SerialNumberEnd);
	}

	static void TestDefineFunction()
	{
		FunctionStore functionStore;
		ParseErrors parseErrors;

		functionStore.DefineStart("Func", &parseErrors, 14);
		Assert::AreEqual(true, functionStore.GetIsCurrentlyParsingFunction());
		functionStore.DefineEnd(16);
		Assert::AreEqual(false, functionStore.GetIsCurrentlyParsingFunction());

		Assert::AreEqual(1, functionStore.GetCount());

		Assert(functionStore, "Func", 14, 16);
	}

	static void TestDefineFunctionTwice()
	{
		FunctionStore functionStore;
		ParseErrors parseErrors;

		functionStore.DefineStart("Func", &parseErrors, 14);
		functionStore.DefineEnd(16);
		functionStore.DefineStart("Func", &parseErrors, 14);

		Assert::AreEqual(1, functionStore.GetCount());

		Assert(functionStore, "Func", 14, 16);
	}

	static void TestDefineTwoFunctions()
	{
		FunctionStore functionStore;
		ParseErrors parseErrors;

		functionStore.DefineStart("Func", &parseErrors, 14);
		functionStore.DefineEnd(16);
		functionStore.DefineStart("Func2", &parseErrors, 33);
		functionStore.DefineEnd(77);

		Assert::AreEqual(2, functionStore.GetCount());

		Assert(functionStore, "Func", 14, 16);
		Assert(functionStore, "Func2", 33, 77);
	}

	static void TestNameTooLong()
	{
		FunctionStore functionStore;
		ParseErrors parseErrors;

		functionStore.DefineStart("Function90Function90Function90Function90Function90Function90Function90Function90Function90Function90Function90Function90Function90", &parseErrors, 14);

		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Function: name too long", parseErrors.GetError(0)._errorText);
	}

	static void TestTooManyFunctions()
	{
		FunctionStore functionStore;
		ParseErrors parseErrors;

		for (int i = 0; i < 12; i++)
		{
			char buffer[20];
			sprintf(buffer, "Func%d", i);
			functionStore.DefineStart(buffer, &parseErrors, 14); 
			functionStore.DefineEnd(16);
		}

		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Function: too many defined", parseErrors.GetError(0)._errorText);
	}


public:
	static void Run()
	{
		TestDefineFunction();
		TestDefineFunctionTwice();
		TestDefineTwoFunctions();
		TestNameTooLong();
		TestTooManyFunctions();
	}
};
#pragma once
