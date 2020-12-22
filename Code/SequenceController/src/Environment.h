class EnvironmentClass
{
public:
    int VectorItemDataPoolCount = 1024;         // Total number of vector items that can exist at any one time. 

    int DebugLogStatements = 0;
    int DebugLogHeapFreeOnAllocation = 0;
};

EnvironmentClass Environment;