class EnvironmentClass
{
public:
  int VectorItemDataPoolCount;
  int VariableStoreChunkSize;
  int DebugLogStatements;
  int DebugLogHeapFreeOnAllocation;
  int DebugLogTouchButtonValues;
  int DebugLogCycleDeltas;
  int DebugLogLedUpdates;
  int DebugPrintOverUdp;

  EnvironmentClass()
  {
    ResetValues();
  }

  void ResetValues()
  {
    VectorItemDataPoolCount = 2048; // Total number of vector items that can exist at any one time.
    VariableStoreChunkSize = 64;    // Size of variable store chunks

    DebugLogStatements = 0;
    DebugLogHeapFreeOnAllocation = 0;
    DebugLogTouchButtonValues = 0;
    DebugLogCycleDeltas = 0;
    DebugLogLedUpdates = 0;
    DebugPrintOverUdp = 0;
  }
};

EnvironmentClass Environment;