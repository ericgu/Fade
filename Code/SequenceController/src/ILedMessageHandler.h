class ILedMessageHandler
{
    public:

	virtual void ExecuteLedCommandMember(CommandResult* pCommandResult) = 0;

	virtual void Configure(int ledGroupNumber, const char *pLedType, int ledCount, int pin1, int pin2, int pin3, int pin4) = 0;
};