class StackWatcher
{
    static char* _stackBase;
   	static char* _lastAddress;
	static bool _enabled;

public:
    static void Init()
    {
        char temp;
        _stackBase = &temp;
        _enabled = false; 
    }

    static void Log(const char* spot)
    {
        if (!_enabled)
        {
            return;
        }

        char temp;
        Serial.print("Stack: (");
        Serial.print(spot);
        Serial.print(") = ");
        Serial.print((int)(_stackBase - &temp));
        Serial.print(") = ");
        Serial.println((int)(_lastAddress - &temp));

        _lastAddress = &temp;
    }
    
	static void Enable()
	{
		_enabled = true;
	}

	static void Disable()
	{
		_enabled = true;
	}

};

char* StackWatcher::_stackBase;
char* StackWatcher::_lastAddress;
bool StackWatcher::_enabled = false;
