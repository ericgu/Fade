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

        public string ProgramText2
    {
      get
      {
        if (_key != null)
        {
          return (string) _key.GetValue("ProgramText");
        }
        else
        {
          return String.Empty;
        }
      }
      set
      {
        _key.SetValue("ProgramText", value);
      }
    }
  }
}
