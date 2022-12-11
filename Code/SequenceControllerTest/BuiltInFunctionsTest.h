#pragma once

class BuildInFunctionsTest
{
    static void TestConfigLed()
    {
        ExecutionContext executionContext;
        ParseErrors parseErrors;
        MockExecutionFlow executionFlow; 
        ExpressionResult expressionResult;

        executionContext.AddVariableAndSet("#A", &Variable(4));
        executionContext.AddVariableAndSet("#A0", &Variable(3));
        executionContext.AddVariableAndSet("#A1", &Variable("RGB"));
        executionContext.AddVariableAndSet("#A2", &Variable(3));
        executionContext.AddVariableAndSet("#A3", &Variable(4444));

        BuiltInFunctions::HandleBuiltInFunctions("ConfigLed", &executionContext, &parseErrors, 15, 0, &executionFlow, &expressionResult);

        Assert::AreEqual(3, executionFlow._ledGroupNumber);
        Assert::AreEqual("RGB", executionFlow._pLedType);
        Assert::AreEqual(3, executionFlow._ledCount);

        Assert::AreEqual(4444, executionFlow._pins[0]);
        Assert::AreEqual(1, executionFlow._pinCount);
    }

    static void TestConfigLedFourPins()
    {
        ExecutionContext executionContext;
        ParseErrors parseErrors;
        MockExecutionFlow executionFlow;
        ExpressionResult expressionResult;

        executionContext.AddVariableAndSet("#A", &Variable(7));
        executionContext.AddVariableAndSet("#A0", &Variable(3));
        executionContext.AddVariableAndSet("#A1", &Variable("RGB"));
        executionContext.AddVariableAndSet("#A2", &Variable(3));
        executionContext.AddVariableAndSet("#A3", &Variable(4444));
        executionContext.AddVariableAndSet("#A4", &Variable(5555));
        executionContext.AddVariableAndSet("#A5", &Variable(6666));
        executionContext.AddVariableAndSet("#A6", &Variable(7777));

        BuiltInFunctions::HandleBuiltInFunctions("ConfigLed", &executionContext, &parseErrors, 15, 0, &executionFlow, &expressionResult);

        Assert::AreEqual(0, parseErrors.GetErrorCount());

        Assert::AreEqual(3, executionFlow._ledGroupNumber);
        Assert::AreEqual("RGB", executionFlow._pLedType);
        Assert::AreEqual(3, executionFlow._ledCount);

        Assert::AreEqual(4444, executionFlow._pins[0]);
        Assert::AreEqual(5555, executionFlow._pins[1]);
        Assert::AreEqual(6666, executionFlow._pins[2]);
        Assert::AreEqual(7777, executionFlow._pins[3]);
        Assert::AreEqual(4, executionFlow._pinCount);

    }

    static void TestConfigLedError()
    {
      ExecutionContext executionContext;
      ParseErrors parseErrors;
      MockExecutionFlow executionFlow;
      ExpressionResult expressionResult;

      executionFlow._configLedsReturnValue = false;

      executionContext.AddVariableAndSet("#A", &Variable(4));
      executionContext.AddVariableAndSet("#A0", &Variable(3));
      executionContext.AddVariableAndSet("#A1", &Variable("Fred"));
      executionContext.AddVariableAndSet("#A2", &Variable(3));
      executionContext.AddVariableAndSet("#A3", &Variable(4444));

      BuiltInFunctions::HandleBuiltInFunctions("ConfigLed", &executionContext, &parseErrors, 15, 0, &executionFlow, &expressionResult);

      Assert::AreEqual(1, parseErrors.GetErrorCount());
      Assert::AreEqual("ConfigLed error", parseErrors.GetError(0)->_errorText);
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

        BuiltInFunctions::HandleBuiltInFunctions("ConfigButton", &executionContext, &parseErrors, 15, 0, &executionFlow, &expressionResult);

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

        BuiltInFunctions::HandleBuiltInFunctions("Debug", &executionContext, &parseErrors, 15, 0, &executionFlow, &expressionResult);

        Assert::AreEqual(1, Environment.DebugLogStatements);
    }

    //HSVtoRGB

    static void TestHSVtoRGB()
    {
      ExecutionContext executionContext;
      ParseErrors parseErrors;
      MockExecutionFlow executionFlow;
      ExpressionResult expressionResult;

      executionContext.AddVariableAndSet("#A0", &Variable(60));
      executionContext.AddVariableAndSet("#A1", &Variable(1));
      executionContext.AddVariableAndSet("#A2", &Variable(1));

      BuiltInFunctions::HandleBuiltInFunctions("HsvToRgb", &executionContext, &parseErrors, 15, 0, &executionFlow, &expressionResult);

      Assert::AreEqual(1.0, expressionResult._variable.GetValueFloat(0));
      Assert::AreEqual(1.0, expressionResult._variable.GetValueFloat(1));
      Assert::AreEqual(0.0, expressionResult._variable.GetValueFloat(2));
    }

