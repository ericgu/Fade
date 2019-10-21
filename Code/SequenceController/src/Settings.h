class Settings
{
    const char* _execOnStartupKeyName = "ExecOnStartup";
	const char* _programKeyName = "Program";
    const char* _nodeNameKeyName = "NodeName";

    void GetString(const char* key, char* buffer, int bufferSize)
    {
        String value = NVS.getString(key);
        strcpy(buffer, value.c_str());
    }

    bool SetString(const char* key, const char* value)
    {
        return NVS.setString(key, value, true);
    }

    int GetInt(const char* key)
    {
        return NVS.getInt(key);
    }

    bool SetInt(const char* key, int value)
    {
        return NVS.setInt(key, value, true);
    }

public:
    void Init()
    {
        NVS.begin();
    }

    void LoadProgramText(char* buffer, int bufferSize)
    {
        GetString(_programKeyName, buffer, bufferSize);
    }

    bool SaveProgramText(const char* value)
    {
        return SetString(_programKeyName, value);
    }

    bool LoadShouldExecuteCode()
    {
        return GetInt(_execOnStartupKeyName);
    }

    bool SaveShouldExecuteCode(bool value)
    {
        return SetInt(_execOnStartupKeyName, value);
    }

	void LoadNodeName(char* buffer, int bufferSize)
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