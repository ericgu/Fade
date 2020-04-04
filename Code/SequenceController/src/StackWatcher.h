class StackWatcher
{
    static char* _stackBase;

public:
    static void Init()
    {
        char temp;
        _stackBase = &temp;
    }

    static void Log(const char* spot)
    {
        return;

        char temp;
        Serial.print("Stack: (");
        Serial.print(spot);
        Serial.print(") = ");
        Serial.println((int)(_stackBase - &temp));
    }

};

char* StackWatcher::_stackBase;