    static void TestMax1()
    {
        ExecutionContext executionContext;
        ParseErrors parseErrors;
        MockExecutionFlow executionFlow;
        ExpressionResult expressionResult;

        executionContext.AddVariableAndSet("#A", &Variable(3));
        executionContext.AddVariableAndSet("#A0", &Variable(60));
        executionContext.AddVariableAndSet("#A1", &Variable(1));
        executionContext.AddVariableAndSet("#A2", &Variable(5));

        BuiltInFunctions::HandleBuiltInFunctions("Max", &executionContext, &parseErrors, 15, 0, &executionFlow, &expressionResult);

        Assert::AreEqual(60.0, expressionResult._variable.GetValueFloat(0));
    }


    static void TestMax2()
    {
        ExecutionContext executionContext;
        ParseErrors parseErrors;
        MockExecutionFlow executionFlow;
        ExpressionResult expressionResult;

        executionContext.AddVariableAndSet("#A", &Variable(3));
        executionContext.AddVariableAndSet("#A1", &Variable(1));
        executionContext.AddVariableAndSet("#A0", &Variable(60));
        executionContext.AddVariableAndSet("#A2", &Variable(5));

        BuiltInFunctions::HandleBuiltInFunctions("Max", &executionContext, &parseErrors, 15, 0, &executionFlow, &expressionResult);

        Assert::AreEqual(60.0, expressionResult._variable.GetValueFloat(0));
    }

    static void TestMax3()
    {
        ExecutionContext executionContext;
        ParseErrors parseErrors;
        MockExecutionFlow executionFlow;
        ExpressionResult expressionResult;

        executionContext.AddVariableAndSet("#A", &Variable(3));
        executionContext.AddVariableAndSet("#A1", &Variable(1));
        executionContext.AddVariableAndSet("#A2", &Variable(5));
        executionContext.AddVariableAndSet("#A0", &Variable(60));

        BuiltInFunctions::HandleBuiltInFunctions("Max", &executionContext, &parseErrors, 15, 0, &executionFlow, &expressionResult);

        Assert::AreEqual(60.0, expressionResult._variable.GetValueFloat(0));
    }


    static void TestMin1()
    {
        ExecutionContext executionContext;
        ParseErrors parseErrors;
        MockExecutionFlow executionFlow;
        ExpressionResult expressionResult;

        executionContext.AddVariableAndSet("#A", &Variable(3));
        executionContext.AddVariableAndSet("#A0", &Variable(60));
        executionContext.AddVariableAndSet("#A1", &Variable(1));
        executionContext.AddVariableAndSet("#A2", &Variable(5));

        BuiltInFunctions::HandleBuiltInFunctions("Min", &executionContext, &parseErrors, 15, 0, &executionFlow, &expressionResult);

        Assert::AreEqual(1.0, expressionResult._variable.GetValueFloat(0));
    }


    static void TestMin2()
    {
        ExecutionContext executionContext;
        ParseErrors parseErrors;
        MockExecutionFlow executionFlow;
        ExpressionResult expressionResult;

        executionContext.AddVariableAndSet("#A", &Variable(3));
        executionContext.AddVariableAndSet("#A1", &Variable(1));
        executionContext.AddVariableAndSet("#A0", &Variable(60));
        executionContext.AddVariableAndSet("#A2", &Variable(5));

        BuiltInFunctions::HandleBuiltInFunctions("Min", &executionContext, &parseErrors, 15, 0, &executionFlow, &expressionResult);

        Assert::AreEqual(1.0, expressionResult._variable.GetValueFloat(0));
    }

    static void TestMin3()
    {
        ExecutionContext executionContext;
        ParseErrors parseErrors;
        MockExecutionFlow executionFlow;
        ExpressionResult expressionResult;

        executionContext.AddVariableAndSet("#A", &Variable(3));
        executionContext.AddVariableAndSet("#A1", &Variable(1));
        executionContext.AddVariableAndSet("#A2", &Variable(5));
        executionContext.AddVariableAndSet("#A0", &Variable(60));

        BuiltInFunctions::HandleBuiltInFunctions("Min", &executionContext, &parseErrors, 15, 0, &executionFlow, &expressionResult);

        Assert::AreEqual(1.0, expressionResult._variable.GetValueFloat(0));
    }

