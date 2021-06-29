namespace WinFade
{
    class LedConfigurationCustom : LedConfiguration
    {
        internal override void PopulateAndEnableForGroupType(EditLedSetup editLedSetup)
        {
            editLedSetup.c_radioButtonCustom.Checked = true;
            editLedSetup.c_buttonCustomPaste.Enabled = true;
        }

        internal override void DisableForGroupType(EditLedSetup editLedSetup)
        {
            editLedSetup.c_buttonCustomPaste.Enabled = false;
        }
    }
}
