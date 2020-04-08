class TimeServices
{
public:
	static unsigned long GetTicks()
	{
		return micros();
	}

    static void DelayMicroseconds(int delay)
    {
        delayMicroseconds(delay);
    }

    static void TaskDelay(int delay)
    {
        vTaskDelay(delay);
    }
};