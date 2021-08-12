using System.Drawing;
using System.IO;

namespace WinFade
{
    class LedConfigurationPwm : LedConfiguration
    {
        public string ColorMapping { get; set; }

        private float[] _values;

        public LedConfigurationPwm()
        {
            _values = new float[4];
        }

        internal override void LoadCustom(FileLineParser fileLineParser)
        {
            ColorMapping = fileLineParser.GetAfterName("ColorMapping");
        }

        internal override void SaveCustom(StreamWriter writer)
        {
            writer.WriteLine("ColorMapping={0}", ColorMapping);
        }

        public override void Create()
        {
            _ledSpots.Clear();

            for (int i = 0; i < LedCount; i++)
            {
                _ledSpots.Add(new LedSpot(0, 0));
            }
        }

        public void ValuesChanged(EditLedSetup editLedSetup)
        {
            ColorMapping = editLedSetup.c_textBoxPwmColorCoding.Text;

            Create();
        }

        internal override void PopulateAndEnableForGroupType(EditLedSetup editLedSetup)
        {
            editLedSetup.c_radioButtonPwm.Checked = true;

            editLedSetup.c_textBoxPwmColorCoding.Enabled = true;
            editLedSetup.c_textBoxPwmColorCoding.Text = ColorMapping;
        }

        static internal void DisableForGroupType(EditLedSetup editLedSetup)
        {
            editLedSetup.c_textBoxPwmColorCoding.Enabled = false;
        }

        public override void UpdateLedColor(Graphics graphics, int ledNumber, float red, float green, float blue)
        {
            if (_ledSpots.Count == 0 || _ledSpots.Count != LedCount)
            {
                Create();
            }

            LedSpot ledSpot = _ledSpots[ledNumber];

            // store current values, then decode them into a single color to draw...

            if (ledNumber < 4)
            {
                _values[ledNumber] = red; 
            }

            for (int colorMappingIndex = 0; colorMappingIndex < ColorMapping.Length; colorMappingIndex++)
            {
                char mappingCode = ColorMapping[colorMappingIndex];

                switch (mappingCode)
                {
                    case 'R':
                        red = _values[colorMappingIndex];
                        break;

                    case 'G':
                        green = _values[colorMappingIndex];
                        break;

                    case 'B':
                        blue = _values[colorMappingIndex];
                        break;
                }
            }

            // white mapping. Add white components if they are higher than the current values...
            for (int colorMappingIndex = 0; colorMappingIndex < ColorMapping.Length; colorMappingIndex++)
            {
                char mappingCode = ColorMapping[colorMappingIndex];

                float value = _values[colorMappingIndex];

                switch (mappingCode)
                {
                    case 'W':
                        red = value > red ? value : red;
                        green = value > green ? value : green;
                        blue = value > blue ? value : blue;
                        break;
                }
            }

            ledSpot.DrawLed(graphics, LedSize, XOffset, YOffset, red, green, blue);
        }
    }
}
