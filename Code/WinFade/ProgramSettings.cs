using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
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

    public string ProgramText
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
