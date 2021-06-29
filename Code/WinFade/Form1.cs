using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Net;
using System.Runtime.ExceptionServices;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using Timer = System.Threading.Timer;


namespace WinFade
{
    public partial class Form1 : Form
    {
        private delegate void CreateLedDeviceDelegate(string text);

        private delegate void UpdateLedDelegate(int ledGroupNumber, int ledNumber, float red, float green, float blue);

        private delegate void ParseErrorDelegate(string message, int lineNumber);

        private delegate void FatalErrorDelegate();

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

            _programSettings = new ProgramSettings();

            _project = new Project();
            _project.Filename = _programSettings.Filename;
            _project.Load();

            _ledForm = new LedForm(_project.LedTestBoard);
            _ledForm.Closing += (sender, args) =>
            {
                args.Cancel = true;
                Stop();
            };

            _ledForm.ButtonPressed += LedFormOnButtonPressed;

            SetBanner();
            c_textBoxProgramText.Text = _project.ProgramText;

            c_textBoxIPAddress.Text = _programSettings.Esp32Address;

            c_labelErrorTag.Visible = false;
            c_labelErrorText.Visible = false;

            _fadeThread = null;

            Icon = new Icon(@"..\..\Fade Icon 64x64.ico");


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

            System.AppDomain.CurrentDomain.UnhandledException += CurrentDomainOnUnhandledException;
        }

