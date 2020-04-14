#include "RDEvaluater.h"

class FunctionCallerSimulator : public IFunctionCaller
{
	float _returnValue;
	VariableCollection* _pVariableCollection;
	Stack* _pStack;
	char _functionName[64];

public:
	FunctionCallerSimulator(VariableCollection* pVariableCollection, Stack* pStack)
	{
		_pVariableCollection = pVariableCollection;
		_pStack = pStack;
	}

	void SetReturnValue(float returnValue)
	{
		_returnValue = returnValue;
	}

	Variable Call(const char* pFunctionName, int lineNumber)
	{
		strcpy(_functionName, pFunctionName);

		return Variable(_returnValue);
	}

	const char* GetFunctionCalled()
	{
		return _functionName;
	}
};

class RDEvaluaterTest
{
	static void TestEvaluation(const char* pExpression, float expected)
	{
		RDEvaluater rdEvaluater;

		Variable result = rdEvaluater.Evaluate(pExpression);

		Assert::AreEqual(expected, result.GetValueFloat(0));
	}

	static void TestPrimitiveNumber()
	{
		TestEvaluation("369", 369.0F);
	}

	static void TestMultiValueSingle()
	{
		RDEvaluater rdEvaluater;

		Variable result = rdEvaluater.Evaluate("{133}");

		Assert::AreEqual(1, result.GetValueCount());
		Assert::AreEqual(133.0F, result.GetValueFloat(0));
	}

	static void TestMultiValueNumber()
	{
		RDEvaluater rdEvaluater;

		Variable result = rdEvaluater.Evaluate("{1, 3, 2}");

		Assert::AreEqual(3, result.GetValueCount());
		Assert::AreEqual(1.0F, result.GetValueFloat(0));
		Assert::AreEqual(3.0F, result.GetValueFloat(1));
		Assert::AreEqual(2.0F, result.GetValueFloat(2));
	}

	static void TestMultiValueExpression()
	{
		RDEvaluater rdEvaluater;

		Variable result = rdEvaluater.Evaluate("{1 * 15, 3 + 33 + 333, 4 / 4 * 4}");

		Assert::AreEqual(3, result.GetValueCount());
		Assert::AreEqual(15.0F, result.GetValueFloat(0));
		Assert::AreEqual(369.0F, result.GetValueFloat(1));
		Assert::AreEqual(4.0F, result.GetValueFloat(2));
	}


	static void TestUnaryMinus()
	{
		TestEvaluation("-369", -369.0F);
	}

	static void TestUnaryPlus()
	{
		TestEvaluation("+15", 15.0F);
	}



	static void TestSingleMultiplication()
	{
		TestEvaluation("5 * 7", 35.0F);
	}

	static void TestSingleDivision()
	{
		TestEvaluation("100/20", 5.0F);
	}

	static void TestSingleModulus()
	{
		TestEvaluation("75 % 10", 5.0F);
	}

	static void TestMultiplyDivide()
	{
		TestEvaluation("10 * 20 / 5", 40.0F);
	}

	static void TestTripleMultiply()
	{
		TestEvaluation("10 * 20 * 5 * 5", 5000.0F);
	}



	static void TestSingleAddition()
	{
		TestEvaluation("5 + 7", 12.0F);
	}

	static void TestSingleSubtraction()
	{
		TestEvaluation("100 - 20", 80.0F);
	}

	static void TestAddThenSubtract()
	{
		TestEvaluation("10 + 20 - 5", 25.0F);
	}



	static void TestMultiplyAdd()
	{
		TestEvaluation("10 * 2 + 20 * 5", 120.0F);
	}



	static void TestLessThan()
	{
		TestEvaluation("5 < 6", 1.0F);
	}

	static void TestLessThanOrEqual()
	{
		TestEvaluation("6 <= 7", 1.0F);
	}

	static void TestGreaterThan()
	{
		TestEvaluation("10 > 5", 1.0F);
	}

	static void TestGreatherThanOrEqual()
	{
		TestEvaluation("10 >= 10", 1.0F);
	}

	static void TestLessThanFails()
	{
		TestEvaluation("10 < 9", 0.0F);
	}

	static void TestLessThanOrEqualFails()
	{
		TestEvaluation("11 <= 9", 0.0F);
	}

	static void TestGreaterThanFails()
	{
		TestEvaluation("12 > 154", 0.0F);
	}

	static void TestGreatherThanOrEqualFails()
	{
		TestEvaluation("12 >= 13", 0.0F);
	}



	static void TestEqualityEqual()
	{
		TestEvaluation("12 == 12", 1.0F);
	}

