class ICommandSource
{
    public:
        virtual Command GetNextCommand() = 0;
		virtual void SetCommandToSerialNumber(int serialNumber) = 0;
		virtual void Reset() = 0;
};

class CommandSource: public ICommandSource
{
	int _serialNumber;

	ListParser* _pListParser;


    public:
		CommandSource()
		{
			_pListParser = 0;
		}

		void SetCommand(const char* pCommandString)
		{
			if (_pListParser != 0)
			{
				delete _pListParser;
			}

			_serialNumber = 0;
			_pListParser = new ListParser("\n\r", pCommandString);
			//Serial.println(">CommandSource.SetCommand");
			//Serial.print("This: "); Serial.println((int) this);
			//Serial.println(pCommandString);
			//Serial.println(_pListParser->GetCount());
			//for (int i = 0; i < _pListParser->GetCount(); i++)
			//{
			//	Serial.println(_pListParser->GetItem(i));
			//}
			//Serial.println("<CommandSource.SetCommand");
		}

		void Reset()
		{
			_serialNumber = 0;
		}

		Command GetNextCommand()
		{
			if (_pListParser == 0 || _serialNumber == _pListParser->GetCount())
			{
				return Command(0, -1);
			}

			//Serial.println(">GetNextCommand");
			//Serial.println(_pListParser->GetCount());
			//for (int i = 0; i < _pListParser->GetCount(); i++)
			{
				//if (i == _serialNumber)
				{
					//Serial.print("+");
				}
				//Serial.println(_pListParser->GetItem(i));
			}
			//Serial.println("<GetNextCommand");

			const char *pCommandStart = _pListParser->GetItem(_serialNumber);
			//Serial.print(_serialNumber); Serial.print(": "); Serial.println(pCommandStart);

			while (*pCommandStart == ' ')
			{
				pCommandStart++;
			}

			Command command = Command(pCommandStart, _serialNumber);
			_serialNumber++;

			//Serial.println(command.GetString());

			return command;
		}

		void SetCommandToSerialNumber(int serialNumber)
		{
			_serialNumber = serialNumber;
		}
};