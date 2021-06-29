using System;
using System.IO;

namespace WinFade
{
    class LedConfigurationRing : LedConfiguration
    {
        private int _ringRadius;

        public int RingRadius
        {
            get { return _ringRadius; }
            set
            {
                if (value != _ringRadius)
                {
                    _ringRadius = value;
                    Create();
                }
            }
        }

        private bool _reversed;

        public bool Reversed
        {
            get { return _reversed; }
            set
            {
                _reversed = value;
                Create();
            }
        }

        internal override void LoadCustom(FileLineParser fileLineParser)
        {
            RingRadius = fileLineParser.GetNumberAfterName("RingRadius");
            bool result;
            Boolean.TryParse(fileLineParser.ReadLine(), out result);
            Reversed = result;
        }

        internal override void SaveCustom(StreamWriter writer)
        {
            writer.WriteLine("RingRadius={0}", RingRadius);
            writer.WriteLine("Reversed={0}", Reversed);
        }

        public override void Create()
        {
            _ledSpots.Clear();

            float xCenter = 200;
            float yCenter = 200;

            double angleIncrement = (2 * Math.PI) / LedCount;
            if (_reversed)
            {
                angleIncrement = -angleIncrement;
            }

            double angle = 0;

            for (int i = 0; i < LedCount; i++)
            {
                float x = (float) (RingRadius * Math.Cos(angle));
                float y = (float) (RingRadius * Math.Sin(angle));

                angle += angleIncrement;

                _ledSpots.Add(new LedSpot(xCenter + x, yCenter + y));
            }
        }

        internal override void PopulateAndEnableForGroupType(EditLedSetup editLedSetup)
        {
            editLedSetup.c_radioButtonRing.Checked = true;
            editLedSetup.c_textBoxRingRadius.Enabled = true;
            editLedSetup.c_textBoxRingRadius.Text = RingRadius.ToString();
            editLedSetup.c_checkBoxReversed.Enabled = true;
            editLedSetup.c_checkBoxReversed.Checked = Reversed;
        }

        internal override void DisableForGroupType(EditLedSetup editLedSetup)
        {
            editLedSetup.c_textBoxRingRadius.Enabled = false;
            editLedSetup.c_checkBoxReversed.Enabled = false;
        }

        public void RingValuesChanged(EditLedSetup editLedSetup)
        {
            int result;
            if (Int32.TryParse(editLedSetup.c_textBoxRingRadius.Text, out result))
            {
                RingRadius = result;
                editLedSetup.PopulateLedConfigurationList();
            }

            Reversed = editLedSetup.c_checkBoxReversed.Checked;
        }
    }
}
