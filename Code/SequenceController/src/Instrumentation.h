
#ifdef fred
#define PROLOGUE Instrumentation::Prologue(__func__); int __start = TimeServices::GetTicks()
#define EPILOGUE Instrumentation::Epilogue(__func__); int __delta = TimeServices::GetTicks() - __start; FunctionTimesInstance.Record(__func__, __delta)
#define RETURN(x) Instrumentation::Epilogue(__func__); int __delta = TimeServices::GetTicks() - __start; FunctionTimesInstance.Record(__func__, __delta); return x
#else
#define PROLOGUE 
#define EPILOGUE 
#define RETURN(x) return x

#endif

class FunctionTime
{

public:
    FunctionTime()
    {
        _functionName[0] = '\0';
        _functionTime = 0;
        _functionCount = 0;
    }


    char _functionName[128];
    int _functionTime;
    int _functionCount;
};

class FunctionTimes
{
    FunctionTime _functionTimes[50];
    int _functionTimesCount = 0;

    FunctionTime* Lookup(const char *pFunctionName)
    {
        for (int i = 0; i < _functionTimesCount; i++)
        {
            if (strcmp((_functionTimes + i)->_functionName, pFunctionName) == 0)
            {
                return _functionTimes + i;
            }
        }

        strcpy(_functionTimes[_functionTimesCount]._functionName, pFunctionName);
        _functionTimes[_functionTimesCount]._functionCount = 0;
        _functionTimes[_functionTimesCount]._functionTime = 0;
        _functionTimesCount++;

        return _functionTimes + (_functionTimesCount - 1);
    }


public:
    void Clear()
    {
        _functionTimesCount = 0;
    }

    void Record(const char* pFunctionName, int functionTime)
    {
        return;
        FunctionTime* pFunctionTime = Lookup(pFunctionName);

        pFunctionTime->_functionTime += functionTime;
        pFunctionTime->_functionCount++;
    }

    void Dump()
    {
        return;
        for (int i = 0; i < _functionTimesCount; i++)
        {
            char buffer[128];
            FunctionTime* pFunctionTime = _functionTimes + i;
            sprintf(buffer, "Name, count, time, average: %s %d %d %f", pFunctionTime->_functionName, pFunctionTime->_functionCount, pFunctionTime->_functionTime, (float)pFunctionTime->_functionTime / (float)pFunctionTime->_functionCount);
            Serial.println(buffer);
        }
    }

};

FunctionTimes FunctionTimesInstance;


class Instrumentation
{
public:
	static void Prologue(const char* functionName)
	{
		StackWatcher::Log(functionName);
	}

	static void Epilogue(const char* functionName)
	{

	}
};