	static void TestEqualityNotEqual()
	{
		TestEvaluation("12 == 13", 0.0F);
	}

	static void TestNonEqualityEqual()
	{
		TestEvaluation("12 != 12", 0.0F);
	}

	static void TestNonEqualityNotEqual()
	{
		TestEvaluation("12 != 13", 1.0F);
	}



	static void TestLogicalAndNeither()
	{
		TestEvaluation("0 && 0", 0.0F);
	}

	static void TestLogicalAndFirst()
	{
		TestEvaluation("1 && 0", 0.0F);
	}

	static void TestLogicalAndSecond()
	{
		TestEvaluation("0 && 1", 0.0F);
	}

	static void TestLogicalAndBoth()
	{
		TestEvaluation("1 && 1", 1.0F);
	}



	static void TestLogicalOrNeither()
	{
		TestEvaluation("0 || 0", 0.0F);
	}

	static void TestLogicalOrFirst()
	{
		TestEvaluation("1 || 0", 1.0F);
	}

	static void TestLogicalOrSecond()
	{
		TestEvaluation("0 || 1", 1.0F);
	}

	static void TestLogicalOrBoth()
	{
		TestEvaluation("1 || 1", 1.0F);
	}



	static void TestMultiplyAddWithParens()
	{
		TestEvaluation("10 * (2 + 20) * 5", 1100.0F);
	}

	static void TestNestedParens()
	{
		TestEvaluation("10 * (2 + (5 + 1) * 2)", 140.0F);
	}



	static void TestVariableReference()
	{
		// TODO: Add in correct stack level checking..

		VariableCollection variableCollection;
		Stack stack;
		RDEvaluater rdEvaluater;
		variableCollection.AddAndSet("MyFirstValue", &Variable(22), 0);

		Variable result = rdEvaluater.Evaluate("MyFirstValue", &variableCollection, &stack);

		Assert::AreEqual(22.0F, result.GetValueFloat(0));
	}

	static void TestVariableReferenceInExpressions()
	{
		// TODO: Add in correct stack level checking..

		VariableCollection variableCollection;
		Stack stack;
		RDEvaluater rdEvaluater;
		variableCollection.AddAndSet("MyFirstValue", &Variable(20), 0);
		variableCollection.AddAndSet("MySecondValue", &Variable(5), 0);

		Variable result = rdEvaluater.Evaluate("MyFirstValue * 3 + MySecondValue", &variableCollection, &stack);

		Assert::AreEqual(65.0F, result.GetValueFloat(0));
	}

	static void TestVariableIncrement()
	{
		// TODO: Add in correct stack level checking..

		VariableCollection variableCollection;
		Stack stack;
		RDEvaluater rdEvaluater;
		variableCollection.AddAndSet("MyFirstValue", &Variable(22), 0);

		Variable result = rdEvaluater.Evaluate("MyFirstValue++", &variableCollection, &stack);

		Assert::AreEqual(22.0F, result.GetValueFloat(0));
		Assert::AreEqual(23.0F, variableCollection.GetWithoutErrorCheck("MyFirstValue", 0)->GetValueFloat(0));
	}

	static void TestVariableDecrement()
	{
		// TODO: Add in correct stack level checking..

		VariableCollection variableCollection;
		Stack stack;
		RDEvaluater rdEvaluater;
		variableCollection.AddAndSet("MyFirstValue", &Variable(22), 0);

		Variable result = rdEvaluater.Evaluate("MyFirstValue--", &variableCollection, &stack);

		Assert::AreEqual(22.0F, result.GetValueFloat(0));
		Assert::AreEqual(21.0F, variableCollection.GetWithoutErrorCheck("MyFirstValue", 0)->GetValueFloat(0));
	}



	static void TestAssignment()
	{
		// TODO: Add in correct stack level checking..

		VariableCollection variableCollection;
		Stack stack;
		RDEvaluater rdEvaluater;

		Variable result = rdEvaluater.Evaluate("AssignedValue = 15", &variableCollection, &stack);

		Assert::AreEqual(1, result.IsNan());
		Assert::AreEqual(1, variableCollection.GetActiveVariableCount());
		Variable* pAssignedValue = variableCollection.Get("AssignedValue", 0, 0, 1);
		Assert::AreEqual(15.0F, pAssignedValue->GetValueFloat(0));
	}

