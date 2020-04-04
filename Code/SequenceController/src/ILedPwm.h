class ILedPwm
{
    public:
        virtual void UpdateLed(LedState ledState) = 0;

        virtual void Show() = 0;
};