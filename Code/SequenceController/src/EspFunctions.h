class EspFunctions
{
public:
    static int GetFreeHeap()
    {
        return ESP.getFreeHeap();
    }

    static void restart()
    {
        esp_restart();
    }
};