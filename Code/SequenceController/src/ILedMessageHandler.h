class ILedMessageHandler
{
    public:

	virtual void ExecuteLedCommandMember(CommandResult* pCommandResult) = 0;

	virtual void Configure(const char* pLedType, int ledTypeCount, int ledCount) = 0;
};