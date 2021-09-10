class ILedMessageHandler
{
public:
	virtual void ExecuteLedCommandMember(CommandResult *pCommandResult) = 0;

	virtual bool Configure(int ledGroupNumber, const char *pLedType, int ledCount, int pinCount, int pins[16]) = 0;

	virtual int GetLedCount() = 0;
};