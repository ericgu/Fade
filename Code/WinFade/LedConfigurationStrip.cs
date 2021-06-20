using System;
using System.IO;
using WinFade;

namespace WinFade
{
    class LedConfigurationStrip : LedConfiguration
    {
        private int _spacing;

        public int Spacing
        {
            get { return _spacing; }
            set
            {
                if (value != _spacing)
                {
                    _spacing = value;
                    Create();
                }
            }
        }

        public LedConfigurationStrip()
        {
            _spacing = 50;
        }

        public override void Create()
        {
            _ledSpots.Clear();

            float x = 0;
            float y = 0;

            for (int i = 0; i < LedCount; i++)
            {
                _ledSpots.Add(new LedSpot(x, y));
                x += _spacing;
            }
        }

        internal override void LoadCustom(StreamReader reader)
        {
            Spacing = GetNumberAfterName(reader, "Spacing");
        }

        internal override void SaveCustom(StreamWriter writer)
        {
            writer.WriteLine("Spacing={0}", Spacing);
        }

        internal override void PopulateAndEnableForGroupType(EditLedSetup editLedSetup)
        {
            editLedSetup.c_radioButtonStrip.Checked = true;
            editLedSetup.c_textBoxStripSpacing.Text = Spacing.ToString();
        }

        public void ValuesChanged(EditLedSetup editLedSetup)
        {
            int result;
            if (Int32.TryParse(editLedSetup.c_textBoxStripSpacing.Text, out result))
            {
                Spacing = result;
            }
        }
    }
}

