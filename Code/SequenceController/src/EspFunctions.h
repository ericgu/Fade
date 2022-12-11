#include "esp_heap_caps.h"

class EspFunctions
{
public:
    static int GetFreeHeap()
    {
        return ESP.getFreeHeap();
    }

    static int GetHeapCapsFreeSize()
    {
        return heap_caps_get_free_size(MALLOC_CAP_8BIT);
    }

    static int GetHeapCapsLargestFreeBlock()
    {
        return heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
    }

    static void Restart()
    {
        esp_restart();
    }
};