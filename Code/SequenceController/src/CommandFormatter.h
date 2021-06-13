class CommandFormatter
{
public:

	static const int preStringCount = 5;
	static const char* preStrings[];

	static const int postStringCount = 5;
	static const char* postStrings[];

	static int GetIndentPre(Command* pCommand)
	{
		char* pCommandString = pCommand->GetString();

		for (int i = 0; i < preStringCount; i++)
		{
			if (strncmp(preStrings[i], pCommandString, strlen(preStrings[i])) == 0)
			{
				return -1;
			}
		}

		return 0;
	}

	static int GetIndentPost(Command* pCommand)
	{
		char* pCommandString = pCommand->GetString();

		for (int i = 0; i < postStringCount; i++)
		{
			if (strncmp(postStrings[i], pCommandString, strlen(postStrings[i])) == 0)
			{
				return 1;
			}
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
		//Serial.println(">PrettyFormat");

		CommandSource commandSource;
		commandSource.SetCommand(pCommand);

		*pOutput = '\0';

		bool first = true;

		int indent = 0;
		int commandNumber = 0;
		while (true)
		{
			Command* pCommand = commandSource.GetCommand(commandNumber);
			if (pCommand == 0)
			{
				break;
			}
			commandNumber++;

			if (!first)
			{
				MyCat(pOutput, "\n");
			}
			first = false;

			indent += GetIndentPre(pCommand);
			for (int i = 0; i < indent * 2; i++)
			{
				MyCat(pOutput, " ");
			}
			char* pString = pCommand->GetString();
			//if (strlen(pString) != 0)
			{
				MyCat(pOutput, pString);
			}
			//Serial.println(pString);

			indent += GetIndentPost(pCommand);
		}
		//Serial.println("<PrettyFormat");
	}
};

const char* CommandFormatter::preStrings[] = { "endif", "endfor", "endfunc", "elseif", "else" };
const char* CommandFormatter::postStrings[] = { "if", "for", "func", "elseif", "else" };