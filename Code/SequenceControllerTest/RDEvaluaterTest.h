
class MockExecutionFlow : public IExecutionFlow
{
public:
    CommandResult _commandResult;
    Command _command;
    bool _aborting = false;
    bool _buttonState = false;
    bool _breaking = false;
    bool _configLedsReturnValue = true;

    int _ledGroupNumber;
    const char* _pLedType;
    int _ledCount;

    int _pinCount;
    int _pins[16];

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

    virtual bool ConfigureLeds(int ledGroupNumber, const char* pLedType, int ledCount, int pinCount, int pins[16])
    {
        _ledGroupNumber = ledGroupNumber;
        _pLedType = pLedType;
        _ledCount = ledCount;

        _pinCount = pinCount;
        for (int i = 0; i < pinCount; i++)
        {
            _pins[i] = pins[i];
        }

        return _configLedsReturnValue;
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

    static void TestAssignmentToExpression()
    {
        // TODO: Add in correct stack level checking..

        ExecutionContext executionContext;
        RDEvaluater rdEvaluater;
        ParseErrors parseErrors;

        Variable result = rdEvaluater.Evaluate("5 * AssignedValue = 15", &executionContext, &parseErrors);

        Assert::AreEqual(1, parseErrors.GetErrorCount());
        Assert::AreEqual("Undefined variable: AssignedValue", parseErrors.GetError(0)->_errorText);
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

        statementTester.Add("if (V == 1)");
        statementTester.Add("J = 2");
        statementTester.Add("elseif (V == 2)");
        statementTester.Add("J = 3");
        statementTester.Add("elseif (V == 3)");
        statementTester.Add("J = 4");
        statementTester.Add("else");
        statementTester.Add("J = 5");
        statementTester.Add("endif");
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

        statementTester.Add("if (V == 1)");
        statementTester.Add("if(X == 1)");
        statementTester.Add("J = 2");
        statementTester.Add("else");
        statementTester.Add("J = 3");
        statementTester.Add("endif");
        statementTester.Add("else");
        statementTester.Add("if(X == 1)");
        statementTester.Add("J = 4");
        statementTester.Add("else");
        statementTester.Add("J = 5");
        statementTester.Add("endif");
        statementTester.Add("endif");
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
        statementTester.Add("if (q == 1)");
        statementTester.Add("J = 2");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Missing endif", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual(2, statementTester._parseErrors.GetError(0)->_lineNumber);
    }

    static void TestIfWithAssignment()
    {
        StatementTester statementTester;

        statementTester.Add("q = 3");
        statementTester.Add("J = 2");
        statementTester.Add("if (q = 1)");
        statementTester.Add("J = 2");
        statementTester.Add("endif");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("if statement did not specify a condition. Did you mean == instead of =?", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual(2, statementTester._parseErrors.GetError(0)->_lineNumber);
    }


    static void TestFor()
    {
        StatementTester statementTester;

        statementTester.Add("for loop 0:3");
        statementTester.Add("Q = loop");
        statementTester.Add("endfor");
        statementTester.Add("Q");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual(3, result.GetValueInt());
    }

    static void TestWhile()
    {
        StatementTester statementTester;

        statementTester.Add("count = 0");
        statementTester.Add("sum = 0");
        statementTester.Add("while count < 10");
        statementTester.Add("sum = sum + count");
        statementTester.Add("count++");
        statementTester.Add("endwhile");
        statementTester.Add("sum");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual(45, result.GetValueInt());
    }

    static void TestWhileNoExecute()
    {
        StatementTester statementTester;

        statementTester.Add("count = 0");
        statementTester.Add("sum = 12");
        statementTester.Add("while count > 10");
        statementTester.Add("sum = 128");
        statementTester.Add("endwhile");
        statementTester.Add("sum");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual(12, result.GetValueInt());
    }



    static void TestForDown()
    {
        StatementTester statementTester;

        statementTester.Add("for loop 0:4:-1");
        statementTester.Add("Q = loop");
        statementTester.Add("endfor");
        statementTester.Add("Q");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual(0, result.GetValueInt());
    }


    static void TestMissingEndfor()
    {
        StatementTester statementTester;

        statementTester.Add("for loop 0:3");
        statementTester.Add("Q = loop");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Missing endfor", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual(1, statementTester._parseErrors.GetError(0)->_lineNumber);
    }

    static void TestFunctionDefinition()
    {
      StatementTester statementTester;

      statementTester.Add("Q = 12");
      statementTester.Add("func MyFunc");
      statementTester.Add("Q = loop");
      statementTester.Add("endfunc");
      statementTester.Add("func MyFunc2");
      statementTester.Add("Q = loop");
      statementTester.Add("endfunc");
      statementTester.Add("Q");

      Variable result = statementTester.Execute();

      Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
      Assert::AreEqual(7, statementTester._executionContext._functionStore.Lookup("MyFunc")->LineNumberStart);
      Assert::AreEqual(36, statementTester._executionContext._functionStore.Lookup("MyFunc2")->LineNumberStart);
    }

    static void TestFunctionDefinitionMissingClosingBrace()
    {
      StatementTester statementTester;

      statementTester.Add("q = 12");
      statementTester.Add("func MyFunc(param");
      statementTester.Add("endfunc");
      statementTester.Add("MyFunc(q)");

      Variable result = statementTester.Execute();

      Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
      Assert::AreEqual("Missing closing ) in function call MyFunc", statementTester._parseErrors.GetError(0)->_errorText);
    }

    static void TestFunctionCallUndefinedVariable()
    {
        StatementTester statementTester;

        statementTester.Add("Q = 12");
        statementTester.Add("func MyFunc(X)");
        statementTester.Add("endfunc");
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
        statementTester.Add("func MyFunc");
        statementTester.Add("Q = loop");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Missing endfunc for function: MyFunc", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual(2, statementTester._parseErrors.GetError(0)->_lineNumber);
    }

    static void TestMissingEndfunc2()
    {
        StatementTester statementTester;

        statementTester.Add("Q = 12");
        statementTester.Add("func MyFunc");
        statementTester.Add("Q = loop");
        statementTester.Add("func M2");
        statementTester.Add("endfunc");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Missing endfunc for function: MyFunc", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual(3, statementTester._parseErrors.GetError(0)->_lineNumber);
    }


    static void TestReturn()
    {
        StatementTester statementTester;

        statementTester.Add("return 1");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());

        Assert::AreEqual(1, statementTester._executionContext.GetVariableWithoutErrorCheck("<ReturnValue>")->GetValueInt());
    }

    static void TestBlankLines()
    {
        StatementTester statementTester;

        statementTester.Add("");
        statementTester.Add("for loop 0:3");
        statementTester.Add("");
        statementTester.Add("Q = loop");
        statementTester.Add("");
        statementTester.Add("endfor");
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

        statementTester.Add("func Test()");
        statementTester.Add("return 55");
        statementTester.Add("endfunc");
        statementTester.Add("Test()");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());

        Assert::AreEqual(55, result.GetValueInt());
    }

