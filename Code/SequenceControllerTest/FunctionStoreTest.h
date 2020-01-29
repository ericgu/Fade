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

		functionStore.DefineStart("Func", 14);
		Assert::AreEqual(true, functionStore.GetIsCurrentlyParsingFunction());
		functionStore.DefineEnd(16);
		Assert::AreEqual(false, functionStore.GetIsCurrentlyParsingFunction());

		Assert::AreEqual(1, functionStore.GetCount());

		Assert(functionStore, "Func", 14, 16);
	}

	static void TestDefineFunctionTwice()
	{
		FunctionStore functionStore;

		functionStore.DefineStart("Func", 14);
		functionStore.DefineEnd(16);
		functionStore.DefineStart("Func", 14);

		Assert::AreEqual(1, functionStore.GetCount());

		Assert(functionStore, "Func", 14, 16);
	}

	static void TestDefineTwoFunctions()
	{
		FunctionStore functionStore;

		functionStore.DefineStart("Func", 14);
		functionStore.DefineEnd(16);
		functionStore.DefineStart("Func2", 33);
		functionStore.DefineEnd(77);

		Assert::AreEqual(2, functionStore.GetCount());

		Assert(functionStore, "Func", 14, 16);
		Assert(functionStore, "Func2", 33, 77);
	}


public:
	static void Run()
	{
		TestDefineFunction();
		TestDefineFunctionTwice();
		TestDefineTwoFunctions();
	}
};
#pragma once
