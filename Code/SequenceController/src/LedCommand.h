class LedCommand
{
    public:
    LedCommand(BrightnessTarget brightnessTarget, int cycleCount)
    {
        _brightnessTarget = brightnessTarget;
        _cycleCount = cycleCount;
    }

    BrightnessTarget _brightnessTarget;
    int _cycleCount;
};