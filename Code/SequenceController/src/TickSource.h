class TickSource
{
public:
	unsigned long GetTicks()
	{
		return micros();
	}
};