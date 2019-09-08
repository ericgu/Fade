#pragma once

class Assert
{
	static char _buffer[128];

public:
	static void Failed(char* tag)
	{
		puts("Failed: ");
		puts(tag);
		puts("\n");
	}

	static void AreEqual(int expected, int actual)
	{
		if (expected != actual)
		{
			sprintf_s(_buffer, "Expected %d got %d", expected, actual);
			Failed(_buffer);
		}
	}

	static void AreEqual(char* expected, char* actual)
	{
		if (strcmp(expected, actual) != 0)
		{
			sprintf_s(_buffer, "Expected %s got %s", expected, actual);
			Failed(_buffer);
		}
	}

	static void AreEqual(const char* expected, char* actual)
	{
		if (strcmp(expected, actual) != 0)
		{
			sprintf_s(_buffer, "Expected %s got %s", expected, actual);
			Failed(_buffer);
		}
	}

	static void AreEqual(const char* expected, const char* actual)
	{
		if (strcmp(expected, actual) != 0)
		{
			sprintf_s(_buffer, "Expected %s got %s", expected, actual);
			Failed(_buffer);
		}
	}

	static void AreEqual(float expected, float actual)
	{
		if (expected != actual)
		{
			sprintf_s(_buffer, "Expected %f got %f", expected, actual);
			Failed(_buffer);
		}
	}

};

char Assert::_buffer[128];
