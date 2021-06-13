class EspFunctions
{
public:
  static int RestartCalled;

  static int GetFreeHeap()
  {
    return 0;
  }

  static void Restart()
  {
    //Serial.println("Restart called!");
    RestartCalled = 1;
  }

};

int EspFunctions::RestartCalled = 0;
