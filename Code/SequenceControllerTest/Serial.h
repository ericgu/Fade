#pragma once

class SerialOutput
{
public:
	void println(char* pString)
	{
		puts(pString);
	}
	void flush() {}
};

SerialOutput Serial;
