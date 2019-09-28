class CommandFormatter
{
public:

	static int GetIndentPre(Command command)
	{
		switch (*command.GetString())
		{
		case 'E':
			return -1;
		}

		return 0;
	}

	static int GetIndentPost(Command command)
	{
		switch (*command.GetString())
		{
		case 'F':
			return 1;
		}

		return 0;
	}

	static void MyCat(char *pBuffer, const char* pAdd)
	{
		//Serial.println((char*) "MyCat: ");
		//Serial.println(pBuffer); Serial.flush();

		pBuffer += strlen(pBuffer);

		while (*pAdd != '\0')
		{
			*pBuffer = *pAdd;
			pBuffer++;
			pAdd++;
		}
		*pBuffer = '\0';
	}

	static void PrettyFormat(const char* pCommand, char* pOutput, int bufferSize)
	{
		CommandSource commandSource;
		commandSource.SetCommand(pCommand);

		*pOutput = '\0';

		bool first = true;

		int indent = 0;
		while (true)
		{
			Command command = commandSource.GetNextCommand(0);

			if (!first)
			{
				MyCat(pOutput, "\n");
			}
			first = false;

			indent += GetIndentPre(command);
			for (int i = 0; i < indent * 2; i++)
			{
				MyCat(pOutput, " ");
			}
			char* pString = command.GetString();
			if (strlen(pString) != 0)
			{
				MyCat(pOutput, pString);
			}

			indent += GetIndentPost(command);

			if (command.GetSerialNumber() == -1)
			{
				break;
			}
		}
	}
};
