using System;
using System.Windows.Forms;

namespace WinFade
{
    public partial class EditLedSetup : Form
    {
        private readonly LedTestBoard _ledTestBoard;
        private LedConfiguration _currentLedConfiguration;

        public EditLedSetup(LedTestBoard ledTestBoard)
        {
            _currentLedConfiguration = null;

            _ledTestBoard = ledTestBoard;

            InitializeComponent();

            c_listBoxLedGroups.Items.Clear();

            PopulateLedConfigurationList();

            if (ledTestBoard.LedConfigurations.Count != 0)
            {
                c_listBoxLedGroups.SelectedIndex = 0;
            }
        }

        internal void PopulateLedConfigurationList()
        {
            int selectedIndex = c_listBoxLedGroups.SelectedIndex;
            c_listBoxLedGroups.Items.Clear();
            foreach (LedConfiguration ledGroup in _ledTestBoard.LedConfigurations)
            {
                c_listBoxLedGroups.Items.Add(ledGroup.ToString());
            }

            if (selectedIndex < _ledTestBoard.LedConfigurations.Count)
            {
                c_listBoxLedGroups.SelectedIndex = selectedIndex;
            }
        }

        LedConfiguration SelectedLedConfiguration
        {
            get
            {
                int selected = c_listBoxLedGroups.SelectedIndex;

                return _ledTestBoard.LedConfigurations[selected];
            }
        }

        private void c_listBoxLedGroups_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (_currentLedConfiguration != null)
            {
                _currentLedConfiguration.DisableForGroupType(this);
            }

            LedConfiguration ledConfiguration = SelectedLedConfiguration;

            c_textBoxGroupNumber.Text = ledConfiguration.GroupNumber.ToString();
            c_textBoxLedCount.Text = ledConfiguration.LedCount.ToString();

            c_textBoxX.Text = ledConfiguration.XOffset.ToString();
            c_textBoxY.Text = ledConfiguration.YOffset.ToString();
            c_textBoxLedSize.Text = ledConfiguration.LedSize.ToString();

            c_textBoxRingRadius.Enabled = false;

            ledConfiguration.PopulateAndEnableForGroupType(this);
            _currentLedConfiguration = ledConfiguration;
        }

        private void MainValuesChanged(object sender, EventArgs e)
        {
            SelectedLedConfiguration.GroupNumber = GetValueFromTextBox(c_textBoxGroupNumber);
            SelectedLedConfiguration.LedCount = GetValueFromTextBox(c_textBoxLedCount);
            SelectedLedConfiguration.LedSize = GetValueFromTextBox(c_textBoxLedSize);
            SelectedLedConfiguration.XOffset = GetValueFromTextBox(c_textBoxX);
            SelectedLedConfiguration.YOffset = GetValueFromTextBox(c_textBoxY);

            PopulateLedConfigurationList();
        }

        public static int GetValueFromTextBox(TextBox textBox)
        {
            int result;
            Int32.TryParse(textBox.Text, out result);
            return result;
        }

        private void c_radioButtonStrip_Click(object sender, EventArgs e)
        {
            ChangeGroupType(LedGroupType.Strip);
        }

        private void ChangeGroupType(LedGroupType ledGroupType)
        {
            _ledTestBoard.ChangeLedConfigurationToType(c_listBoxLedGroups.SelectedIndex, ledGroupType);
            PopulateLedConfigurationList();
        }

        private void c_radioButtonCustom_Click(object sender, EventArgs e)
        {
            ChangeGroupType(LedGroupType.Custom);
        }

        private void c_buttonCustomPaste_Click(object sender, EventArgs e)
        {
            _ledTestBoard.ChangeLedConfigurationToType(c_listBoxLedGroups.SelectedIndex, LedGroupType.Custom);
            string contents = Clipboard.GetText();
            _ledTestBoard.PasteCustomConfigurationText(c_listBoxLedGroups.SelectedIndex, contents);

            PopulateLedConfigurationList();
        }

        private void c_radioButtonRing_Click(object sender, EventArgs e)
        {
            ChangeGroupType(LedGroupType.Ring);
        }

        private void c_radioButtonMatrix_Click(object sender, EventArgs e)
        {
            ChangeGroupType(LedGroupType.Matrix);
        }

        private void c_radioButtonPwm_Click(object sender, EventArgs e)
        {
            ChangeGroupType(LedGroupType.Pwm);
        }

        private void c_buttonDeleteClick(object sender, EventArgs e)
        {
            if (c_listBoxLedGroups.SelectedIndex != -1)
            {
                int newSelectedIndex = c_listBoxLedGroups.SelectedIndex;
                _ledTestBoard.LedConfigurations.RemoveAt(c_listBoxLedGroups.SelectedIndex);
                PopulateLedConfigurationList();

                if (newSelectedIndex == _ledTestBoard.LedConfigurations.Count)
                {
                    newSelectedIndex--;
                }

                c_listBoxLedGroups.SelectedIndex = newSelectedIndex;
            }
        }

        private void c_buttonAdd_Click(object sender, EventArgs e)
        {
            _ledTestBoard.LedConfigurations.Add(new LedConfigurationStrip());
            PopulateLedConfigurationList();
        }

        private void c_buttonUp_Click(object sender, EventArgs e)
        {
            if (c_listBoxLedGroups.SelectedIndex > 0)
            {
                LedConfiguration ledConfiguration = _ledTestBoard.LedConfigurations[c_listBoxLedGroups.SelectedIndex];
                _ledTestBoard.LedConfigurations.RemoveAt(c_listBoxLedGroups.SelectedIndex);
                _ledTestBoard.LedConfigurations.Insert(c_listBoxLedGroups.SelectedIndex - 1, ledConfiguration);
                c_listBoxLedGroups.SelectedIndex--;
                PopulateLedConfigurationList();
            }
        }

        private void c_buttonDown_Click(object sender, EventArgs e)
        {
            if (c_listBoxLedGroups.SelectedIndex < _ledTestBoard.LedConfigurations.Count - 1)
            {
                LedConfiguration ledConfiguration = _ledTestBoard.LedConfigurations[c_listBoxLedGroups.SelectedIndex];
                _ledTestBoard.LedConfigurations.RemoveAt(c_listBoxLedGroups.SelectedIndex);
                _ledTestBoard.LedConfigurations.Insert(c_listBoxLedGroups.SelectedIndex + 1, ledConfiguration);
                c_listBoxLedGroups.SelectedIndex++;
                PopulateLedConfigurationList();
            }
        }

        private void RingValuesChanged(object sender, EventArgs e)
        {
            ((LedConfigurationRing)this.SelectedLedConfiguration).RingValuesChanged(this);
        }

        private void c_textBoxMatrixControl_Changed(object sender, EventArgs e)
        {
            ((LedConfigurationMatrix)SelectedLedConfiguration).ValuesChanged(this);
        }

        private void c_textBoxPwmColorCoding_Leave(object sender, EventArgs e)
        {
            ((LedConfigurationPwm)SelectedLedConfiguration).ValuesChanged(this);
        }

        private void c_textBoxStripSpacing_TextChanged(object sender, EventArgs e)
        {
            ((LedConfigurationStrip)SelectedLedConfiguration).ValuesChanged(this);
        }
    }
}
