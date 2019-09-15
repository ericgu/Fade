// SequenceControllerTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Assert.h"
#include "Serial.h"
#include "CommandTest.h"
#include "CommandSourceTest.h"
#include "CommandFormatterTest.h"
#include "LedState.h"
#include "LedPwm.h"
#include "CommandResult.h"
#include "LedCommand.h"
#include "VariableTest.h"
#include "StackTest.h"
#include "ListParserTest.h"
#include "CommandDecoderTest.h"
#include "LedManagerTest.h"
#include "CommandSourceSimulatorTest.h"
#include "ExecutionFlowTest.h"
#include "TimebaseTest.h"
#include "IntegrationTest.h"
#include "LoopTest.h"


int main()
{
	IntegrationTest::Run();

	CommandSourceSimulatorTest::Test();
	
	ExecutionFlowTest::Run();
	TimebaseTest::Run();
	CommandSourceTest::Run();
	LedManagerTest::Run();
	CommandDecoderTest::Run();
	

	LoopTest::Run();

	VariableTest::Run();

	StackTest::Run();

	ListParserTest::Run();
	CommandTest::Run();

	CommandFormatterTest::Run();

    return 0;
}

