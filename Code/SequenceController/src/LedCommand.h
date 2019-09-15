class LedCommand
{
    public:
    LedCommand(CommandResult commandResult)
    {
        _commandResult = commandResult;
    }

	CommandResult _commandResult;
};