class IExecutionFlow
{
public:
	virtual CommandResultStatus RunProgram(int runCount = -1) = 0;
};