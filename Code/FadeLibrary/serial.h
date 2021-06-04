#pragma once

class SerialOutput
{
	bool _outputOn;
  char* _pBuffer;


public:
	SerialOutput()
	{
    _pBuffer = new char[1024];
		_outputOn = true;
	}


  void printf(const char* s)
	{
    NewSerialTextAvailable(s);
	}

  void printf(const char* s, int value)
	{
    sprintf(_pBuffer, s, value);
    NewSerialTextAvailable(_pBuffer);
	}

  void puts(const char* s)
	{
    NewSerialTextAvailable(s);
	}

	void println(const char* pString)
	{
		if (_outputOn)
		{
      *_pBuffer = '\0';
      strcpy(_pBuffer, pString);
      strcat(_pBuffer, "\r\n");
			puts(_pBuffer);
		}
	}

	void print(const char* pString)
	{
		if (_outputOn)
		{
			puts(pString);
		}
	}

	void println(int value)
	{
		if (_outputOn)
		{
      *_pBuffer = '\0';

      sprintf(_pBuffer, "%d", value);
      strcat(_pBuffer, "\r\n");
      puts(_pBuffer);
		}
	}

	void print(int value)
	{
		if (_outputOn)
		{
      *_pBuffer = '\0';

      sprintf(_pBuffer, "%d", value);
      puts(_pBuffer);
		}
	}

	void print(float value)
	{
		if (_outputOn)
		{
      *_pBuffer = '\0';

      sprintf(_pBuffer, "%f", value);
      puts(_pBuffer);
		}
	}

	void println(float value)
	{
		if (_outputOn)
		{
      *_pBuffer = '\0';

      sprintf(_pBuffer, "%f", value);
      strcat(_pBuffer, "\r\n");
      puts(_pBuffer);
		}
	}

	void println()
	{
		if (_outputOn)
		{ 
      puts("\n");
		}
	}

	void flush() {}

	void SetOutput(bool outputOn)
	{
		_outputOn = outputOn;
	}
};

SerialOutput Serial;
