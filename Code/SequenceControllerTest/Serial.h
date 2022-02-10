#pragma once

class SerialOutput
{
	char* _pLastString;
	bool _outputOn;

public:
	SerialOutput()
	{
		_pLastString = new char[16636];
		strcpy(_pLastString, "<Undefined>");
		_outputOn = true;
	}

    ~SerialOutput()
    {
        delete[] _pLastString;
    }

	void println(const char* pString)
	{
		if (_outputOn)
		{
			puts(pString);
		}
		strcpy(_pLastString, pString);
	}

	void print(const char* pString)
	{
		if (_outputOn)
		{
			printf(pString);
		}
		strcpy(_pLastString, pString);
	}

	void println(int value)
	{
		if (_outputOn)
		{
			printf("%d\n", value);
		}
	}

	void print(int value)
	{
		if (_outputOn)
		{
			printf("%d", value);
		}
	}

	void print(float value)
	{
		if (_outputOn)
		{
			printf("%f", value);
		}
	}

	void println(float value)
	{
		if (_outputOn)
		{
			printf("%f\n", value);
		}
	}

	void println()
	{
		if (_outputOn)
		{
			printf("\n");
		}
	}

	void flush() {}

	const char* GetLastString() { return _pLastString; }

	void SetOutput(bool outputOn)
	{
		_outputOn = outputOn;
		if (!outputOn)
		{
			*_pLastString = '\0';
		}
	}
};

SerialOutput Serial;