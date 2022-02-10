#include "fs.h"
#include "Spiffs.h"

class Settings
{
    const char *_execOnStartupKeyName = "ExecOnStartup";
    const char *_programKeyName = "Program";
    const char *_networkKeyName = "Network";
    const char *_nodeNameKeyName = "NodeName";

    void GetString(const char *key, char *buffer, int bufferSize)
    {
        String value = NVS.getString(key);
        SafeString::StringCopy(buffer, value.c_str(), bufferSize);
    }

    bool SetString(const char *key, const char *value)
    {
        return NVS.setString(key, value, true);
    }

    int GetInt(const char *key)
    {
        return NVS.getInt(key);
    }

    bool SetInt(const char *key, int value)
    {
        return NVS.setInt(key, value, true);
    }

public:
    void Init()
    {
        NVS.begin();
        Serial.println("Spiffs begin true");
        SPIFFS.begin(true);
        Serial.println("Spiffs begin true");
    }

    void LoadProgramText(char *buffer, int bufferSize)
    {
        File file = SPIFFS.open("/program", "r");
        if (file)
        {
            Serial.println("Spiffs read");
            uint32_t sizeInBytes = file.size();
            Serial.print("LoadProgramText - file size in bytes: ");
            Serial.println(sizeInBytes);
            size_t bytesRead = file.readBytes(buffer, bufferSize);
            Serial.println(bytesRead);
            file.close();
            Serial.println("Spiffs read done");
            if (bytesRead != 0)
            {
                return;
            }
        }

        GetString(_programKeyName, buffer, bufferSize);
    }

    bool SaveProgramText(const char *value)
    {
        File file = SPIFFS.open("/program", "w");
        if (file)
        {
            Serial.print("SaveProgramText: ");
            size_t byteswritten = file.write((const uint8_t *)value, strlen(value) + 1);
            Serial.println(byteswritten);
            file.close();
            return true;
        }

        return false;
        //return SetString(_programKeyName, value);
    }

    void LoadNetworkName(char *buffer, int bufferSize)
    {
        GetString(_networkKeyName, buffer, bufferSize);
    }

    bool SaveNetworkName(const char *value)
    {
        return SetString(_networkKeyName, value);
    }

    bool LoadShouldExecuteCode()
    {
        return GetInt(_execOnStartupKeyName);
    }

    bool SaveShouldExecuteCode(bool value)
    {
        return SetInt(_execOnStartupKeyName, value);
    }

    void LoadNodeName(char *buffer, int bufferSize)
    {
        GetString(_nodeNameKeyName, buffer, bufferSize);
    }

    bool SaveNodeName(const char *nodeName)
    {
        return SetString(_nodeNameKeyName, nodeName);
    }

    void EraseAll()
    {
        NVS.eraseAll();
    }
};