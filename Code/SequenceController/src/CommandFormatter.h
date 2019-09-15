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
		case 'L':
			return 1;
		}

		return 0;
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
				strcat_s(pOutput, bufferSize, "\n");
			}
			first = false;

			indent += GetIndentPre(command);
			for (int i = 0; i < indent * 2; i++)
			{
				strcat_s(pOutput, bufferSize, " ");
			}
			strcat_s(pOutput, bufferSize, "$");
			strcat_s(pOutput, bufferSize, command.GetString());

			indent += GetIndentPost(command);

			if (command.GetSerialNumber() == -1)
			{
				break;
			}
		}
	}
};