    static void TestRandWrongArgumentCount()
    {
      ExecutionContext executionContext;
      ParseErrors parseErrors;
      MockExecutionFlow executionFlow;
      ExpressionResult expressionResult;

      ExpressionTokenSource expressionTokenSource("Rand(1)");

      executionContext.AddVariableAndSet("#A", &Variable(1));
      executionContext.AddVariableAndSet("#A0", &Variable(1));

      BuiltInFunctions::HandleBuiltInFunctions("Rand", &executionContext, &parseErrors, 15, &expressionTokenSource, &executionFlow, &expressionResult);

      Assert::AreEqual(1, parseErrors.GetErrorCount());
      Assert::AreEqual("Rand requires zero or two parameters", parseErrors.GetError(0)->_errorText);
    }

    static void TestPrintString()
    {
        ExecutionContext executionContext;
        ParseErrors parseErrors;
        MockExecutionFlow executionFlow;
        ExpressionResult expressionResult;

        ExpressionTokenSource expressionTokenSource("P(\"Hello\")");

        executionContext.AddVariableAndSet("#A", &Variable(1));
        executionContext.AddVariableAndSet("#A0", &Variable("Hello"));

        Serial.SetOutput(false);
        BuiltInFunctions::HandleBuiltInFunctions("P", &executionContext, &parseErrors, 15, &expressionTokenSource, &executionFlow, &expressionResult);
        Assert::AreEqual("Hello", Serial.GetLastString());
        Serial.SetOutput(true);
    }

    static void TestPrintStringThreeParameters()
    {
        ExecutionContext executionContext;
        ParseErrors parseErrors;
        MockExecutionFlow executionFlow;
        ExpressionResult expressionResult;

        ExpressionTokenSource expressionTokenSource("P(\"Hello\")");

        executionContext.AddVariableAndSet("#A", &Variable(3));
        executionContext.AddVariableAndSet("#A0", &Variable("Hello"));
        executionContext.AddVariableAndSet("#A1", &Variable("There"));
        executionContext.AddVariableAndSet("#A2", &Variable("Everybody"));

        Serial.SetOutput(false);
        BuiltInFunctions::HandleBuiltInFunctions("P", &executionContext, &parseErrors, 15, &expressionTokenSource, &executionFlow, &expressionResult);
        Assert::AreEqual("HelloThereEverybody", Serial.GetLastString());
        Serial.SetOutput(true);
    }

    static void TestConfigEnvironment()
    {
        ExecutionContext executionContext;
        ParseErrors parseErrors;
        MockExecutionFlow executionFlow;
        ExpressionResult expressionResult;

        ExpressionTokenSource expressionTokenSource("ConfigEnvironment(\"VectorItemDataPoolCount\", 33)");

        executionContext.AddVariableAndSet("#A", &Variable(2));
        executionContext.AddVariableAndSet("#A0", &Variable("VectorItemDataPoolCount"));
        executionContext.AddVariableAndSet("#A1", &Variable(33));

        int temp = Environment.VectorItemChunkSize;
        BuiltInFunctions::HandleBuiltInFunctions("ConfigEnvironment", &executionContext, &parseErrors, 15, &expressionTokenSource, &executionFlow, &expressionResult);

        Assert::AreEqual(0, parseErrors.GetErrorCount());
        Assert::AreEqual(512, Environment.VectorItemChunkSize);
        Environment.VectorItemChunkSize = temp;
    }

    static void TestConfigEnvironment2()
    {
        ExecutionContext executionContext;
        ParseErrors parseErrors;
        MockExecutionFlow executionFlow;
        ExpressionResult expressionResult;

        ExpressionTokenSource expressionTokenSource("ConfigEnvironment(\"VariableStoreChunkSize\", 1833)");

        executionContext.AddVariableAndSet("#A", &Variable(2));
        executionContext.AddVariableAndSet("#A0", &Variable("VariableStoreChunkSize"));
        executionContext.AddVariableAndSet("#A1", &Variable(1833));

        int temp = Environment.VariableStoreChunkSize;
        BuiltInFunctions::HandleBuiltInFunctions("ConfigEnvironment", &executionContext, &parseErrors, 15, &expressionTokenSource, &executionFlow, &expressionResult);

        Assert::AreEqual(0, parseErrors.GetErrorCount());
        Assert::AreEqual(1833, Environment.VariableStoreChunkSize);
        Environment.VariableStoreChunkSize = temp;
    }

public:
    static void Run()
    {
        TestConfigEnvironment();
        TestConfigEnvironment2();

        TestConfigLed();
        TestConfigLedFourPins();
        TestConfigLedError();

        TestConfigTouchButton();
        TestDebugLogStatements();
        TestHSVtoRGB();
        TestRandWrongArgumentCount();

        TestPrintString();
        TestPrintStringThreeParameters();

        TestMax1();
        TestMax2();
        TestMax3();
        TestMin1();
        TestMin2();
        TestMin3();
    }
};
