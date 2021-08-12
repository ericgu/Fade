#include <cstdlib>

class MyRandom
{
public:


	static int GetValue(int min, int max)
	{
        int returnValue = min + rand() % (max - min + 1);

		return returnValue;
	}

    static float GetFloatValue()
    {
        return rand() / ((float) RAND_MAX);
    }
};

