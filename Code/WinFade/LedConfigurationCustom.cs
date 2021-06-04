namespace WinFade
{
    class LedConfigurationCustom : LedConfiguration
    {
        internal override void PopulateAndEnableForGroupType(EditLedSetup editLedSetup)
        {
            editLedSetup.c_radioButtonCustom.Checked = true;
        }
    }
}
