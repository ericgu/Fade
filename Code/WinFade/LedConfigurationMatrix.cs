using System.IO;

namespace WinFade
{
    class LedConfigurationMatrix: LedConfiguration
    {
        public int XCount { get; set; }
        public int YCount { get; set; }
        public int XSpacing { get; set; }
        public int YSpacing { get; set; }

        internal override void LoadCustom(StreamReader reader)
        {
            XCount = GetNumberAfterName(reader, "XCount");
            YCount = GetNumberAfterName(reader, "YCount");
            XSpacing = GetNumberAfterName(reader, "XSpacing");
            YSpacing = GetNumberAfterName(reader, "YSpacing");
        }

        internal override void SaveCustom(StreamWriter writer)
        {
            if (XSpacing == 0)
            {
                XSpacing = LedSize;
            }

            if (YSpacing == 0)
            {
                YSpacing = LedSize;
            }

            writer.WriteLine("XCount={0}", XCount);
            writer.WriteLine("YCount={0}", YCount);
            writer.WriteLine("XSpacing={0}", XSpacing);
            writer.WriteLine("YSpacing={0}", YSpacing);
        }

        public override void Create()
        {
            _ledSpots.Clear();

            float yPosition = 200;
            for (int yNumber = 0; yNumber < YCount; yNumber++)
            {
                float xPosition = 200;

                for (int xNumber = 0; xNumber < XCount; xNumber++)
                {
                    _ledSpots.Add(new LedSpot(xPosition, yPosition));

                    xPosition += XSpacing;
                }

                yPosition += YSpacing;
            }
        }

        public void ValuesChanged(EditLedSetup editLedSetup)
        {
            XCount = EditLedSetup.GetValueFromTextBox(editLedSetup.c_textBoxMatrixXCount);
            YCount = EditLedSetup.GetValueFromTextBox(editLedSetup.c_textBoxMatrixYCount);
            XSpacing = EditLedSetup.GetValueFromTextBox(editLedSetup.c_textBoxMatrixXSpacing);
            YSpacing = EditLedSetup.GetValueFromTextBox(editLedSetup.c_textBoxMatrixYSpacing);

            Create();
        }

        internal override void PopulateAndEnableForGroupType(EditLedSetup editLedSetup)
        {
            editLedSetup.c_radioButtonMatrix.Checked = true;

            editLedSetup.c_textBoxMatrixXCount.Text = XCount.ToString();
            editLedSetup.c_textBoxMatrixYCount.Text = YCount.ToString();
            editLedSetup.c_textBoxMatrixXSpacing.Text = XSpacing.ToString();
            editLedSetup.c_textBoxMatrixYSpacing.Text = YSpacing.ToString();
        }
    }
}