	static void TestAssignmentMultiValue()
	{
		// TODO: Add in correct stack level checking..

		VariableCollection variableCollection;
		Stack stack;
		RDEvaluater rdEvaluater;

		Variable result = rdEvaluater.Evaluate("AssignedValue = {15, 20, 25}", &variableCollection, &stack);

		Assert::AreEqual(1, result.IsNan());
		Assert::AreEqual(1, variableCollection.GetActiveVariableCount());
		Variable* pAssignedValue = variableCollection.Get("AssignedValue", 0, 0, 1);

		Assert::AreEqual(3, pAssignedValue->GetValueCount());
		Assert::AreEqual(15.0F, pAssignedValue->GetValueFloat(0));
		Assert::AreEqual(20.0F, pAssignedValue->GetValueFloat(1));
		Assert::AreEqual(25.0F, pAssignedValue->GetValueFloat(2));
	}



	static void TestFunctionCallParsing()
	{
		VariableCollection variableCollection;
		RDEvaluater rdEvaluater;
		Stack stack;
		FunctionCallerSimulator functionCaller(&variableCollection, &stack);
		functionCaller.SetReturnValue(129393);

		Variable result = rdEvaluater.Evaluate("MyFunction(15, 20)", &variableCollection, &stack, &functionCaller);

		Assert::AreEqual(129393.0F, result.GetValueFloat(0));
		Assert::AreEqual("MyFunction", functionCaller.GetFunctionCalled());
	}

	static void TestFunctionCallNoParameters()
	{
		VariableCollection variableCollection;
		RDEvaluater rdEvaluater;
		Stack stack;
		FunctionCallerSimulator functionCaller(&variableCollection, &stack);
		functionCaller.SetReturnValue(129393);

		Variable result = rdEvaluater.Evaluate("MyFunction()", &variableCollection, &stack, &functionCaller);

		Assert::AreEqual(129393.0F, result.GetValueFloat(0));
		Assert::AreEqual("MyFunction", functionCaller.GetFunctionCalled());
	}


	static void TestPrimitiveString()
	{
		RDEvaluater rdEvaluater;

		Variable result = rdEvaluater.Evaluate("\"MyStringValue\"");

		Assert::AreEqual("MyStringValue", result.GetValueString());
	}



	static void TestAddWithMissingArgument()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("15 + ", 0, 0, 0, &parseErrors, 7);

