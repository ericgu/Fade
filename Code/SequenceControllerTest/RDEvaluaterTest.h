
class MockExecutionFlow : public IExecutionFlow
{
public:
    CommandResult _commandResult;
    Command _command;
    bool _aborting = false;
    bool _buttonState = false;
    bool _breaking = false;

    int _ledGroupNumber;
    const char* _pLedType;
    int _ledCount;
    int _pin1;
    int _pin2;
    int _pin3;
    int _pin4;

    int _buttonNumber;
    const char* _pButtonType;
    int _pinNumber;
    int _parameter1; 

    MockExecutionFlow() : _commandResult(16) {}

    virtual CommandResultStatus RunProgram(int runCount = -1)
    {
        return CommandResultStatus::CommandNone;
    }

    virtual	void ExecuteLedCommand(CommandResult* pCommandResult)
    {}

    virtual CommandResult* GetCommandResult()
    {
        return &_commandResult;
    }

    virtual Command* GetCommand(int commandNumber)
    {
        return &_command;
    }

    virtual void AbortExecution()
    {
        _aborting = true;
    }

    virtual bool IsAborting()
    {
        return _aborting;
    }

    virtual void BreakExecution()
    {
        _breaking = true;
    }

    virtual bool IsBreaking()
    {
        return _breaking;
    }

    virtual void ClearBreak()
    {
        _breaking = false;
    }

    virtual void ConfigureLeds(int ledGroupNumber, const char* pLedType, int ledCount, int pin1, int pin2, int pin3, int pin4)
    {
        _ledGroupNumber = ledGroupNumber;
        _pLedType = pLedType;
        _ledCount = ledCount;
        _pin1 = pin1;
        _pin2 = pin2;
        _pin3 = pin3;
        _pin4 = pin4;
    }

    virtual void ConfigureButton(int buttonNumber, const char* pButtonType, int pinNumber, int parameter1)
    {
        _buttonNumber = buttonNumber;
        _pButtonType = pButtonType;
        _pinNumber = pinNumber;
        _parameter1 = parameter1;
    }


    void SetButtonState(bool state)
    {
        _buttonState = state;
    }

    virtual bool GetButtonState(int buttonNumber)
    {
        return _buttonState;
    }

    virtual int GetButtonCount()
    {
        return 1;
    }

};

class StatementTester
{
    char _program[2048];

public:
    ExecutionContext _executionContext;
    RDEvaluater _rdEvaluater;
    ParseErrors _parseErrors;
    //FunctionCallerSimulator _functionCaller;
    MockExecutionFlow _executionFlow;

    StatementTester() //:
        // _functionCaller(_executionContext.Variables(), _executionContext.GetStack())
    {
        _program[0] = '\0';
    }


    void Add(const char* pLine)
    {
        if (strlen(_program) != 0)
        {
            SafeString::StringCat(_program, "\n", sizeof(_program));
        }
        SafeString::StringCat(_program, pLine, sizeof(_program));
    }

