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

    void SetString(const char* key, const char* value)
    {
        NVS.setString(key, value);
    }

};