class StackWatcherOld

{
	static char* _baseAddress;
	static char* _lastAddress;
	static bool _enabled;

public:
	static void Init()
	{
		char x;
		_baseAddress = &x;
		_enabled = false;
	}

	static void Log(const char* spot)
	{
		char x;

		if (!_enabled)
		{
			return;
		}

		Serial.print("Stack @ ");
		Serial.print(spot);
		Serial.print(" => ");
		Serial.print(_baseAddress - &x);
		Serial.print(" : ");
		Serial.println(_lastAddress - &x);

		_lastAddress = &x;
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

char* StackWatcherOld::_baseAddress;
char* StackWatcherOld::_lastAddress;
bool StackWatcherOld::_enabled = false;

