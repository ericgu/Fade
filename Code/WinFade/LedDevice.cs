using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFade
{
    class LedDevice
    {
        private int _ledGroupNumber;
        private string _ledType;
        private int _ledCount;

        public LedDevice(int ledGroupNumber, string ledType, int ledCount, int p1, int p2, int p3, int p4)
        {
            _ledGroupNumber = ledGroupNumber;
            _ledType = ledType;
            _ledCount = ledCount;
        }

    }
}