        private void CurrentDomainOnUnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            throw new NotImplementedException();
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
            var d = new CreateLedDeviceDelegate(AddOutputToListbox);
            c_listBoxSerialOutput.Invoke(d, new object[] {text});
        }

        private void AddOutputToListbox(string text)
        {
            if (c_listBoxSerialOutput.Items.Count == 0)
            {
                c_listBoxSerialOutput.Items.Add("");
            }

            c_listBoxSerialOutput.Items[c_listBoxSerialOutput.Items.Count - 1] += text;

            if (text.EndsWith("\n"))
            {
                c_listBoxSerialOutput.Items.Add("");
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



        private void LedUpdated(int ledGroupNumber, int channel, int brightnessCount, float brightness1, float brightness2,
            float brightness3, float brightness4)
        {
            _ledForm.UpdateLedColor(ledGroupNumber, channel, brightness1, brightness2, brightness3);
//            var d = new UpdateLedDelegate(_ledForm.UpdateLedColor);
//            _ledForm.Invoke(d, new object[] {ledGroupNumber, channel, brightness1, brightness2, brightness3});
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

        private void FatalError()
        {
            NewTextAvailable("Fatal error in animation system.");

            Stop();
        }


        [HandleProcessCorruptedStateExceptions]
        private void DoFade()
        {
            try
            {
                FadeLibrary.Run(_project.ProgramText, NewTextAvailable, CreateNewLedDevice, LedUpdated,
                    LedUpdateCycleDone,
                    ParseErrorEncountered);
            }
            catch (ThreadAbortException)
            {
                return;
            }
            catch (Exception)
            {

                var d = new FatalErrorDelegate(FatalError);
                c_textBoxProgramText.Invoke(d);
            }
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
            //_fadeThread.
            try
            {
                _fadeThread.Start();
            }
            catch (Exception)
            {
            }
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
                LedUpdated(0, channel, 3, brightness, brightness, 0, 0);
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
            openFileDialog.Filter = "fade files (*.fade)|*.fade";
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

        private void c_buttonUpload_Click(object sender, EventArgs e)
        {
            c_labelWorking.Text = "Working...";

            WebRequest req = WebRequest.Create("http://" + c_textBoxIPAddress.Text);
            string postData = "Program=" + _project.ProgramText;

            byte[] send = Encoding.Default.GetBytes(postData);
            req.Method = "POST";
            req.ContentType = "application/x-www-form-urlencoded";
            req.ContentLength = send.Length;

            try
            {
                Stream sout = req.GetRequestStream();
                sout.Write(send, 0, send.Length);
                sout.Flush();
                sout.Close();

                WebResponse res = req.GetResponse();
                StreamReader sr = new StreamReader(res.GetResponseStream());
                string returnvalue = sr.ReadToEnd();
                c_labelWorking.Text = "Done";
            }
            catch (Exception exception)
            {
                MessageBox.Show(exception.Message);
                c_labelWorking.Text = "Error";
            }

        }

        private void c_textBoxIPAddress_TextChanged(object sender, EventArgs e)
        {
            _programSettings.Esp32Address = c_textBoxIPAddress.Text;
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private Point _textBoxMouseLocation;

        private void c_textBoxProgramText_MouseMove(object sender, MouseEventArgs e)
        {
            _textBoxMouseLocation = e.Location;

            c_timerProgramTextHover.Stop();
            c_timerProgramTextHover.Enabled = true;
            c_timerProgramTextHover.Interval = 500;
            c_timerProgramTextHover.Start();
        }

        private void c_timerProgramTextHover_Tick(object sender, EventArgs e)
        {
            DisableMouseHoverTimer();
            int index = c_textBoxProgramText.GetCharIndexFromPosition(_textBoxMouseLocation);

            int start = index;
            char current = _project.ProgramText[start];
            while (IsIdentifier(current))
            {
                start--;
                if (start == -1)
                {
                    break;
                }
                current = _project.ProgramText[start];
            }

            int end = index;
            current = _project.ProgramText[end];
            while (IsIdentifier(current))
            {
                end++;
                current = _project.ProgramText[end];
            }

            try
            {
                string identifier = _project.ProgramText.Substring(start + 1, end - start - 1);

                string helpText = GetHelpText(identifier);

                if (helpText != null)
                {
                    c_labelErrorText.Text = helpText;
                    c_labelErrorText.Visible = true;
                }
                else
                {
                    c_labelErrorText.Text = String.Empty;
                }

                Debug.WriteLine(index.ToString() + " " + identifier);
            }
            catch (Exception)
            {
                c_labelErrorText.Text = String.Empty;
            }
        }

        private static bool IsIdentifier(char current)
        {
            return (current >= 'A' && current <= 'Z') ||
                   (current >= 'a' && current <= 'z') ||
                   (current >= '0' && current <= '9') ||
                   (current == '_');
        }

        private string GetHelpText(string word)
        {
            switch (word)
            {
                case "A":
                    return "A(cycles) - animate the currently-defined fades for a given number of cycles";

                case "ConfigButton":
                    return "ConfigButton(<buttonNumber>, <buttonType>, <buttonPin>, <params>) - configure a button";

                case "ConfigLed":
                    return "ConfigLed(<ledStringNumber>, <ledStringType>, <ledCount>, <params>) - configure an LED string";

                case "D":
                    return "D(<cycleCount>, <ledIndex>, <targetBrightness>) - define a fade for a specific LED to a specific color over a number of cycles";

                case "Di":
                    return "Di(<cycleCount>, <ledIndex>, <targetBrightness>) - define and execute a fade for a specific LED to a specific color over a number of cycles";

                case "Debug":
                    return "Debug (<debugFlag>, <value>) - sets a debug flag to a specific value";

                case "HsvToRgb":
                    return "HsvToRgb(< hue >, < saturation >, < value >) - convert an HSV color value to RGB";

                case "P":
                    return "P(<value>,...) - print one or more values to the serial system.";

                case "Pl":
                    return "Pl(<value>,...) - print one or more values followed by a newline to the serial system.";

                case "Rand":
                    return "float Rand(minimum, maximum) - return a random number between minimum and maximum";

                case "ReadButton":
                    return "int ReadButton(<buttonNumber>) - returns the current value of a button";

                case "S":
                    return "S(<cycleCount>, <firstLedTargetBrightness>, ..., <lastLedTargetBrightness>) - define a fade on multiple LEDs";

                case "Si":
                    return "Si(<cycleCount>, <firstLedTargetBrightness>, ..., <lastLedTargetBrightness>) - define and execute a fade on multiple LEDs";
            }

            return null;
        }


        private void c_textBoxProgramText_MouseLeave(object sender, EventArgs e)
        {
            DisableMouseHoverTimer();
        }

        private void DisableMouseHoverTimer()
        {
            c_timerProgramTextHover.Stop();
            c_timerProgramTextHover.Enabled = false;
        }

        private string UpgradeLanguage(string programText)
        {
            return programText
                .Replace("ENDFOR", "endfor")
                .Replace("FOR", "for")
                .Replace("ENDFUNC", "endfunc")
                .Replace("FUNC", "func")
                .Replace("ENDIF", "endif")
                .Replace("ELSEIF", "elseif")
                .Replace("ELSE", "else")
                .Replace("IF", "if")
                .Replace("BREAK", "break")
                .Replace("RETURN", "return")
                .Replace("RAND(", "Rand(")
                .Replace("DEBUG(", "Debug(")
                .Replace("DI(", "Di(")
                .Replace("SI(", "Si(")
                .Replace("PL(", "Pl(")
                .Replace("ABORT(", "Abort(")
                .Replace("CONFIGLED", "ConfigLed")
                .Replace("CONFIGBUTTON", "ConfigButton")
                .Replace("READBUTTON", "ReadButton");
        }

        private void c_buttonReformat_Click(object sender, EventArgs e)
        {
            StringBuilder prettyVersion = new StringBuilder(_project.ProgramText.Length + 256);

            FadeLibrary.PrettyFormat(_project.ProgramText, prettyVersion, prettyVersion.Capacity);

            _project.ProgramText = UpgradeLanguage(prettyVersion.ToString());
            _project.ProgramText = _project.ProgramText.Replace("\n", "\r\n");
            c_textBoxProgramText.Text = _project.ProgramText;

            // call reformat function, change reformat function to modify uppercase keywords and function to lower/pascal cased ones. 

            // Make language changes one keyword at a time first. 
        }

        private void c_buttonRemote0_Click(object sender, EventArgs e)
        {
            c_labelWorking.Text = "Working...";

            string buttonUrl = "http://" + c_textBoxIPAddress.Text + "/Button?ButtonNumber=" + ((Button)sender).Text;
            WebRequest req = WebRequest.Create(buttonUrl);
 
            try
            {
                WebResponse res = req.GetResponse();
                StreamReader sr = new StreamReader(res.GetResponseStream());
                string returnvalue = sr.ReadToEnd();
                c_labelWorking.Text = "Done";
            }
            catch (Exception exception)
            {
                MessageBox.Show(exception.Message);
                c_labelWorking.Text = "Error";
            }
        }
    }
}
