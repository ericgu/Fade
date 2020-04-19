
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

	static void TestEmptyString()
	{
		RDEvaluater rdEvaluater;

		Variable result = rdEvaluater.Evaluate("");

		Assert::AreEqual(1, result.IsNan());
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

		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;
		executionContext.AddVariableAndSet("MyFirstValue", &Variable(22));

		Variable result = rdEvaluater.Evaluate("MyFirstValue", &executionContext);

		Assert::AreEqual(22.0F, result.GetValueFloat(0));
	}

	static void TestVariableReferenceInExpressions()
	{
		// TODO: Add in correct stack level checking..

		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;
		executionContext.AddVariableAndSet("MyFirstValue", &Variable(20));
		executionContext.AddVariableAndSet("MySecondValue", &Variable(5));

		Variable result = rdEvaluater.Evaluate("MyFirstValue * 3 + MySecondValue", &executionContext);

		Assert::AreEqual(65.0F, result.GetValueFloat(0));
	}

	static void TestVariableIncrement()
	{
		// TODO: Add in correct stack level checking..

		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;
		executionContext.AddVariableAndSet("MyFirstValue", &Variable(22));

		Variable result = rdEvaluater.Evaluate("MyFirstValue++", &executionContext);

		Assert::AreEqual(22.0F, result.GetValueFloat(0));
		Assert::AreEqual(23.0F, executionContext.GetVariableWithoutErrorCheck("MyFirstValue")->GetValueFloat(0));
	}

	static void TestVariableDecrement()
	{
		// TODO: Add in correct stack level checking..

		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;
		executionContext.AddVariableAndSet("MyFirstValue", &Variable(22));

		Variable result = rdEvaluater.Evaluate("MyFirstValue--", &executionContext);

		Assert::AreEqual(22.0F, result.GetValueFloat(0));
		Assert::AreEqual(21.0F, executionContext.GetVariableWithoutErrorCheck("MyFirstValue")->GetValueFloat(0));
	}



	static void TestAssignment()
	{
		// TODO: Add in correct stack level checking..

		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;

		Variable result = rdEvaluater.Evaluate("AssignedValue = 15", &executionContext);

		Assert::AreEqual(1, result.IsNan());
		Assert::AreEqual(1, executionContext.Variables()->GetActiveVariableCount());
		Variable* pAssignedValue = executionContext.GetVariableWithoutErrorCheck("AssignedValue");
		Assert::AreEqual(15.0F, pAssignedValue->GetValueFloat(0));
	}

	static void TestAssignmentMultiValue()
	{
		// TODO: Add in correct stack level checking..

		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;

		Variable result = rdEvaluater.Evaluate("AssignedValue = {15, 20, 25}", &executionContext);

		Assert::AreEqual(1, result.IsNan());
		Assert::AreEqual(1, executionContext.Variables()->GetActiveVariableCount());
		Variable* pAssignedValue = executionContext.GetVariableWithoutErrorCheck("AssignedValue");

		Assert::AreEqual(3, pAssignedValue->GetValueCount());
		Assert::AreEqual(15.0F, pAssignedValue->GetValueFloat(0));
		Assert::AreEqual(20.0F, pAssignedValue->GetValueFloat(1));
		Assert::AreEqual(25.0F, pAssignedValue->GetValueFloat(2));
	}



	static void TestFunctionCallParsing()
	{
		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;
		FunctionCallerSimulator functionCaller(executionContext.Variables(), executionContext.GetStack());
		functionCaller.SetReturnValue(129393);

		Variable result = rdEvaluater.Evaluate("MyFunction(15, 20)", &executionContext, &functionCaller);

		Assert::AreEqual(129393.0F, result.GetValueFloat(0));
		Assert::AreEqual("MyFunction", functionCaller.GetFunctionCalled());
	}

	static void TestFunctionCallNoParameters()
	{
		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;
		FunctionCallerSimulator functionCaller(executionContext.Variables(), executionContext.GetStack());
		functionCaller.SetReturnValue(129393);

		Variable result = rdEvaluater.Evaluate("MyFunction()", &executionContext, &functionCaller);

		Assert::AreEqual(129393.0F, result.GetValueFloat(0));
		Assert::AreEqual("MyFunction", functionCaller.GetFunctionCalled());
	}


	static void TestPrimitiveString()
	{
		RDEvaluater rdEvaluater;

		Variable result = rdEvaluater.Evaluate("\"MyStringValue\"");

		Assert::AreEqual("MyStringValue", result.GetValueString());
	}

	static void ValidateParseErrors(ParseErrors* pParseErrors, const char* pMessage, int lineNumber)
	{
		Assert::AreEqual(1, pParseErrors->GetErrorCount());
		Assert::AreEqual(pMessage, pParseErrors->GetError(0)->_errorText);
		Assert::AreEqual(lineNumber, pParseErrors->GetError(0)->_lineNumber);
	}

	static void TestAddWithMissingArgument()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("15 + ", 0, 0, &parseErrors, 7);

		Assert::AreEqual(1, result.IsNan());
		ValidateParseErrors(&parseErrors, "Missing value at end of expression", 7);
	}

	static void TestAddWithWrongArgument()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("15 + +", 0, 0, &parseErrors, 9);

		Assert::AreEqual(1, result.IsNan());
		ValidateParseErrors(&parseErrors, "Missing value at end of expression", 9);
	}

	static void TestUnknownCharacter()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("15 $ 12", 0, 0, &parseErrors, 8);

		Assert::AreEqual(1, result.IsNan());
		ValidateParseErrors(&parseErrors, "Missing value at end of expression", 8);
	}

	static void TestUnexpectedValueAfterParsing()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("15 12", 0, 0, &parseErrors, 444);

		Assert::AreEqual(15.0, result.GetValueFloat(0));
		ValidateParseErrors(&parseErrors, "Unexpected token remaining after parsing: 12", 444);
	}

	static void TestMissingClosingParen()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("(15 * 12", 0, 0, &parseErrors, 123);

		Assert::AreEqual(1, result.IsNan());
		ValidateParseErrors(&parseErrors, "Missing ) in expression", 123);
	}

	static void TestMissingClosingBrace()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("{15", 0, 0, &parseErrors, 5512);

		Assert::AreEqual(1, result.IsNan());
		ValidateParseErrors(&parseErrors, "Missing } in expression", 5512);
	}

	static void TestUndeclaredVariable()
	{
		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("MyFirstValue", &executionContext, 0, &parseErrors, 122);

		Assert::AreEqual(1, result.IsNan());
		ValidateParseErrors(&parseErrors, "Undefined variable: MyFirstValue", 122);
	}

	static void TestUndeclaredVariable2()
	{
		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;
		executionContext.AddVariableAndSet("MyFirstValue", &Variable(22));

		Variable result = rdEvaluater.Evaluate("13 * (2 + MyFirstValue) * MyMissingValue * 33", &executionContext, 0, &parseErrors, 55);

		Assert::AreEqual(1, result.IsNan());
		ValidateParseErrors(&parseErrors, "Undefined variable: MyMissingValue", 55);
	}

	static void TestFunctionCallNoParametersMissingClosingParen()
	{
		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;
		FunctionCallerSimulator functionCaller(executionContext.Variables(), executionContext.GetStack());
		functionCaller.SetReturnValue(129393);

		Variable result = rdEvaluater.Evaluate("MyFunction(", &executionContext, &functionCaller, &parseErrors, 55);

		Assert::AreEqual(1, result.IsNan());
		ValidateParseErrors(&parseErrors, "Missing closing ) in expression", 55);
	}

	static void TestFunctionCallOneParametersMissingClosingParen()
	{
		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;
		FunctionCallerSimulator functionCaller(executionContext.Variables(), executionContext.GetStack());
		functionCaller.SetReturnValue(129393);

		Variable result = rdEvaluater.Evaluate("MyFunction(15", &executionContext, &functionCaller, &parseErrors, 59);

		Assert::AreEqual(1, result.IsNan());
		ValidateParseErrors(&parseErrors, "Missing closing ) in expression", 59);
	}

	static void TestFunctionCallTwoParametersEndsAfterComma()
	{
		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;
		FunctionCallerSimulator functionCaller(executionContext.Variables(), executionContext.GetStack());
		functionCaller.SetReturnValue(129393);

		Variable result = rdEvaluater.Evaluate("MyFunction(15,", &executionContext, &functionCaller, &parseErrors, 99);

		Assert::AreEqual(1, result.IsNan());
		ValidateParseErrors(&parseErrors, "Missing closing ) in expression", 99);
	}

    static void DoTestIf(int conditionValue, int expectedValue)
    {
        ExecutionContext executionContext;
        RDEvaluater rdEvaluater;
        ParseErrors parseErrors;
        FunctionCallerSimulator functionCaller(executionContext.Variables(), executionContext.GetStack());
        executionContext.AddVariableAndSet("V", &Variable(conditionValue));

        const char* pCode = " \
IF (V == 1) \n\
J = 2\n\
ELSEIF (V == 2)\n\
J = 3\n\
ELSEIF (V == 3)\n\
J = 4\n\
ELSE\n\
J = 5\n\
ENDIF\n\
J\
";

        Variable result = rdEvaluater.Evaluate(pCode, &executionContext, &functionCaller, &parseErrors, 99);

        Assert::AreEqual(expectedValue, result.GetValueInt());
    }

    static void TestIf()
    {
        DoTestIf(1, 2);
        DoTestIf(2, 3);
        DoTestIf(3, 4);
        DoTestIf(4, 5);
    }

    static void DoTestNestedIf(int conditionValue1, int conditionValue2, int expectedValue)
    {
        ExecutionContext executionContext;
        RDEvaluater rdEvaluater;
        ParseErrors parseErrors;
        FunctionCallerSimulator functionCaller(executionContext.Variables(), executionContext.GetStack());
        executionContext.AddVariableAndSet("V", &Variable(conditionValue1));
        executionContext.AddVariableAndSet("X", &Variable(conditionValue2));

        const char* pCode = " \
IF (V == 1) \n\
IF (X == 1) \n\
J = 2\n\
ELSE\n\
J = 3\n\
ENDIF\n\
ELSE\n\
IF (X == 1) \n\
J = 4\n\
ELSE\n\
J = 5\n\
ENDIF\n\
ENDIF\n\
J\
";

        Variable result = rdEvaluater.Evaluate(pCode, &executionContext, &functionCaller, &parseErrors, 99);

        Assert::AreEqual(expectedValue, result.GetValueInt());
    }

    static void TestNestedIf()
    {
       DoTestNestedIf(1, 1, 2);
       DoTestNestedIf(1, 2, 3);
       DoTestNestedIf(4, 1, 4);
       DoTestNestedIf(4, 5, 5);
    }

    static void TestMissingEndif()
    {
        ExecutionContext executionContext;
        RDEvaluater rdEvaluater;
        ParseErrors parseErrors;
        FunctionCallerSimulator functionCaller(executionContext.Variables(), executionContext.GetStack());

        const char* pCode = " \
IF (V == 1) \n\
J = 2\
";

        Variable result = rdEvaluater.Evaluate(pCode, &executionContext, &functionCaller, &parseErrors, 99);

        Assert::AreEqual(1, parseErrors.GetErrorCount());
        Assert::AreEqual("Missing ENDIF", parseErrors.GetError(0)->_errorText);
    }


    static void TestFor()
    {
        ExecutionContext executionContext;
        RDEvaluater rdEvaluater;
        ParseErrors parseErrors;
        FunctionCallerSimulator functionCaller(executionContext.Variables(), executionContext.GetStack());

        const char* pCode = " \
FOR loop 0:3\n\
Q = loop\n\
ENDFOR\n\
Q\
";


        Variable result = rdEvaluater.Evaluate(pCode, &executionContext, &functionCaller, &parseErrors, 99);

        Assert::AreEqual(0, parseErrors.GetErrorCount());
        Assert::AreEqual(3, result.GetValueInt());
    }

    static void TestForDown()
    {
        ExecutionContext executionContext;
        RDEvaluater rdEvaluater;
        ParseErrors parseErrors;
        FunctionCallerSimulator functionCaller(executionContext.Variables(), executionContext.GetStack());

        const char* pCode = " \
FOR loop 0:3:-1\n\
Q = loop\n\
ENDFOR\n\
Q\
";


        Variable result = rdEvaluater.Evaluate(pCode, &executionContext, &functionCaller, &parseErrors, 99);

        Assert::AreEqual(0, parseErrors.GetErrorCount());
        Assert::AreEqual(0, result.GetValueInt());
    }


    static void TestMissingEndfor()
    {
        ExecutionContext executionContext;
        RDEvaluater rdEvaluater;
        ParseErrors parseErrors;
        FunctionCallerSimulator functionCaller(executionContext.Variables(), executionContext.GetStack());

        const char* pCode = " \
FOR loop 0:3\n\
Q = loop\
";


        Variable result = rdEvaluater.Evaluate(pCode, &executionContext, &functionCaller, &parseErrors, 99);

        Assert::AreEqual(1, parseErrors.GetErrorCount());
        Assert::AreEqual("Missing ENDFOR", parseErrors.GetError(0)->_errorText);
    }



    static void TestFunctionDefinition()
    {
        ExecutionContext executionContext;
        RDEvaluater rdEvaluater;
        ParseErrors parseErrors;
        FunctionCallerSimulator functionCaller(executionContext.Variables(), executionContext.GetStack());

        const char* pCode = " \
Q = 12\n\
FUNC MyFunc\n\
Q = loop\n\
ENDFUNC\n\
FUNC MyFunc2\n\
Q = loop\n\
ENDFUNC\n\
Q\
";

        Variable result = rdEvaluater.Evaluate(pCode, &executionContext, &functionCaller, &parseErrors, 99);

        Assert::AreEqual(0, parseErrors.GetErrorCount());
        Assert::AreEqual(5, executionContext._functionStore.Lookup("MyFunc")->LineNumberStart);
        Assert::AreEqual(14, executionContext._functionStore.Lookup("MyFunc2")->LineNumberStart);
    }

    static void TestMissingEndfunc()
    {
        ExecutionContext executionContext;
        RDEvaluater rdEvaluater;
        ParseErrors parseErrors;
        FunctionCallerSimulator functionCaller(executionContext.Variables(), executionContext.GetStack());

        const char* pCode = " \
Q = 12\n\
FUNC MyFunc\n\
Q = loop\
";

        Variable result = rdEvaluater.Evaluate(pCode, &executionContext, &functionCaller, &parseErrors, 99);

        Assert::AreEqual(1, parseErrors.GetErrorCount());
        Assert::AreEqual("Missing ENDFUNC for function: MyFunc", parseErrors.GetError(0)->_errorText);
    }


public:
	static void Run()
	{
        TestIf();
        TestNestedIf();
        TestFor();
        TestForDown();
        TestFunctionDefinition();
        TestMissingEndfunc();
        TestMissingEndif();
        TestMissingEndfor();

		TestEmptyString();

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