    static void TestFunctionCallOneParam()
    {
        StatementTester statementTester;

        statementTester.Add("func Test(q)");
        statementTester.Add("return 5 * q");
        statementTester.Add("endfunc");
        statementTester.Add("Test(3)");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());

        Assert::AreEqual(15, result.GetValueInt());
    }

    static void TestFunctionCallFuncAsParam()
    {
        StatementTester statementTester;

        statementTester.Add("func Test(q)");
        statementTester.Add("return 5 * q");
        statementTester.Add("endfunc");
        statementTester.Add("func Param()");
        statementTester.Add("return 3");
        statementTester.Add("endfunc");
        statementTester.Add("Test(Param())");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());

        Assert::AreEqual(15, result.GetValueInt());
    }

    static void TestFunctionCallMultiValueReturn()
    {
        StatementTester statementTester;

        statementTester.Add("return {1, 2, 3}");
        statementTester.Add("func Function()");
        statementTester.Add("return {1, 2, 3}");
        statementTester.Add("endfunc");
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

        statementTester.Add("func Function()");
        statementTester.Add("return 11.0");
        statementTester.Add("endfunc");
        statementTester.Add("func FunctionOuter()");
        statementTester.Add("A=Function()");
        statementTester.Add("return A");
        statementTester.Add("endfunc");
        statementTester.Add("B=FunctionOuter()");
        statementTester.Add("B");

        Variable result = statementTester.Execute();

        Assert::AreEqual(11.0F, result.GetValueFloat(0));
    }


    static void TestFunctionCallAsArgument()
    {
        StatementTester statementTester;

        statementTester.Add("func Function(A, B)");
        statementTester.Add("return A");
        statementTester.Add("endfunc");
        statementTester.Add("func Function2(X, Y, Z)");
        statementTester.Add("return X + Y + Z");
        statementTester.Add("endfunc");
        statementTester.Add("B=Function2(3, Function(1, 3), 6)");
        statementTester.Add("B");

        Variable result = statementTester.Execute();

        Assert::AreEqual(10.0F, result.GetValueFloat(0));
    }

    static void TestMethodCallWithParameterDuplicateName()
    {
        StatementTester statementTester;

        statementTester.Add("V=15.0");
        statementTester.Add("func Function(V)");
        statementTester.Add("return V");
        statementTester.Add("endfunc");
        statementTester.Add("Function(13)");

        Variable result = statementTester.Execute();

        Assert::AreEqual(13.0F, result.GetValueFloat(0));
    }

    static void TestMethodCannotAccessParentVariables()
    {
        StatementTester statementTester;

        statementTester.Add("V=15.0");
        statementTester.Add("func Function()");
        statementTester.Add("return V");
        statementTester.Add("endfunc");
        statementTester.Add("Function()");

        Variable result = statementTester.Execute();

        Assert::AreEqual(2, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Missing value in return statement", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual(2, statementTester._parseErrors.GetError(0)->_lineNumber);

    }

    static void TestMethodCallWithWrongArgumentCount()
    {
        StatementTester statementTester;

        statementTester.Add("func Function(X)");
        statementTester.Add("Pl(X)");
        statementTester.Add("endfunc");
        statementTester.Add("Function()");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Missing argument in function call for parameter: X", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual(0, statementTester._parseErrors.GetError(0)->_lineNumber);

    }


    static void TestMethodCallWithWrongArgumentCount2()
    {
        StatementTester statementTester;

        statementTester.Add("func Function(X)");
        statementTester.Add("Pl(X)");
        statementTester.Add("endfunc");
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

        statementTester.Add("ConfigLed(\"RGB\", 33, 13)");
        statementTester.Add("func AngleToRGB(angleInDegrees)");
        statementTester.Add("//Pl(angleInDegrees)  ");
        statementTester.Add("");
        statementTester.Add("brightness = 0.2");
        statementTester.Add("if(angleInDegrees <= 120)");
        statementTester.Add("temp = angleInDegrees / 120 * brightness");
        statementTester.Add("value = { temp, 0, brightness - temp }");
        statementTester.Add("elseif(angleInDegrees <= 240)");
        statementTester.Add("temp = (angleInDegrees - 120) / 120 * brightness");
        statementTester.Add("value = { brightness - temp, temp, 0 }");
        statementTester.Add("else");
        statementTester.Add("temp = (angleInDegrees - 240) / 120 * brightness");
        statementTester.Add("value = { 0, brightness - temp, temp }");
        statementTester.Add("endif");
        statementTester.Add("return value");
        statementTester.Add("endfunc");
        statementTester.Add("");
        statementTester.Add("func Test()");
        statementTester.Add("endfunc");

        statementTester.Add("func DoChunk(chunk, offset, angle)");
        statementTester.Add("//Pl(\"DoChunk\")");
        statementTester.Add("rgb = AngleToRGB((angle + offset) % 360)");
        statementTester.Add("//Pl(rgb)");
        statementTester.Add("start = chunk * 3");
        statementTester.Add("D(5, start, rgb)");
        statementTester.Add("D(5, start + 1, rgb)");
        statementTester.Add("D(5, start + 2, rgb)");
        statementTester.Add("");
        statementTester.Add("endfunc");
        statementTester.Add("");
        statementTester.Add("func Main()");
        statementTester.Add("");
        statementTester.Add("//Pl(\"hello\")");
        statementTester.Add("for angle 0:359 : 5");
        statementTester.Add("//Pl(angle)");
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
        statementTester.Add("endfor");
        statementTester.Add("endfunc");
        statementTester.Add("");
        statementTester.Add("for count 0:1");
        statementTester.Add("P(\".\")");
        statementTester.Add("Main()");
        statementTester.Add("endfor");

        for (int i = 0; i < 20; i++)
        {
            Variable result = statementTester.Execute();
        }

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
    }

    static void TestAbort()
    {
        StatementTester statementTester;

        statementTester.Add("Abort()");

        Variable result = statementTester.Execute();

        Assert::AreEqual(true, statementTester._executionFlow.IsAborting());
        Assert::AreEqual(2, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Aborting: Abort", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual("Aborting: STATEMENT", statementTester._parseErrors.GetError(1)->_errorText);
    }


    static void TestAbortInForLoop()
    {
        StatementTester statementTester;

        statementTester.Add("for Test 0:2");
        statementTester.Add("Abort()");
        statementTester.Add("x = 5");
        statementTester.Add("endfor");

        Variable result = statementTester.Execute();

        Assert::AreEqual(true, statementTester._executionFlow.IsAborting());
        Assert::AreEqual(3, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Aborting: Abort", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual("Aborting: for", statementTester._parseErrors.GetError(1)->_errorText);
        Assert::AreEqual("Aborting: STATEMENT", statementTester._parseErrors.GetError(2)->_errorText);
    }

    static void TestAbortInStatementList()
    {
        StatementTester statementTester;

        statementTester.Add("Abort()");
        statementTester.Add("junk1");
        statementTester.Add("junk2");
        statementTester.Add("junk3");
        statementTester.Add("junk4");

        Variable result = statementTester.Execute();

        Assert::AreEqual(true, statementTester._executionFlow.IsAborting());
        Assert::AreEqual(2, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Aborting: Abort", statementTester._parseErrors.GetError(0)->_errorText);
        Assert::AreEqual("Aborting: STATEMENT", statementTester._parseErrors.GetError(1)->_errorText);
    }

    static void TestButtonReadTrue()
    {
        ExecutionContext _executionContext;
        RDEvaluater _rdEvaluater;
        ParseErrors _parseErrors;
        MockExecutionFlow _executionFlow;
        _executionFlow.SetButtonState(false);

        Variable result = _rdEvaluater.Evaluate("ReadButton(0)", &_executionContext, &_parseErrors, &_executionFlow);

        Assert::AreEqual(0, result.GetValueInt());
    }

    static void TestButtonReadFalse()
    {
        ExecutionContext _executionContext;
        RDEvaluater _rdEvaluater;
        ParseErrors _parseErrors;
        MockExecutionFlow _executionFlow;
        _executionFlow.SetButtonState(true);

        Variable result = _rdEvaluater.Evaluate("ReadButton(0)", &_executionContext, &_parseErrors, &_executionFlow);

        Assert::AreEqual(1, result.GetValueInt());
    }

    static void TestButtonReadNegativeButtonNumberGivesError()
    {
        ExecutionContext _executionContext;
        RDEvaluater _rdEvaluater;
        ParseErrors _parseErrors;
        MockExecutionFlow _executionFlow;
        _executionFlow.SetButtonState(true);

        Variable result = _rdEvaluater.Evaluate("ReadButton(-1)", &_executionContext, &_parseErrors, &_executionFlow);

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

        Variable result = _rdEvaluater.Evaluate("ReadButton(1)", &_executionContext, &_parseErrors, &_executionFlow);

        Assert::AreEqual(1, _parseErrors.GetErrorCount());
        Assert::AreEqual("Invalid Button Number: button number is not defined", _parseErrors.GetError(0)->_errorText);
    }

    static void TestLoopVariableAfterForLoop()
    {
        StatementTester statementTester;

        statementTester.Add("SavedValue = 0");
        statementTester.Add("for Test 4:10");
        statementTester.Add("SavedValue = Test");
        statementTester.Add("endfor");
        statementTester.Add("SavedValue");

        Variable result = statementTester.Execute();

        Assert::AreEqual(10, result.GetValueInt());
    }

    static void TestBreakInForLoop()
    {
        StatementTester statementTester;

        statementTester.Add("SavedValue = 0");
        statementTester.Add("for Test 4:10");
        statementTester.Add("SavedValue = Test");
        statementTester.Add("break");
        statementTester.Add("endfor");
        statementTester.Add("SavedValue");

        Variable result = statementTester.Execute();

        Assert::AreEqual(4, result.GetValueInt());
    }

    static void TestBreakInForLoopWithIf()
    {
        StatementTester statementTester;

        statementTester.Add("SavedValue = 0");
        statementTester.Add("for Test 4:10");
        statementTester.Add("SavedValue = Test");
        statementTester.Add("if Test == 6");
        statementTester.Add("break");
        statementTester.Add("endif");
        statementTester.Add("endfor");
        statementTester.Add("SavedValue");

        Variable result = statementTester.Execute();

        Assert::AreEqual(6, result.GetValueInt());
    }

    static void TestBreakInWhileLoop()
    {
        StatementTester statementTester;

        statementTester.Add("SavedValue = 4");
        statementTester.Add("while (1)");
        statementTester.Add("SavedValue++");
        statementTester.Add("break");
        statementTester.Add("endwhile");
        statementTester.Add("SavedValue");

        Variable result = statementTester.Execute();

        Assert::AreEqual(5, result.GetValueInt());
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

      Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
    }

    static void TestArrayIndexing2()
    {
      StatementTester statementTester;

      statementTester.Add("values = {155, 25, 55}");
      statementTester.Add("values[1]");

      Variable result = statementTester.Execute();

      Assert::AreEqual(25, result.GetValueInt());

      Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
    }

    static void TestArrayIndexingExpression()
    {
        StatementTester statementTester;

        statementTester.Add("values = {155, 25, 55, 66}");
        statementTester.Add("values[1 * 3]");

        Variable result = statementTester.Execute();

        Assert::AreEqual(66, result.GetValueInt());

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
    }

    static void TestArrayAssignment()
    {
      StatementTester statementTester;

      statementTester.Add("values[0] = 88");
      statementTester.Add("values[0]");

      Variable result = statementTester.Execute();

      Assert::AreEqual(88, result.GetValueInt()); 

      Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
    }

    static void TestArrayAssignment2()
    {
      StatementTester statementTester;

      statementTester.Add("values[0] = 88");
      statementTester.Add("values[1] = 99");
      statementTester.Add("values[1]");

      Variable result = statementTester.Execute();

      Assert::AreEqual(99, result.GetValueInt());

      Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
    }

    static void TestArrayAssignment3()
    {
        StatementTester statementTester;

        statementTester.Add("x = 0");
        statementTester.Add("values[x] = 88");
        statementTester.Add("values[0]");

        Variable result = statementTester.Execute();

        Assert::AreEqual(88, result.GetValueInt());

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
    }

    static void TestArrayReferenceClosingBrace()
    {
        StatementTester statementTester;

        statementTester.Add("values = {1, 2, 3}");
        statementTester.Add("values[0");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Missing closing bracket", statementTester._parseErrors.GetError(0)->_errorText);
    }

    static void TestArrayAssignmentMissingClosingBrace()
    {
        StatementTester statementTester;

        statementTester.Add("values[0 = 22");
        statementTester.Add("values[0]");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Missing closing bracket", statementTester._parseErrors.GetError(0)->_errorText);
    }

    static void TestArrayAssignmentMissingClosingBrace2()
    {
      StatementTester statementTester;

      statementTester.Add("values[0] = 33");
      statementTester.Add("values[0 = 22");
      statementTester.Add("values[0]");

      Variable result = statementTester.Execute();

      Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
      Assert::AreEqual("Missing closing bracket", statementTester._parseErrors.GetError(0)->_errorText);
    }

    static void TestArrayComplex()
    {
      StatementTester statementTester;

      //statementTester.Add("Debug(\"LogStatements\", 1)");
      statementTester.Add("func Create()");
      statementTester.Add("for x 0:10");
      statementTester.Add("  value[x] = x * x");
      statementTester.Add("endfor");
      statementTester.Add("return value");
      statementTester.Add("endfunc");

	    statementTester.Add("func Test(values, item)");
      //statementTester.Add("  P(\"Item\")");
      //statementTester.Add("  Pl(item)");
      statementTester.Add("  if (values[item] == item * item)");
      statementTester.Add("    return 1");
      statementTester.Add("  endif");
      statementTester.Add("  return 0");
      statementTester.Add("endfunc");

      statementTester.Add("values = Create()");
      statementTester.Add("matched = 0");
      statementTester.Add("for x 0:10");
      statementTester.Add("  if (Test(values, x) == 0)");
      statementTester.Add("    matched++");
      statementTester.Add("  endif");
      statementTester.Add("endfor");
      statementTester.Add("matched");

      Variable result = statementTester.Execute();

      Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());
      Assert::AreEqual(11, result.GetValueInt());
    }

public:
    static void Run()
    {
        TestArrayComplex();
        TestArrayAssignment();
        TestArrayAssignment2();
        TestArrayAssignment3();
        TestArrayIndexing();
        TestArrayIndexing2();
        TestArrayIndexingExpression();

        TestArrayReferenceClosingBrace();
        TestArrayAssignmentMissingClosingBrace();
        TestArrayAssignmentMissingClosingBrace2();

        TestIf();
        TestNestedIf();
        TestFor();
        TestForDown();
        TestWhileNoExecute();
        TestWhile();
        TestFunctionDefinition();
        TestFunctionDefinitionMissingClosingBrace();

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
        TestAssignmentToExpression();

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
        TestFunctionCallUndefinedVariable();
        TestIfWithAssignment();

        TestAbort();
        TestAbortInForLoop();
        TestAbortInStatementList();
        TestBlankLines();


        TestButtonReadFalse();
        TestButtonReadTrue();
        TestButtonReadNegativeButtonNumberGivesError();
        TestButtonReadOutOfRangeButtonNumberGivesError();

        TestLoopVariableAfterForLoop();
        TestBreakInForLoop();
        TestBreakInForLoopWithIf();
        TestBreakInWhileLoop();
    }
};
