using System;
using System.Collections.Generic;
using System.Linq;
using System.Resources;
using System.Text;
using System.Threading.Tasks;

namespace WinFade
{
    class LedDevices
    {
        private List<LedDevice> _ledDevices;
        private int _nextLedGroupNumber;

        public LedDevices()
        {
            _ledDevices = new List<LedDevice>();
        }

        public void Reset()
        {
            _ledDevices.Clear();
            _nextLedGroupNumber = 0;
        }

       public void AddNewDevice(string ledType, int ledCount, int pin1, int pin2, int pin3, int pin4)
        {
            _ledDevices.Add(new LedDevice(_nextLedGroupNumber, ledType, ledCount, pin1, pin2, pin3, pin4));
        }
    }
}
