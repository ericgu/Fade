using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WinFade
{
    class LedGroups
    {
        private List<LedGroup> _ledGroups;

        public LedGroups()
        {
            _ledGroups = new List<LedGroup>();
            _ledGroups.Add(new LedGroup(0));
            _ledGroups.Add(new LedGroup(1));
        }

        public void UpdateLedColor(Graphics graphics, int ledGroupNumber, int ledNumber, float red, float green, float blue)
        {
            LedGroup ledGroup = _ledGroups[ledGroupNumber];

            ledGroup.UpdateLedColor(graphics, ledNumber, red, green, blue);
        }
    }
}
