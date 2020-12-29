#pragma once

class BuildInFunctionsTest
{
    static void TestConfigLed()
    {
        ExecutionContext executionContext;
        ParseErrors parseErrors;
        MockExecutionFlow executionFlow; 
        ExpressionResult expressionResult;

        executionContext.AddVariableAndSet("#A0", &Variable(3));
        executionContext.AddVariableAndSet("#A1", &Variable("RGB"));
        executionContext.AddVariableAndSet("#A2", &Variable(3));
        executionContext.AddVariableAndSet("#A3", &Variable(4444));

        BuiltInFunctions::HandleBuiltInFunctions("CONFIGLED", &executionContext, &parseErrors, 0, &executionFlow, &expressionResult);

        Assert::AreEqual(3, executionFlow._ledGroupNumber);
        Assert::AreEqual("RGB", executionFlow._pLedType);
        Assert::AreEqual(3, executionFlow._ledCount);
        Assert::AreEqual(4444, executionFlow._pin1);
        Assert::AreEqual(-1, executionFlow._pin2);
        Assert::AreEqual(-1, executionFlow._pin3);
        Assert::AreEqual(-1, executionFlow._pin4);
    }

    static void TestConfigLedFourPins()
    {
        ExecutionContext executionContext;
        ParseErrors parseErrors;
        MockExecutionFlow executionFlow;
        ExpressionResult expressionResult;

        executionContext.AddVariableAndSet("#A0", &Variable(3));
        executionContext.AddVariableAndSet("#A1", &Variable("RGB"));
        executionContext.AddVariableAndSet("#A2", &Variable(3));
        executionContext.AddVariableAndSet("#A3", &Variable(4444));
        executionContext.AddVariableAndSet("#A4", &Variable(5555));
        executionContext.AddVariableAndSet("#A5", &Variable(6666));
        executionContext.AddVariableAndSet("#A6", &Variable(7777));

        BuiltInFunctions::HandleBuiltInFunctions("CONFIGLED", &executionContext, &parseErrors, 0, &executionFlow, &expressionResult);

        Assert::AreEqual(3, executionFlow._ledGroupNumber);
        Assert::AreEqual("RGB", executionFlow._pLedType);
        Assert::AreEqual(3, executionFlow._ledCount);
        Assert::AreEqual(4444, executionFlow._pin1);
        Assert::AreEqual(5555, executionFlow._pin2);
        Assert::AreEqual(6666, executionFlow._pin3);
        Assert::AreEqual(7777, executionFlow._pin4);
    }

    static void TestConfigTouchButton()
    {
        ExecutionContext executionContext;
        ParseErrors parseErrors;
        MockExecutionFlow executionFlow;
        ExpressionResult expressionResult;

        executionContext.AddVariableAndSet("#A0", &Variable(3));
        executionContext.AddVariableAndSet("#A1", &Variable("TOUCH"));
        executionContext.AddVariableAndSet("#A2", &Variable(3));
        executionContext.AddVariableAndSet("#A3", &Variable(20));

        BuiltInFunctions::HandleBuiltInFunctions("CONFIGBUTTON", &executionContext, &parseErrors, 0, &executionFlow, &expressionResult);

        Assert::AreEqual(3, executionFlow._buttonNumber);
        Assert::AreEqual("TOUCH", executionFlow._pButtonType);
        Assert::AreEqual(3, executionFlow._pinNumber);
        Assert::AreEqual(20, executionFlow._parameter1);
    }

    static void TestDebugLogStatements()
    {
        ExecutionContext executionContext;
        ParseErrors parseErrors;
        MockExecutionFlow executionFlow;
        ExpressionResult expressionResult;

        executionContext.AddVariableAndSet("#A0", &Variable("LogStatements"));
        executionContext.AddVariableAndSet("#A1", &Variable(1));

        BuiltInFunctions::HandleBuiltInFunctions("DEBUG", &executionContext, &parseErrors, 0, &executionFlow, &expressionResult);

        Assert::AreEqual(1, Environment.DebugLogStatements);
    }


public:
    static void Run()
    {
        TestConfigLed();
        TestConfigLedFourPins();
        TestConfigTouchButton();
        TestDebugLogStatements();
    }
};
