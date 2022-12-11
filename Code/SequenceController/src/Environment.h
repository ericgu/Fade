class EnvironmentClass
{
public:
  int VectorItemChunkSize;
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
    VectorItemChunkSize = 512;      // The size of each vector item chunk. 
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