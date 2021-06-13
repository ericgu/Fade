using System;
using Microsoft.Win32;

namespace WinFade
{
  class ProgramSettings
  {
    private readonly RegistryKey _key;


    public ProgramSettings()
    {
      _key = Registry.CurrentUser.CreateSubKey(@"SOFTWARE\WinFade");
    }


    public string Filename
    {
        get
        {
            if (_key != null)
            {
                return (string)_key.GetValue("Filename");
            }
            else
            {
                return String.Empty;
            }
        }
        set
        {
            _key.SetValue("Filename", value);
        }
    }

    public string Esp32Address
    {
      get
      {
        if (_key != null)
        {
          return (string) _key.GetValue("Esp32Address");
        }
        else
        {
          return String.Empty;
        }
      }
      set
      {
        _key.SetValue("Esp32Address", value);
      }
    }
  }
}
