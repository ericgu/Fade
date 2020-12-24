class EnvironmentClass
{
public:
    int VectorItemDataPoolCount = 2048;         // Total number of vector items that can exist at any one time.
    int VariableStoreChunkSize = 64;            // Size of variable store chunks

    int DebugLogStatements = 0;
    int DebugLogHeapFreeOnAllocation = 0;
};

EnvironmentClass Environment;