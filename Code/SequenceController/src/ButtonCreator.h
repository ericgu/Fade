class ButtonCreator : public IButtonCreator
{
public:
    virtual IButton *Create(const char *pButtonType, int pin, int parameter1)
    {
        if (strcmp(pButtonType, "TOUCH") == 0)
        {
            return new TouchButton(pin, parameter1);
        }
        else if (strcmp(pButtonType, "ACTIVELOW") == 0)
        {
            return new Button(pin, parameter1);
        }
        else if (strcmp(pButtonType, "ACTIVEHIGH") == 0)
        {
            return new Button(pin, parameter1);
        }

        return 0;
    }
};