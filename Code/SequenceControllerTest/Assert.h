#pragma once

class Assert
{
	static char _buffer[16636];
	static int _assertCount;

public:
	static int GetAssertCount() { return _assertCount;  }

	static void Failed(char* tag)
	{
		puts("Failed: ");
		puts(tag);
		puts("\n");
	}

  static void AreEqual(void* pExpected, void* pActual)
  {
    _assertCount++;

    if (pExpected != pActual)
    {
      sprintf_s(_buffer, "Expected %p got %p", pExpected, pActual);
      Failed(_buffer);
    }
  }

	static void AreEqual(int expected, int actual)
	{
		_assertCount++; 

		if (expected != actual)
		{
			sprintf_s(_buffer, "Expected %d got %d", expected, actual);
			Failed(_buffer);
		}
	}

	static void AreEqual(const char* expected, char* actual)
	{
		_assertCount++;

		if (actual == 0)
		{
			actual = (char*) "<null value>";
		}

		if (strcmp(expected, actual) != 0)
		{
			sprintf_s(_buffer, "Expected %s got %s", expected, actual);
			Failed(_buffer);
		}
	}

	static void AreEqual(const char* expected, const char* actual)
	{
		_assertCount++;

		if (strcmp(expected, actual) != 0)
		{
			sprintf_s(_buffer, "Expected %s got %s", expected, actual);
			Failed(_buffer);
		}
	}

  static void IsNull(const char* actual)
  {
    _assertCount++;

    if (actual != NULL)
    {
      sprintf_s(_buffer, "Expected NULL got %s", actual);
      Failed(_buffer);
    }
  }

	static void AreEqual(float expected, float actual)
	{
		_assertCount++;

		if (expected != actual)
		{
			sprintf_s(_buffer, "Expected %f got %f", expected, actual);
			Failed(_buffer);
		}
	}

};

char Assert::_buffer[16636];
int Assert::_assertCount = 0;
