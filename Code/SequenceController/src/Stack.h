class StackFrame
{
public:
	StackFrame()
	{
		SerialNumberStart = 0;
		SerialNumberEnd = 0;
	}

	int SerialNumberStart;
	int SerialNumberEnd;
};

class Stack
{
	int _stackFrameCount = 0;
	StackFrame _stackFrames[10];

public:
	int GetFrameCount() { return _stackFrameCount; }

	void CreateFrame()
	{
		_stackFrameCount++;
	}

	void DestroyFrame()
	{
		_stackFrameCount--;
	}

	StackFrame& GetTopFrame()
	{
		return _stackFrames[_stackFrameCount - 1];
	}
};