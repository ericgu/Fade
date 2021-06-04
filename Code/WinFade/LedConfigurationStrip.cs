namespace WinFade
{
    class LedConfigurationStrip : LedConfiguration
    {
        public override void Create()
        {
            _ledSpots.Clear();

            float x = 0;
            float y = 0;

            for (int i = 0; i < LedCount; i++)
            {
                _ledSpots.Add(new LedSpot(x, y));
                x += 50;
            }
        }

        internal override void PopulateAndEnableForGroupType(EditLedSetup editLedSetup)
        {
            editLedSetup.c_radioButtonStrip.Checked = true;
        }
    }
}
