class MyRandom
{
public:
	static int GetValue(int minValue, int maxValue)
	{
        //Serial.println("Min, max, value: ");
        int value = random(minValue, maxValue);
        //Serial.println(minValue);
        //Serial.println(maxValue);
        //Serial.println(value);

        return value;
	}
};