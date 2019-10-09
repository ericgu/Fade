class Settings
{
public:
    Settings()
    {
        NVS.begin();
    }

    String GetString(const char* key)
    {
        return NVS.getString(key);
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

    void EraseAll()
    {
        NVS.eraseAll();
    }
};