class ProfilerClass
{
    char _stage[512];
    long int _startMicros;

    char _stageBig[512];
    long int _startMicrosBig;

public:
    ProfilerClass()
    {
        _stage[0] = 0;
        _stageBig[0] = 0;
    }

    void Start(const char* stage)
    {
        char buffer[1024];
        long int delta = micros() - _startMicros;
        snprintf(buffer, sizeof(buffer) / sizeof(char), "%s = %d", _stage, (int) delta);
        //Serial.println(buffer);
        SafeString::StringCopy(_stage, stage, sizeof(_stage));
        _startMicros = micros();
    }

    void StartBig(const char* stage)
    {
        char buffer[1024];
        long int delta = micros() - _startMicrosBig;
        snprintf(buffer, sizeof(buffer) / sizeof(char), "%s = %d", _stageBig, (int) delta);
        //Serial.println(buffer);
		SafeString::StringCopy(_stageBig, stage, sizeof(_stageBig));
        _startMicrosBig = micros();
    }

    
};

ProfilerClass Profiler;