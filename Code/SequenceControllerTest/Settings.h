#pragma once

class Settings
{
	char _programText[16636];
	bool _shouldExecuteCode;

public:

	Settings()
	{
		_programText[0] = '\0';
		_shouldExecuteCode = 0;
	}

	void LoadProgramText(char* buffer, int bufferSize)
	{
		strcpy(buffer, _programText);
	}

	bool SaveProgramText(const char* value)
	{
		strcpy(_programText, value);
		return true;
	}

	bool LoadShouldExecuteCode()
	{
		return _shouldExecuteCode;
	}

	bool SaveShouldExecuteCode(bool value)
	{
		_shouldExecuteCode = value;
		return true;
	}
};