    Variable Execute()
    {
        return _rdEvaluater.Evaluate(_program, &_executionContext, &_parseErrors, &_executionFlow);
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

    static void TestMultiValueArgumentUndefined()
    {
        TestUndefinedVariableInOperation("{x, 1, 3}", "Undefined variable: x");
        TestUndefinedVariableInOperation("{1, 2, 3, ff}", "Undefined variable: ff");
    }

	static void TestUnaryMinus()
	{
		TestEvaluation("-369", -369.0F);
	}

	static void TestUnaryPlus()
	{
		TestEvaluation("+15", 15.0F);
	}

    static void TestUnaryUndefined()
    {
        TestUndefinedVariableInOperation("-x", "Undefined variable: x");
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

    static void TestMultiplicativeUndefined()
    {
        TestUndefinedVariableInOperation("x * 3", "Undefined variable: x");
        TestUndefinedVariableInOperation("3 % a", "Undefined variable: a");
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

    static void TestAdditiveUndefined()
    {
        TestUndefinedVariableInOperation("x + 3", "Undefined variable: x");
        TestUndefinedVariableInOperation("3 - a", "Undefined variable: a");
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

    static void TestRelationalUndefined()
    {
        TestUndefinedVariableInOperation("x < 3", "Undefined variable: x");
        TestUndefinedVariableInOperation("3 > a", "Undefined variable: a");
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

    static void TestEqualityUndefined()
    {
        TestUndefinedVariableInOperation("x == 3", "Undefined variable: x");
        TestUndefinedVariableInOperation("3 != a", "Undefined variable: a");
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

    static void TestLogicalUndefined()
    {
        TestUndefinedVariableInOperation("x && 3", "Undefined variable: x");
        TestUndefinedVariableInOperation("3 && a", "Undefined variable: a");
        TestUndefinedVariableInOperation("x || 3", "Undefined variable: x");
        TestUndefinedVariableInOperation("3 || a", "Undefined variable: a");
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

    static void TestAssignmentUndefined()
    {
        TestUndefinedVariableInOperation("x = 5\nx = y", "Undefined variable: y", 1);
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

		Variable result = rdEvaluater.Evaluate("\n15 + ", 0, &parseErrors);

		ValidateParseErrors(&parseErrors, "Missing value at end of expression", 1);
	}

	static void TestAddWithWrongArgument()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("\n15 + +", 0, &parseErrors);

		ValidateParseErrors(&parseErrors, "Missing value at end of expression", 1);
	}

	static void TestUnknownCharacter()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("\n15 $ 12", 0, &parseErrors);

		ValidateParseErrors(&parseErrors, "Missing value at end of expression", 1);
	}

	static void TestUnexpectedValueAfterParsing()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("\n15 12", 0, &parseErrors);

		ValidateParseErrors(&parseErrors, "Unexpected token remaining after parsing: 12", 1);
	}

	static void TestMissingClosingParen()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("\n(15 * 12", 0, &parseErrors);

		ValidateParseErrors(&parseErrors, "Missing ) in expression", 1);
	}

	static void TestMissingClosingBrace()
	{
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("\n{15", 0, &parseErrors);

		ValidateParseErrors(&parseErrors, "Missing } in expression", 1);
	}

	static void TestUndeclaredVariable()
	{
		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("\nMyFirstValue", &executionContext, &parseErrors);

		ValidateParseErrors(&parseErrors, "Undefined variable: MyFirstValue", -1);
	}

	static void TestUndeclaredVariable2()
	{
		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;
		executionContext.AddVariableAndSet("MyFirstValue", &Variable(22));

		Variable result = rdEvaluater.Evaluate("\n13 * (2 + MyFirstValue) * MyMissingValue * 33", &executionContext, &parseErrors);

		ValidateParseErrors(&parseErrors, "Undefined variable: MyMissingValue", 1);
	}

	static void TestFunctionCallNoParametersMissingClosingParen()
	{
		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("\nMyFunction(", &executionContext, &parseErrors);

		ValidateParseErrors(&parseErrors, "Missing closing ) in expression", 1);
	}

	static void TestFunctionCallOneParametersMissingClosingParen()
	{
		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("\nMyFunction(15", &executionContext, &parseErrors);

		ValidateParseErrors(&parseErrors, "Missing closing ) in expression", 1);
	}

	static void TestFunctionCallTwoParametersEndsAfterComma()
	{
		ExecutionContext executionContext;
		RDEvaluater rdEvaluater;
		ParseErrors parseErrors;

		Variable result = rdEvaluater.Evaluate("\nMyFunction(15,", &executionContext, &parseErrors);

		ValidateParseErrors(&parseErrors, "Missing closing ) in expression", 1);
	}

 

    static void DoTestIf(int conditionValue, int expectedValue)
    {
        StatementTester statementTester;

        statementTester.Add("IF (V == 1)");
        statementTester.Add("J = 2");
        statementTester.Add("ELSEIF (V == 2)");
        statementTester.Add("J = 3");
        statementTester.Add("ELSEIF (V == 3)");
        statementTester.Add("J = 4");
        statementTester.Add("ELSE");
        statementTester.Add("J = 5");
        statementTester.Add("ENDIF");
        statementTester.Add("J");

        statementTester._executionContext.AddVariableAndSet("V", &Variable(conditionValue), 1);

        Variable result = statementTester.Execute();

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
        StatementTester statementTester;

        statementTester.Add("IF (V == 1)");
        statementTester.Add("IF(X == 1)");
        statementTester.Add("J = 2");
        statementTester.Add("ELSE");
        statementTester.Add("J = 3");
        statementTester.Add("ENDIF");
        statementTester.Add("ELSE");
        statementTester.Add("IF(X == 1)");
        statementTester.Add("J = 4");
        statementTester.Add("ELSE");
        statementTester.Add("J = 5");
        statementTester.Add("ENDIF");
        statementTester.Add("ENDIF");
        statementTester.Add("J");

        statementTester._executionContext.AddVariableAndSet("V", &Variable(conditionValue1));
        statementTester._executionContext.AddVariableAndSet("X", &Variable(conditionValue2));

        Variable result = statementTester.Execute();

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
        StatementTester statementTester;

        statementTester.Add("q = 3");
        statementTester.Add("IF (q == 1)");
        statementTester.Add("J = 2");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Missing ENDIF", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual(2, statementTester._parseErrors.GetError(0)->_lineNumber);
    }


    static void TestFor()
    {
        StatementTester statementTester;

        statementTester.Add("FOR loop 0:3");
        statementTester.Add("Q = loop");
        statementTester.Add("ENDFOR");
        statementTester.Add("Q");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual(3, result.GetValueInt());
    }

    static void TestForDown()
    {
        StatementTester statementTester;

        statementTester.Add("FOR loop 0:4:-1");
        statementTester.Add("Q = loop");
        statementTester.Add("ENDFOR");
        statementTester.Add("Q");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual(0, result.GetValueInt());
    }


    static void TestMissingEndfor()
    {
        StatementTester statementTester;

        statementTester.Add("FOR loop 0:3");
        statementTester.Add("Q = loop");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Missing ENDFOR", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual(1, statementTester._parseErrors.GetError(0)->_lineNumber);
    }

    static void TestFunctionDefinition()
    {
        StatementTester statementTester;

        statementTester.Add("Q = 12");
        statementTester.Add("FUNC MyFunc");
        statementTester.Add("Q = loop");
        statementTester.Add("ENDFUNC");
        statementTester.Add("FUNC MyFunc2");
        statementTester.Add("Q = loop");
        statementTester.Add("ENDFUNC");
        statementTester.Add("Q");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual(7, statementTester._executionContext._functionStore.Lookup("MyFunc")->LineNumberStart);
        Assert::AreEqual(36, statementTester._executionContext._functionStore.Lookup("MyFunc2")->LineNumberStart);
    }

    static void TestFunctionCallUndefinedVariable()
    {
        StatementTester statementTester;

        statementTester.Add("Q = 12");
        statementTester.Add("FUNC MyFunc(X)");
        statementTester.Add("ENDFUNC");
        statementTester.Add("MyFunc(z)");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Undefined variable: z", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual(3, statementTester._parseErrors.GetError(0)->_lineNumber);
    }



    static void TestMissingEndfunc()
    {
        StatementTester statementTester;

        statementTester.Add("Q = 12");
        statementTester.Add("FUNC MyFunc");
        statementTester.Add("Q = loop");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Missing ENDFUNC for function: MyFunc", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual(2, statementTester._parseErrors.GetError(0)->_lineNumber);
    }

    static void TestMissingEndfunc2()
    {
        StatementTester statementTester;

        statementTester.Add("Q = 12");
        statementTester.Add("FUNC MyFunc");
        statementTester.Add("Q = loop");
        statementTester.Add("FUNC M2");
        statementTester.Add("ENDFUNC");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Missing ENDFUNC for function: MyFunc", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual(3, statementTester._parseErrors.GetError(0)->_lineNumber);
    }


    static void TestReturn()
    {
        StatementTester statementTester;

        statementTester.Add("RETURN 1");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());

        Assert::AreEqual(1, statementTester._executionContext.GetVariableWithoutErrorCheck("<ReturnValue>")->GetValueInt());
    }

    static void TestBlankLines()
    {
        StatementTester statementTester;

        statementTester.Add("");
        statementTester.Add("FOR loop 0:3");
        statementTester.Add("");
        statementTester.Add("Q = loop");
        statementTester.Add("");
        statementTester.Add("ENDFOR");
        statementTester.Add("");
        statementTester.Add("Q");
        statementTester.Add("");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual(3, result.GetValueInt());
    }



    static void TestFunctionCallNoParams()
    {
        StatementTester statementTester;

        statementTester.Add("FUNC Test()");
        statementTester.Add("RETURN 55");
        statementTester.Add("ENDFUNC");
        statementTester.Add("Test()");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());

        Assert::AreEqual(55, result.GetValueInt());
    }

    static void TestFunctionCallOneParam()
    {
        StatementTester statementTester;

        statementTester.Add("FUNC Test(q)");
        statementTester.Add("RETURN 5 * q");
        statementTester.Add("ENDFUNC");
        statementTester.Add("Test(3)");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());

        Assert::AreEqual(15, result.GetValueInt());
    }

