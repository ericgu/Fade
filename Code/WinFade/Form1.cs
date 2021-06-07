using System;
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
        //private string _programText;

        readonly ProgramSettings _programSettings;

        private readonly LedForm _ledForm;

        private Project _project;

        private string _filename;

        public Form1()
        {
            InitializeComponent();

            _project = new Project();
            _ledForm = new LedForm(_project.LedTestBoard);
            _ledForm.Closing += (sender, args) =>
            {
                args.Cancel = true;
                Stop();
            };

            _ledForm.ButtonPressed += LedFormOnButtonPressed;

            _programSettings = new ProgramSettings();
            //c_textBoxProgramText.Text = _programSettings.ProgramText;
            //_project.ProgramText = _programSettings.ProgramText;

            _project.Filename = _programSettings.Filename;
            _project.Load();
            SetBanner();
            c_textBoxProgramText.Text = _project.ProgramText;

            c_labelErrorTag.Visible = false;
            c_labelErrorText.Visible = false;

            _fadeThread = null;

            //_ledDevices = new LedDevices();

            FormClosing += (sender, args) =>
            {
                if (_fadeThread != null)
                {
                    Stop();
                }
            };

            Activated += (sender, args) =>
            {
                if (_fadeThread != null)
                {
                    _ledForm.BringToFront();
                }
            };

            Closing += (sender, args) => { Stop(); };


        }

        private void LedFormOnButtonPressed(int buttonnumber)
        {
            FadeLibrary.PressButton(buttonnumber);
        }

        void SetBanner()
        {
            Text = "WinFade - " + _project.Filename;
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

        private void CreateNewLedDevice(int groupNumber, string ledType, int ledCount, int p1, int p2, int p3, int p4)
        {
            //_ledDevices.AddNewDevice(ledType, ledCount, p1, p2, p3, p4);

            _project.LedTestBoard.AddNewDevice(groupNumber, ledType, ledCount, p1, p2, p3, p4);   

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
                Stop();

                HighlightLine(lineNumber);
                c_labelErrorTag.Visible = true;
                c_labelErrorText.Visible = true;
                c_labelErrorText.Text = message;

                ShowErrorOutline(true);
            }
        }

        private void DoFade()
        {
            FadeLibrary.Run(_project.ProgramText, NewTextAvailable, CreateNewLedDevice, LedUpdated, LedUpdateCycleDone,
                ParseErrorEncountered);
            //FadeLibrary.Run(_programText, null, null, null, null);
        }

        private void RunProgram(object sender, EventArgs e)
        {
            if (_fadeThread == null)
            {
                Run();
            }
            else
            {
                Stop();

            }
        }

        private void Run()
        {
            if (_fadeThread != null)
            {
                _fadeThread.Abort();
            }

            c_buttonRunStop.Text = "Stop";

            c_labelErrorTag.Visible = false;
            c_labelErrorText.Visible = false;

            c_listBoxSerialOutput.Items.Clear();
            _outputLineCount = 0;
            //_programText = c_textBoxProgramText.Text;
            //_project.ProgramText = _programText;
            ShowErrorOutline(false);

            _ledForm.Show();

            _fadeThread = new Thread(DoFade);
            _fadeThread.Start();
        }

        private void Stop()
        {
            _ledForm.Hide();
            c_buttonRunStop.Text = "Run";

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



        private void setupButton_Click(object sender, EventArgs e)
        {
            if (_project.LedTestBoard.LedConfigurations.Count != 0)
            {
                EditLedSetup editLedSetup = new EditLedSetup(_project.LedTestBoard);

                DialogResult result = editLedSetup.ShowDialog();
            }
        }
        
        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.InitialDirectory = Project.BasePath;
            openFileDialog.DefaultExt = "fade";
            openFileDialog.Filter = "fade files (*.fade)";
            openFileDialog.FilterIndex = 0;
            openFileDialog.CheckFileExists = true;
            openFileDialog.CheckPathExists = false;

            DialogResult result = openFileDialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                _filename = openFileDialog.FileName;
                _programSettings.Filename = _filename;
                _project.Filename = _filename;
                _project.Load();
                SetBanner();
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(_project.Filename))
            {
                saveAsToolStripMenuItem_Click(sender, e);
            }
            else
            {
                _project.Save();
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDialog = new SaveFileDialog();
            saveFileDialog.InitialDirectory = Project.BasePath;
            saveFileDialog.DefaultExt = "fade";
            saveFileDialog.Filter = "fade files (*.fade)|*.fade";
            saveFileDialog.FilterIndex = 0;
            saveFileDialog.CheckPathExists = false;

            DialogResult result = saveFileDialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                _filename = saveFileDialog.FileName;
                _project.Filename = _filename;
                _project.Save();
                _programSettings.Filename = _filename;
                SetBanner();
            }
        }

        private void c_textBoxProgramText_TextChanged(object sender, EventArgs e)
        {
            _project.ProgramText = c_textBoxProgramText.Text;
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.F5)
            {
                RunProgram(null, null);
                e.Handled = true;
            }
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            _project = new Project();
            SetBanner();
            c_textBoxProgramText.Text = _project.ProgramText;
        }
    }
}
