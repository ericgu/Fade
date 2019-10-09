#pragma once

class SerialOutput
{
	const char* _pLastString;
	bool _outputOn;

public:
	SerialOutput()
	{
		_pLastString = (char*) "<undefined>";
		_outputOn = true;
	}

	void println(const char* pString)
	{
		if (_outputOn)
		{
			puts(pString);
		}
		_pLastString = pString;
	}

	void print(const char* pString)
	{
		if (_outputOn)
		{
			printf(pString);
		}
		_pLastString = pString;
	}

	void println(int value)
	{
		printf("%d\n", value);
	}

	void print(int value)
	{
		printf("%d", value);
	}

	void flush() {}

	const char* GetLastString() { return _pLastString; }

	void SetOutput(bool outputOn)
	{
		_outputOn = outputOn;
	}
};

SerialOutput Serial;
