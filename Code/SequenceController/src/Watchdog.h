// Watchdog is used by the system to determine whether it should run the stored code or not.

// It stores/implements the following events and behavior:

// Event 1: Program updated
// When the program is updated, the program is stored to non-volatile memory and the ExecuteOnStartup flag is set to false,
// because the program has not been proven to be functional. The running program is replaced with the new program.

// Event 2: Program is functional
// this is determined by it passing a given number of clock ticks (currently, 500 or 5 seconds). At this point, the ExecuteOnStartup flag is updated to true and stored.

// Event 3: Restart
// The ExecuteOnStartup flag is loaded and controls whether the controller will execute the loaded program.

class Watchdog
{
    Settings* _pSettings;
    bool _shouldExecuteCode;
    bool _shouldExecuteCodeLoaded;
    int _executionCount = 0;
    const char* _execOnStartupKeyName = "ExecOnStartup";

public:
    void Initialize(Settings* pSettings)
    {
        _pSettings = pSettings;

        _shouldExecuteCode = _pSettings->GetInt(_execOnStartupKeyName);
        _shouldExecuteCodeLoaded = _shouldExecuteCode;
        Serial.print("ExecOnStartup Load = "); Serial.println(_shouldExecuteCode);
    }

    void ProgramUpdated()
    {
        bool saved = _pSettings->SetInt(_execOnStartupKeyName, 0);
        Serial.print("Status = "); Serial.println(saved);
        _shouldExecuteCode = true;
    }

    bool ShouldExecuteCode()
    {
        return _shouldExecuteCode;
    }

    void ExecutionTick()
    {
        _executionCount++;

        if (_executionCount == 500 && !_shouldExecuteCodeLoaded)
        {
            Serial.println("ExecOnStartup = 1");
            bool saved = _pSettings->SetInt(_execOnStartupKeyName, 1);
            Serial.print("Status = "); Serial.println(saved);
        }
    }

    void StopExecution()
    {
        _shouldExecuteCode = false;
    }
};