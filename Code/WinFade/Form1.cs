using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Threading;
using System.Windows.Forms;



namespace WinFade
{
    public partial class Form1 : Form
    {
        private delegate void CreateLedDeviceDelegate(string text);

        private delegate void UpdateLedDelegate(int ledGroupNumber, int ledNumber, float red, float green, float blue);

        private delegate void ParseErrorDelegate(string message, int lineNumber);

        private Thread _fadeThread;
        private int _outputLineCount;
        private string _programText;

        readonly ProgramSettings _programSettings;

        private readonly LedForm _ledForm;

        private List<string> _configFileList;

        private LedDevices _ledDevices;

        public Form1()
        {
            InitializeComponent();

            _ledForm = new LedForm();
            _ledForm.Closing += (sender, args) =>
            {
                args.Cancel = true;
                c_buttonAbort_Click(null, null);
            };

            _programSettings = new ProgramSettings();
            c_textBoxProgramText.Text = _programSettings.ProgramText;

            c_labelErrorTag.Visible = false;
            c_labelErrorText.Visible = false;

            _fadeThread = null;

            _ledDevices = new LedDevices();

            FormClosing += (sender, args) =>
            {
                if (_fadeThread != null)
                {
                    _fadeThread.Abort();
                }
            };

            Activated += (sender, args) =>
            {
                c_comboBoxLedArrangements.Items.Clear();
                foreach (string arrangementName in LedGroup.ConfigFileList)
                {
                    c_comboBoxLedArrangements.Items.Add(arrangementName);
                }
                //_pictureBoxManager.Render();
            };

            Closing += (sender, args) => { c_buttonAbort_Click(null, null); };


        }

        private void NewTextAvailable(string text)
        {
            if (c_listBoxSerialOutput.InvokeRequired)
            {
                var d = new CreateLedDeviceDelegate(NewTextAvailable);
                c_listBoxSerialOutput.Invoke(d, new object[] {text});
            }
            else
            {
                c_listBoxSerialOutput.Items.Add(text);
                _outputLineCount++;
                c_labelOutputLineCount.Text = _outputLineCount.ToString();
            }
        }

        private void CreateNewLedDevice(string ledType, int ledCount, int p1, int p2, int p3, int p4)
        {
            _ledDevices.AddNewDevice(ledType, ledCount, p1, p2, p3, p4);

            //int k = 12;

        }



        private void LedUpdated(int ledGroupNumber, int channel, int cycleCount, int brightnessCount, float brightness1, float brightness2,
            float brightness3, float brightness4)
        {
            var d = new UpdateLedDelegate(_ledForm.UpdateLedColor);
            _ledForm.Invoke(d, new object[] {ledGroupNumber, channel, brightness1, brightness2, brightness3});
        }

        private void LedUpdateCycleDone()
        {
            _ledForm.Render();
        }

        private void HighlightLine(int lineNumber)
        {
            if (lineNumber == -1)
            {
                return;
            }

            string[] lines = c_textBoxProgramText.Text.Split('\n');

            int offset = 0;
            int line = 0;
            while (line < lineNumber)
            {
                offset += lines[line].Length + 1;
                line++;
            }

            c_textBoxProgramText.SelectionStart = offset;
            c_textBoxProgramText.SelectionLength = lines[line].Length;
            c_textBoxProgramText.Focus();
        }

        private void ShowErrorOutline(bool visible)
        {
            int margin = 5;

            using (Graphics graphics = this.CreateGraphics())
            {
                Rectangle rectangle = new Rectangle(c_textBoxProgramText.Location.X, c_textBoxProgramText.Location.Y,
                    c_textBoxProgramText.Width, c_textBoxProgramText.Height);
                rectangle.X -= margin;
                rectangle.Y -= margin;
                rectangle.Width += margin * 2;
                rectangle.Height += margin * 2;

                Brush brush = visible ? new SolidBrush(Color.Red) : new SolidBrush(this.BackColor);
                Pen pen = new Pen(brush);
                pen.Width = 10;

                graphics.DrawRectangle(pen, rectangle);

                pen.Dispose();
                brush.Dispose();
            }

        }

        private void ParseErrorEncountered(string message, int lineNumber)
        {
            if (c_textBoxProgramText.InvokeRequired)
            {
                var d = new ParseErrorDelegate(ParseErrorEncountered);
                c_textBoxProgramText.Invoke(d, new object[] {message, lineNumber});
            }
            else
            {
                HighlightLine(lineNumber);
                c_labelErrorTag.Visible = true;
                c_labelErrorText.Visible = true;
                c_labelErrorText.Text = message;

                ShowErrorOutline(true);
            }
        }

        private void DoFade()
        {
            FadeLibrary.Run(_programText, NewTextAvailable, CreateNewLedDevice, LedUpdated, LedUpdateCycleDone,
                ParseErrorEncountered);
            //FadeLibrary.Run(_programText, null, null, null, null);
        }

        private void RunProgram(object sender, EventArgs e)
        {
            if (_fadeThread != null)
            {
                _fadeThread.Abort();
            }

            c_labelErrorTag.Visible = false;
            c_labelErrorText.Visible = false;

            c_listBoxSerialOutput.Items.Clear();
            _outputLineCount = 0;
            _programText = c_textBoxProgramText.Text;
            _programSettings.ProgramText = _programText;
            ShowErrorOutline(false);
            _ledDevices.Reset();

            _ledForm.Show();

            _fadeThread = new Thread(DoFade);
            _fadeThread.Start();
        }

        private void c_buttonAbort_Click(object sender, EventArgs e)
        {
            _ledForm.Hide();

            if (_fadeThread != null)
            {
                _fadeThread.Abort();

                _fadeThread = null;
            }
        }

        private void c_buttonDrawTest_Click(object sender, EventArgs e)
        {
            _ledForm.Show();
            Stopwatch stopwatch = new Stopwatch();
            stopwatch.Start();
            int cycles = 0;

            for (float brightness = 0; brightness < 1.0; brightness += 0.01F)
            {
                DrawAllLeds(brightness);
                _ledForm.Render();
                cycles++;
            }

            for (float brightness = 1.0F; brightness > 0.0; brightness -= 0.01F)
            {
                DrawAllLeds(brightness);
                _ledForm.Render();
                cycles++;
            }

            stopwatch.Stop();
            _ledForm.Hide();

        }

        private void DrawAllLeds(float brightness)
        {
            for (int channel = 0; channel < 32; channel++)
            {
                LedUpdated(0, channel, 1, 3, brightness, brightness, 0, 0);
            }
        }

        private void c_comboBoxLedArrangements_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