    static void TestFunctionCallFuncAsParam()
    {
        StatementTester statementTester;

        statementTester.Add("FUNC Test(q)");
        statementTester.Add("RETURN 5 * q");
        statementTester.Add("ENDFUNC");
        statementTester.Add("FUNC Param()");
        statementTester.Add("RETURN 3");
        statementTester.Add("ENDFUNC");
        statementTester.Add("Test(Param())");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());

        Assert::AreEqual(15, result.GetValueInt());
    }

    static void TestFunctionCallMultiValueReturn()
    {
        StatementTester statementTester;

        statementTester.Add("RETURN {1, 2, 3}");
        statementTester.Add("FUNC Function()");
        statementTester.Add("RETURN {1, 2, 3}");
        statementTester.Add("ENDFUNC");
        statementTester.Add("A=Function()");
        statementTester.Add("A");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());

        Assert::AreEqual(1.0F, result.GetValueFloat(0));
        Assert::AreEqual(2.0F, result.GetValueFloat(1));
        Assert::AreEqual(3.0F, result.GetValueFloat(2));
    }

    static void TestFunctionCallNested()
    {
        StatementTester statementTester;

        statementTester.Add("FUNC Function()");
        statementTester.Add("RETURN 11.0");
        statementTester.Add("ENDFUNC");
        statementTester.Add("FUNC FunctionOuter()");
        statementTester.Add("A=Function()");
        statementTester.Add("RETURN A");
        statementTester.Add("ENDFUNC");
        statementTester.Add("B=FunctionOuter()");
        statementTester.Add("B");

        Variable result = statementTester.Execute();

        Assert::AreEqual(11.0F, result.GetValueFloat(0));
    }


    static void TestFunctionCallAsArgument()
    {
        StatementTester statementTester;

        statementTester.Add("FUNC Function(A, B)");
        statementTester.Add("RETURN A");
        statementTester.Add("ENDFUNC");
        statementTester.Add("FUNC Function2(X, Y, Z)");
        statementTester.Add("RETURN X + Y + Z");
        statementTester.Add("ENDFUNC");
        statementTester.Add("B=Function2(3, Function(1, 3), 6)");
        statementTester.Add("B");

        Variable result = statementTester.Execute();

        Assert::AreEqual(10.0F, result.GetValueFloat(0));
    }

    static void TestMethodCallWithParameterDuplicateName()
    {
        StatementTester statementTester;

        statementTester.Add("V=15.0");
        statementTester.Add("FUNC Function(V)");
        statementTester.Add("RETURN V");
        statementTester.Add("ENDFUNC");
        statementTester.Add("Function(13)");

        Variable result = statementTester.Execute();

        Assert::AreEqual(13.0F, result.GetValueFloat(0));
    }

    static void TestMethodCannotAccessParentVariables()
    {
        StatementTester statementTester;

        statementTester.Add("V=15.0");
        statementTester.Add("FUNC Function()");
        statementTester.Add("RETURN V");
        statementTester.Add("ENDFUNC");
        statementTester.Add("Function()");

        Variable result = statementTester.Execute();

        Assert::AreEqual(2, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Missing value in RETURN statement", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual(2, statementTester._parseErrors.GetError(0)->_lineNumber);

    }

    static void TestMethodCallWithWrongArgumentCount()
    {
        StatementTester statementTester;

        statementTester.Add("FUNC Function(X)");
        statementTester.Add("PL(X)");
        statementTester.Add("ENDFUNC");
        statementTester.Add("Function()");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Missing argument in function call for parameter: X", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual(0, statementTester._parseErrors.GetError(0)->_lineNumber);

    }


    static void TestMethodCallWithWrongArgumentCount2()
    {
        StatementTester statementTester;

        statementTester.Add("FUNC Function(X)");
        statementTester.Add("PL(X)");
        statementTester.Add("ENDFUNC");
        statementTester.Add("Function(15.0, 35.0)");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Extra arguments passed to function Function", statementTester._parseErrors.GetError(0)->_errorText);
    }

    static void TestUndefinedFunctionWithVariableParameter()
    {
        StatementTester statementTester;

        statementTester.Add("V=15.0");
        statementTester.Add("XX(V)");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Unrecognized function: XX", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual(1, statementTester._parseErrors.GetError(0)->_lineNumber);
    }

    static void TestRealCode()
    {
        //return;

        StatementTester statementTester;

        statementTester.Add("CONFIGLED(\"RGB\", 33, 13)");
        statementTester.Add("FUNC AngleToRGB(angleInDegrees)");
        statementTester.Add("//PL(angleInDegrees)  ");
        statementTester.Add("");
        statementTester.Add("brightness = 0.2");
        statementTester.Add("IF(angleInDegrees <= 120)");
        statementTester.Add("temp = angleInDegrees / 120 * brightness");
        statementTester.Add("value = { temp, 0, brightness - temp }");
        statementTester.Add("ELSEIF(angleInDegrees <= 240)");
        statementTester.Add("temp = (angleInDegrees - 120) / 120 * brightness");
        statementTester.Add("value = { brightness - temp, temp, 0 }");
        statementTester.Add("ELSE");
        statementTester.Add("temp = (angleInDegrees - 240) / 120 * brightness");
        statementTester.Add("value = { 0, brightness - temp, temp }");
        statementTester.Add("ENDIF");
        statementTester.Add("RETURN value");
        statementTester.Add("ENDFUNC");
        statementTester.Add("");
        statementTester.Add("FUNC Test()");
        statementTester.Add("ENDFUNC");

        statementTester.Add("FUNC DoChunk(chunk, offset, angle)");
        statementTester.Add("//PL(\"DoChunk\")");
        statementTester.Add("rgb = AngleToRGB((angle + offset) % 360)");
        statementTester.Add("//PL(rgb)");
        statementTester.Add("start = chunk * 3");
        statementTester.Add("D(5, start, rgb)");
        statementTester.Add("D(5, start + 1, rgb)");
        statementTester.Add("D(5, start + 2, rgb)");
        statementTester.Add("");
        statementTester.Add("ENDFUNC");
        statementTester.Add("");
        statementTester.Add("FUNC Main()");
        statementTester.Add("");
        statementTester.Add("//PL(\"hello\")");
        statementTester.Add("FOR angle 0:359 : 5");
        statementTester.Add("//PL(angle)");
        statementTester.Add("DoChunk(0, 0, angle)");
        statementTester.Add("DoChunk(1, 72, angle)");
        statementTester.Add("DoChunk(2, 144, angle)");
        statementTester.Add("DoChunk(3, 216, angle)");
        statementTester.Add("DoChunk(4, 288, angle)");
        statementTester.Add("DoChunk(5, 252, angle)");
        statementTester.Add("DoChunk(6, 180, angle)");
        statementTester.Add("DoChunk(7, 108, angle)");
        statementTester.Add("DoChunk(8, 36, angle) ");
        statementTester.Add("DoChunk(9, 324, angle)");
        statementTester.Add("A(5)");
        statementTester.Add("ENDFOR");
        statementTester.Add("ENDFUNC");
        statementTester.Add("");
        statementTester.Add("FOR count 0:1");
        statementTester.Add("P(\".\")");
        statementTester.Add("Main()");
        statementTester.Add("ENDFOR");

        for (int i = 0; i < 20; i++)
        {
            Variable result = statementTester.Execute();
        }

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
    }

    static void TestAbort()
    {
        StatementTester statementTester;

        statementTester.Add("ABORT()");

        Variable result = statementTester.Execute();

        Assert::AreEqual(true, statementTester._executionFlow.IsAborting());
        Assert::AreEqual(2, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Aborting: ABORT", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual("Aborting: STATEMENT", statementTester._parseErrors.GetError(1)->_errorText);
    }


    static void TestAbortInForLoop()
    {
        StatementTester statementTester;

        statementTester.Add("FOR Test 0:2");
        statementTester.Add("ABORT()");
        statementTester.Add("x = 5");
        statementTester.Add("ENDFOR");

        Variable result = statementTester.Execute();

        Assert::AreEqual(true, statementTester._executionFlow.IsAborting());
        Assert::AreEqual(3, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Aborting: ABORT", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual("Aborting: FOR", statementTester._parseErrors.GetError(1)->_errorText);
        Assert::AreEqual("Aborting: STATEMENT", statementTester._parseErrors.GetError(2)->_errorText);
    }

    static void TestAbortInStatementList()
    {
        StatementTester statementTester;

        statementTester.Add("ABORT()");
        statementTester.Add("junk1");
        statementTester.Add("junk2");
        statementTester.Add("junk3");
        statementTester.Add("junk4");

        Variable result = statementTester.Execute();

        Assert::AreEqual(true, statementTester._executionFlow.IsAborting());
        Assert::AreEqual(2, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Aborting: ABORT", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual("Aborting: STATEMENT", statementTester._parseErrors.GetError(1)->_errorText);
    }

    static void TestButtonReadTrue()
    {
        ExecutionContext _executionContext;
        RDEvaluater _rdEvaluater;
        ParseErrors _parseErrors;
        MockExecutionFlow _executionFlow;
        _executionFlow.SetButtonState(false);

        Variable result = _rdEvaluater.Evaluate("READBUTTON(0)", &_executionContext, &_parseErrors, &_executionFlow);

        Assert::AreEqual(0, result.GetValueInt());
    }

    static void TestButtonReadFalse()
    {
        ExecutionContext _executionContext;
        RDEvaluater _rdEvaluater;
        ParseErrors _parseErrors;
        MockExecutionFlow _executionFlow;
        _executionFlow.SetButtonState(true);

        Variable result = _rdEvaluater.Evaluate("READBUTTON(0)", &_executionContext, &_parseErrors, &_executionFlow);

        Assert::AreEqual(1, result.GetValueInt());
    }

    static void TestButtonReadNegativeButtonNumberGivesError()
    {
        ExecutionContext _executionContext;
        RDEvaluater _rdEvaluater;
        ParseErrors _parseErrors;
        MockExecutionFlow _executionFlow;
        _executionFlow.SetButtonState(true);

        Variable result = _rdEvaluater.Evaluate("READBUTTON(-1)", &_executionContext, &_parseErrors, &_executionFlow);

        Assert::AreEqual(1, _parseErrors.GetErrorCount());
        Assert::AreEqual("Invalid Button Number: button numbers must positive", _parseErrors.GetError(0)->_errorText);
    }

    static void TestButtonReadOutOfRangeButtonNumberGivesError()
    {
        ExecutionContext _executionContext;
        RDEvaluater _rdEvaluater;
        ParseErrors _parseErrors;
        MockExecutionFlow _executionFlow;
        _executionFlow.SetButtonState(true);

        Variable result = _rdEvaluater.Evaluate("READBUTTON(1)", &_executionContext, &_parseErrors, &_executionFlow);

        Assert::AreEqual(1, _parseErrors.GetErrorCount());
        Assert::AreEqual("Invalid Button Number: button number is not defined", _parseErrors.GetError(0)->_errorText);
    }

    static void TestLoopVariableAfterForLoop()
    {
        StatementTester statementTester;

        statementTester.Add("SavedValue = 0");
        statementTester.Add("FOR Test 4:10");
        statementTester.Add("SavedValue = Test");
        statementTester.Add("ENDFOR");
        statementTester.Add("SavedValue");

        Variable result = statementTester.Execute();

        Assert::AreEqual(10, result.GetValueInt());
    }

    static void TestBreakInForLoop()
    {
        StatementTester statementTester;

        statementTester.Add("SavedValue = 0");
        statementTester.Add("FOR Test 4:10");
        statementTester.Add("SavedValue = Test");
        statementTester.Add("BREAK");
        statementTester.Add("ENDFOR");
        statementTester.Add("SavedValue");

        Variable result = statementTester.Execute();

        Assert::AreEqual(4, result.GetValueInt());
    }

    static void TestBreakInForLoopWithIf()
    {
        StatementTester statementTester;

        statementTester.Add("SavedValue = 0");
        statementTester.Add("FOR Test 4:10");
        statementTester.Add("SavedValue = Test");
        statementTester.Add("IF Test == 6");
        statementTester.Add("BREAK");
        statementTester.Add("ENDIF");
        statementTester.Add("ENDFOR");
        statementTester.Add("SavedValue");

        Variable result = statementTester.Execute();

        Assert::AreEqual(6, result.GetValueInt());
    }


    static void TestUndefinedVariableInOperation(const char* pExpression, const char* pExpectedError, int lineNumber = 0)
    {
        StatementTester statementTester;

        statementTester.Add(pExpression);

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual(pExpectedError, statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual(lineNumber, statementTester._parseErrors.GetError(0)->_lineNumber);
    }

    static void TestRelationalArgumentUndefined()
    {
        TestUndefinedVariableInOperation("y > 15", "Undefined variable: y");
        TestUndefinedVariableInOperation("15 > q", "Undefined variable: q");
    }


    static void TestArrayIndexing()
    {
      StatementTester statementTester;

      statementTester.Add("values = {155}");
      statementTester.Add("values[0]");

      Variable result = statementTester.Execute();

      Assert::AreEqual(155, result.GetValueInt());
    }

    static void TestArrayIndexing2()
    {
      StatementTester statementTester;

      statementTester.Add("values = {155, 25, 55}");
      statementTester.Add("values[1]");

      Variable result = statementTester.Execute();

      Assert::AreEqual(25, result.GetValueInt());
    }

public:
	static void Run()
	{
    TestArrayIndexing();
    TestArrayIndexing2();

        TestIf();
        TestNestedIf();
        TestFor();
        TestForDown();
        TestFunctionDefinition();
        TestMissingEndfunc();
        TestMissingEndfunc2();
        TestMissingEndif();
        TestMissingEndfor();
        TestReturn();

        TestFunctionCallNoParams();
        TestFunctionCallOneParam();
        TestFunctionCallFuncAsParam();
        TestFunctionCallMultiValueReturn();
        TestFunctionCallNested();
        TestFunctionCallAsArgument();
        TestMethodCallWithParameterDuplicateName();
        TestMethodCannotAccessParentVariables();
        TestMethodCallWithWrongArgumentCount();
        TestMethodCallWithWrongArgumentCount2();
        TestUndefinedFunctionWithVariableParameter();

        //TestRealCode();

		TestEmptyString();

		TestPrimitiveNumber();

		TestMultiValueSingle();
		TestMultiValueNumber();
		TestMultiValueExpression();
        TestMultiValueArgumentUndefined();

		TestUnaryMinus();
		TestUnaryPlus();
        TestUnaryUndefined();

		TestSingleMultiplication();
		TestSingleDivision();
		TestSingleModulus();
		TestMultiplyDivide();
		TestTripleMultiply();
        TestMultiplicativeUndefined();

		TestSingleAddition();
		TestSingleSubtraction();
		TestAddThenSubtract();
        TestAdditiveUndefined();

		TestMultiplyAdd();

		TestLessThan();
		TestLessThanFails();
		TestLessThanOrEqual();
		TestLessThanOrEqualFails();
		TestGreaterThan();
		TestGreaterThanFails();
		TestGreatherThanOrEqual();
		TestGreatherThanOrEqualFails();
        TestRelationalArgumentUndefined();
        TestRelationalUndefined();


		TestEqualityEqual();
		TestEqualityNotEqual();
		TestNonEqualityEqual();
		TestNonEqualityNotEqual();
        TestEqualityUndefined();

		TestLogicalAndNeither();
		TestLogicalAndFirst();
		TestLogicalAndSecond();
		TestLogicalAndBoth();

		TestLogicalOrNeither();
		TestLogicalOrFirst();
		TestLogicalOrSecond();
		TestLogicalOrBoth();
        TestLogicalUndefined();

		TestMultiplyAddWithParens();
		TestNestedParens();

		TestVariableReference();
		TestVariableReferenceInExpressions();
		TestVariableIncrement();
		TestVariableDecrement();

		TestAssignment();
		TestAssignmentMultiValue();
        TestAssignmentUndefined();

		TestPrimitiveString();

        TestBlankLines();

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
        TestFunctionCallUndefinedVariable();

        TestAbort();
        TestAbortInForLoop();
        TestAbortInStatementList();

        TestButtonReadFalse();
        TestButtonReadTrue();
        TestButtonReadNegativeButtonNumberGivesError();
        TestButtonReadOutOfRangeButtonNumberGivesError();

        TestLoopVariableAfterForLoop();
        TestBreakInForLoop();
        TestBreakInForLoopWithIf();
    }
};
