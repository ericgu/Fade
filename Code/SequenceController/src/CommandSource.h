class ICommandSource
{
    public:
        virtual Command* GetCommand(int commandNumber) = 0;
};

class CommandSource: public ICommandSource
{
	//int _serialNumber;

	ListParser* _pListParser;
	Command _command;

    public:
		CommandSource()
		{
			_pListParser = 0;
		}

		~CommandSource()
		{
			if (_pListParser != 0)
			{
				delete _pListParser;
			}
			_pListParser = 0;
		}

		void SetCommand(const char* pCommandString)
		{
			if (_pListParser != 0)
			{
				delete _pListParser;
				_pListParser = 0;
			}

			//_serialNumber = 0;
			_pListParser = new ListParser(1022, 1024);
			_pListParser->ParseByLines(pCommandString);
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

		Command* GetCommand(int commandNumber)
		{
			if (_pListParser == 0 || commandNumber >= _pListParser->GetCount())
			{
				return 0;
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

			const char *pCommandStart = _pListParser->GetItem(commandNumber);
			//Serial.print(_serialNumber); Serial.print(": "); Serial.println(pCommandStart);

			while (*pCommandStart == ' ')
			{
				pCommandStart++;
			}

			_command = Command(pCommandStart, commandNumber);

			//Serial.println(command.GetString());

			return &_command;
		}

};