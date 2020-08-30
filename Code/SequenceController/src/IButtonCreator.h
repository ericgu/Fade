class IButtonCreator
{
public:
    virtual IButton *Create(const char *pButtonType, int pin, int parameter1) = 0;
};