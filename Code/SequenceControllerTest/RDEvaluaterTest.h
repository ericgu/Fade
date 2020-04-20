
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

    class StatementTester
    {
        char _program[512];

    public:
        ExecutionContext _executionContext;
        RDEvaluater _rdEvaluater;
        ParseErrors _parseErrors;
        FunctionCallerSimulator _functionCaller;

        StatementTester(): 
            _functionCaller(_executionContext.Variables(), _executionContext.GetStack())
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
            return _rdEvaluater.Evaluate(_program, &_executionContext, &_functionCaller, &_parseErrors, 99);
        }

    };

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

        statementTester._executionContext.AddVariableAndSet("V", &Variable(conditionValue));

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

        statementTester.Add("IF (V == 1)");
        statementTester.Add("J = 2");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Missing ENDIF", statementTester._parseErrors.GetError(0)->_errorText);
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
        Assert::AreEqual(5, statementTester._executionContext._functionStore.Lookup("MyFunc")->LineNumberStart);
        Assert::AreEqual(14, statementTester._executionContext._functionStore.Lookup("MyFunc2")->LineNumberStart);
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
    }


    static void TestReturn()
    {
        StatementTester statementTester;

        statementTester.Add("RETURN 1");

        Variable result = statementTester.Execute();

        Assert::AreEqual(0, statementTester._parseErrors.GetErrorCount());

        Assert::AreEqual(1, statementTester._executionContext.GetVariableWithoutErrorCheck("<ReturnValue>")->GetValueInt());
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

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Missing value in RETURN statement", statementTester._parseErrors.GetError(0)->_errorText);
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
        Assert::AreEqual("Extra arguments passed to function", statementTester._parseErrors.GetError(0)->_errorText);
    }

    static void TestUndefinedFunctionWithVariableParameter()
    {
        StatementTester statementTester;

        statementTester.Add("V=15.0");
        statementTester.Add("XX(V)");

        Variable result = statementTester.Execute();

        Assert::AreEqual(1, statementTester._parseErrors.GetErrorCount());
        Assert::AreEqual("Unrecognized function: XX", statementTester._parseErrors.GetError(0)->_errorText);
    }


#if fred
    static void TestAbort()
    {
        StatementTester statementTester;

        statementTester.Add("ABORT()");

        Variable result = statementTester.Execute();

        //Assert::AreEqual(1, statementTester.
        Assert::AreEqual("Unrecognized function: XX", statementTester._parseErrors.GetError(0)->_errorText);


        CommandSourceSimulator commandSource;
        ParseErrors parseErrors;
        LedMessageHandlerSimulator ledMessageHandlerSimulator;

        commandSource.AddCommand("ABORT()");
        commandSource.AddCommand("DI(a,0,10.0)");

        ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);


        CommandResultStatus status = executionFlow.RunProgram(1);

        Assert::AreEqual(true, executionFlow.IsAborting());
    }

    static void TestAbortInForLoop()
    {
        CommandSourceSimulator commandSource;
        ParseErrors parseErrors;
        LedMessageHandlerSimulator ledMessageHandlerSimulator;

        commandSource.AddCommand("FOR Test 0:1");
        commandSource.AddCommand("ABORT()");
        commandSource.AddCommand("DI(a,0,10.0)");
        commandSource.AddCommand("ENDFOR");

        ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);


        CommandResultStatus status = executionFlow.RunProgram(1);

        Assert::AreEqual(true, executionFlow.IsAborting());
    }

    static void TestAbortInFunctionCall()
    {
        CommandSourceSimulator commandSource;
        ParseErrors parseErrors;
        LedMessageHandlerSimulator ledMessageHandlerSimulator;

        commandSource.AddCommand("FOR Test 0:1");
        commandSource.AddCommand("ABORT()");
        commandSource.AddCommand("DI(a,0,10.0)");
        commandSource.AddCommand("ENDFOR");

        ExecutionFlow executionFlow(&commandSource, &parseErrors, &ledMessageHandlerSimulator);


        CommandResultStatus status = executionFlow.RunProgram(1);

        Assert::AreEqual(true, executionFlow.IsAborting());
    }


#endif


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
