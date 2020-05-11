class VariableData
{
    static const int ValuesPerVariable = 4;
    static const int MaxVariableNameLength = 64;
    static const int MaxStringVariableLength = 128;

public:
    char _variableName[MaxVariableNameLength];
    char _stringValue[MaxStringVariableLength];
    float _value[ValuesPerVariable];
    int _valueCount;
    int _stackLevel;
    int _referenceCount;
};