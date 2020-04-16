class StackFrame
{
	int InstructionPointer;
	int _ipDebug = 0;

public:
	StackFrame()
	{
		LineNumberStart = 0;
		InstructionPointer = 0;
	}

	int LineNumberStart;

	void IncrementInstructionPointer(const char* tag)
	{
		if (_ipDebug)
		{
			Serial.print("IncrementInstructionPointer:"); Serial.println(tag);
		}

		InstructionPointer++;
	}

	void SetInstructionPointer(int instructionPointer, const char* tag)
	{
		if (_ipDebug)
		{
					Serial.print("SetInstructionPointer:"); Serial.println(tag);
		}
		InstructionPointer = instructionPointer;
	}

	int GetInstructionPointer()
	{
		return InstructionPointer;
	}
};

class Stack
{
	static const int MaxStackFrames = 10;

	int _stackFrameCount = 0;
	StackFrame _stackFrames[MaxStackFrames];

public:
	int GetFrameCount() { return _stackFrameCount; }

	void CreateFrame()
	{
		if (_stackFrameCount == MaxStackFrames)
		{
			Serial.println("Stack Frame count exceeded");
			return;
		}
		_stackFrameCount++;

		StackFrame* pNewFrame = GetTopFrame();
		pNewFrame->SetInstructionPointer(0, "CreateFrame");;
		pNewFrame->LineNumberStart = 0;
	}

	void DestroyFrame()
	{
		if (_stackFrameCount == 0)
		{ 
			Serial.println("Can't delete stack frame");
			return;
		}

		_stackFrameCount--;
	}

	StackFrame* GetTopFrame()
	{
		return &_stackFrames[_stackFrameCount - 1];
	}

	StackFrame* GetCallingFrame()
	{
		if (_stackFrameCount >= 2)
		{
			return &_stackFrames[_stackFrameCount - 2];
		}

		return 0;
	}

	void Clear()
	{
		_stackFrameCount = 0;
	}
};