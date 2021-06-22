#pragma once

#include "VariableData.h"

class VariableDataTest
{
  static void TestAssignmentOperator()
  {
    VariableData variableData;
    variableData.SetValue(0, 15.0F);
    variableData._valueCount = 10;

    Assert::AreEqual(15.0F, variableData.GetValue(0));

    VariableData second;
    second = variableData;

    second.SetValue(0, 22.0F);

    Assert::AreEqual(15.0F, variableData.GetValue(0));
    Assert::AreEqual(22.0F, second.GetValue(0));
  }

  static void TestReferenceCounting()
  {
    VariableData variableData;
    variableData.SetValue(0, 15.0F);
    Assert::AreEqual(0, variableData.GetReferenceCount());

    variableData.IncrementReferenceCount();

    Assert::AreEqual(1, variableData.GetReferenceCount());

    variableData.DecrementReferenceCountInternal();

    Assert::AreEqual(0, variableData.GetReferenceCount());
  }

public:
  static void Run()
  {
    TestAssignmentOperator();
    TestReferenceCounting();
  }
};
#pragma once
