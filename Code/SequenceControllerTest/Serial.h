#pragma once

class SerialOutput
{
public:
	void println(char* pString)
	{
		puts(pString);
	}
};

SerialOutput Serial;
