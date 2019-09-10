class LedCommand
{
    public:
    LedCommand(CommandResult commandResult, int cycleCount)
    {
        _commandResult = commandResult;
        _cycleCount = cycleCount;
    }

	CommandResult _commandResult;
    int _cycleCount;
};