		Assert::AreEqual(1, result.IsNan());
		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Missing value at end of expression", parseErrors.GetError(0)._errorText);
		Assert::AreEqual(7, parseErrors.GetError(0)._lineNumber);
	}

	static void TestAddWithWrongArgument()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("15 + +", 0, 0, 0, &parseErrors, 9);

		Assert::AreEqual(1, result.IsNan());
		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Missing value at end of expression", parseErrors.GetError(0)._errorText);
		Assert::AreEqual(9, parseErrors.GetError(0)._lineNumber);
	}

	static void TestUnknownCharacter()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("15 $ 12", 0, 0, 0, &parseErrors, 8);

		Assert::AreEqual(1, result.IsNan());
		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Missing value at end of expression22", parseErrors.GetError(0)._errorText);
		Assert::AreEqual(8, parseErrors.GetError(0)._lineNumber);
	}

	static void TestUnexpectedValueAfterParsing()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("15 12", 0, 0, 0, &parseErrors, 444);

		Assert::AreEqual(15.0, result.GetValueFloat(0));
		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Unexpected token remaining after parsing: 12", parseErrors.GetError(0)._errorText);
		Assert::AreEqual(444, parseErrors.GetError(0)._lineNumber);
	}

	static void TestMissingClosingParen()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("(15 * 12", 0, 0, 0, &parseErrors, 123);

		Assert::AreEqual(1, result.IsNan());
		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Missing ) in expression", parseErrors.GetError(0)._errorText);
		Assert::AreEqual(123, parseErrors.GetError(0)._lineNumber);
	}

	static void TestMissingClosingBrace()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("{15", 0, 0, 0, &parseErrors, 5512);

		Assert::AreEqual(1, result.IsNan());
		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Missing } in expression", parseErrors.GetError(0)._errorText);
		Assert::AreEqual(5512, parseErrors.GetError(0)._lineNumber);
	}

	static void TestUndeclaredVariable()
	{
		VariableCollection variableCollection;
		Stack stack;
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("MyFirstValue", &variableCollection, &stack, 0, &parseErrors, 122);

		Assert::AreEqual(1, result.IsNan());
		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Undefined variable: MyFirstValue", parseErrors.GetError(0)._errorText);
		Assert::AreEqual(122, parseErrors.GetError(0)._lineNumber);
	}

	static void TestUndeclaredVariable2()
	{
		VariableCollection variableCollection;
		Stack stack;
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;
		variableCollection.AddAndSet("MyFirstValue", &Variable(22), 0);

		Variable result = rdEvaluater.Evaluate("13 * (2 + MyFirstValue) * MyMissingValue * 33", &variableCollection, &stack, 0, &parseErrors, 55);

		Assert::AreEqual(1, result.IsNan());
		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Undefined variable: MyMissingValue", parseErrors.GetError(0)._errorText);
		Assert::AreEqual(55, parseErrors.GetError(0)._lineNumber);
	}

	static void TestFunctionCallNoParametersMissingClosingParen()
	{
		VariableCollection variableCollection;
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;
		Stack stack;
		FunctionCallerSimulator functionCaller(&variableCollection, &stack);
		functionCaller.SetReturnValue(129393);

		Variable result = rdEvaluater.Evaluate("MyFunction(", &variableCollection, &stack, &functionCaller, &parseErrors, 55);

		Assert::AreEqual(1, result.IsNan());
		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Missing closing ) in expression", parseErrors.GetError(0)._errorText);
		Assert::AreEqual(55, parseErrors.GetError(0)._lineNumber);
	}

	static void TestFunctionCallOneParametersMissingClosingParen()
	{
		VariableCollection variableCollection;
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;
		Stack stack;
		FunctionCallerSimulator functionCaller(&variableCollection, &stack);
		functionCaller.SetReturnValue(129393);

		Variable result = rdEvaluater.Evaluate("MyFunction(15", &variableCollection, &stack, &functionCaller, &parseErrors, 59);

		Assert::AreEqual(1, result.IsNan());
		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Missing closing ) in expression", parseErrors.GetError(0)._errorText);
		Assert::AreEqual(59, parseErrors.GetError(0)._lineNumber);
	}

	static void TestFunctionCallTwoParametersEndsAfterComma()
	{
		VariableCollection variableCollection;
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;
		Stack stack;
		FunctionCallerSimulator functionCaller(&variableCollection, &stack);
		functionCaller.SetReturnValue(129393);

		Variable result = rdEvaluater.Evaluate("MyFunction(15,", &variableCollection, &stack, &functionCaller, &parseErrors, 99);

		Assert::AreEqual(1, result.IsNan());
		Assert::AreEqual(1, parseErrors.GetErrorCount());
		Assert::AreEqual("Missing closing ) in expression", parseErrors.GetError(0)._errorText);
		Assert::AreEqual(99, parseErrors.GetError(0)._lineNumber);
	}

public:
	static void Run()
	{
		TestPrimitiveNumber();

		TestMultiValueSingle();
		TestMultiValueNumber();
		TestMultiValueExpression();

		TestUnaryMinus();
		TestUnaryPlus();

		TestSingleMultiplication();
		TestSingleDivision();
		TestSingleModulus();
		TestMultiplyDivide();
		TestTripleMultiply();

		TestSingleAddition();
		TestSingleSubtraction();
		TestAddThenSubtract();

		TestMultiplyAdd();

		TestLessThan();
		TestLessThanFails();
		TestLessThanOrEqual();
		TestLessThanOrEqualFails();
		TestGreaterThan();
		TestGreaterThanFails();
		TestGreatherThanOrEqual();
		TestGreatherThanOrEqualFails();

		TestEqualityEqual();
		TestEqualityNotEqual();
		TestNonEqualityEqual();
		TestNonEqualityNotEqual();

		TestLogicalAndNeither();
		TestLogicalAndFirst();
		TestLogicalAndSecond();
		TestLogicalAndBoth();

		TestLogicalOrNeither();
		TestLogicalOrFirst();
		TestLogicalOrSecond();
		TestLogicalOrBoth();

		TestMultiplyAddWithParens();
		TestNestedParens();

		TestVariableReference();
		TestVariableReferenceInExpressions();
		TestVariableIncrement();
		TestVariableDecrement();

		TestAssignment();
		TestAssignmentMultiValue();

		TestFunctionCallParsing();
		TestFunctionCallNoParameters();

		TestPrimitiveString();

		TestAddWithMissingArgument();
		TestAddWithWrongArgument();
		TestUnexpectedValueAfterParsing();
		TestMissingClosingParen();
		TestMissingClosingBrace();
		TestUndeclaredVariable();
		TestUndeclaredVariable2();
		TestFunctionCallNoParametersMissingClosingParen();
		TestFunctionCallOneParametersMissingClosingParen();
		TestFunctionCallTwoParametersEndsAfterComma();
	}
};
