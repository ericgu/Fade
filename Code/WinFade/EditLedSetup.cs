using System;
using System.Windows.Forms;

namespace WinFade
{
    public partial class EditLedSetup : Form
    {
        private readonly LedTestBoard _ledTestBoard;

        public EditLedSetup(LedTestBoard ledTestBoard)
        {
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

            if (selectedIndex == -1 && _ledTestBoard.LedConfigurations.Count != 0)
            {
              selectedIndex = 0;
            }

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

        // update the text in the listbox. 
        internal void UpdateLedConfigurationList()
        {
            for (int item = 0; item < _ledTestBoard.LedConfigurations.Count; item++)
            {
                c_listBoxLedGroups.Items[item] = _ledTestBoard.LedConfigurations[item].ToString();
            }
        }

        LedConfiguration SelectedLedConfiguration
        {
            get
            {
                int selected = c_listBoxLedGroups.SelectedIndex;
                if (selected == -1)
                {
                  return null;
                }

                return _ledTestBoard.LedConfigurations[selected];
            }
        }

        private void DisableForGroupTypes()
        {
            if (!(SelectedLedConfiguration is LedConfigurationCustom))
            {
                LedConfigurationCustom.DisableForGroupType(this);
            }

            if (!(SelectedLedConfiguration is LedConfigurationMatrix))
            {
                LedConfigurationMatrix.DisableForGroupType(this);
            }

            if (!(SelectedLedConfiguration is LedConfigurationPwm))
            {
                LedConfigurationPwm.DisableForGroupType(this);
            }

            if (!(SelectedLedConfiguration is LedConfigurationRing))
            {
                LedConfigurationRing.DisableForGroupType(this);
            }

            if (!(SelectedLedConfiguration is LedConfigurationStrip))
            {
              LedConfigurationStrip.DisableForGroupType(this);
            }

            if (!(SelectedLedConfiguration is LedConfigurationServo))
            {
              LedConfigurationServo.DisableForGroupType(this);
            }
    }

        private void c_listBoxLedGroups_SelectedIndexChanged(object sender, EventArgs e)
        {
          if (SelectedLedConfiguration == null)
          {
            return;
          }

            DisableForGroupTypes();

            c_textBoxGroupNumber.Text = SelectedLedConfiguration.GroupNumber.ToString();
            c_textBoxLedCount.Text = SelectedLedConfiguration.LedCount.ToString();

            c_textBoxX.Text = SelectedLedConfiguration.XOffset.ToString();
            c_textBoxY.Text = SelectedLedConfiguration.YOffset.ToString();
            c_textBoxLedSize.Text = SelectedLedConfiguration.LedSize.ToString();

            c_textBoxRingRadius.Enabled = false;

            SelectedLedConfiguration.PopulateAndEnableForGroupType(this);
        }

        private void MainValuesChanged(object sender, EventArgs e)
        {
            SelectedLedConfiguration.GroupNumber = GetValueFromTextBox(c_textBoxGroupNumber);
            SelectedLedConfiguration.LedCount = GetValueFromTextBox(c_textBoxLedCount);
            SelectedLedConfiguration.LedSize = GetValueFromTextBox(c_textBoxLedSize);
            SelectedLedConfiguration.XOffset = GetValueFromTextBox(c_textBoxX);
            SelectedLedConfiguration.YOffset = GetValueFromTextBox(c_textBoxY);

            UpdateLedConfigurationList();
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

            UpdateLedConfigurationList();
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

        private void c_radioButtonServo_Click(object sender, EventArgs e)
        {
          ChangeGroupType(LedGroupType.Servo);
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

    private void label14_Click(object sender, EventArgs e)
    {

    }

    private void ServoAnglesChanged(object sender, EventArgs e)
    {
      ((LedConfigurationServo) SelectedLedConfiguration).ValuesChanged(this);
    }

    private void label16_Click(object sender, EventArgs e)
    {

    }

    private void label15_Click(object sender, EventArgs e)
    {

    }


  }
}
