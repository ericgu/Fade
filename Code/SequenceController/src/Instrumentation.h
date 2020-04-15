#define PROLOGUE Instrumentation::Prologue(__func__);
#define EPILOGUE Instrumentation::Epilogue(__func__);
#define RETURN(x) Instrumentation::Epilogue(__func__); return x;

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