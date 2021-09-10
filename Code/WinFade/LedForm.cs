using System.Drawing;
using System.Drawing.Imaging;
using System.Windows.Forms;

namespace WinFade
{
    public delegate void ButtonPressedDelegate(int buttonNumber);

    public partial class LedForm : Form
    {
        Bitmap _bitmap;
        Graphics _graphics;
        private readonly LedTestBoard _ledTestBoard;

        private delegate void RenderDelegate();

        public event ButtonPressedDelegate ButtonPressed;

        private UdpSender _udpSender;

        public LedForm(LedTestBoard ledTestBoard)
        {
            _ledTestBoard = ledTestBoard;
            InitializeComponent();

            ClientSize = new Size(ledTestBoard.XSize, ledTestBoard.YSize);
            StartPosition = FormStartPosition.Manual;
            Location = new Point(ledTestBoard.XLocation, ledTestBoard.YLocation);

            _bitmap = new Bitmap(Width, Height, PixelFormat.Format32bppPArgb);
            _graphics = Graphics.FromImage(_bitmap);

            if (ledTestBoard.UdpEnabled)
            {
                _udpSender = new UdpSender();
                _udpSender.Init(1, 3, (ushort) ledTestBoard.UdpPort, (ushort) ledTestBoard.UdpUniverse);
            }
        }

        ~LedForm()
        {
            _graphics.Dispose();
            _bitmap.Dispose();
        }

        public void Render()
        {
            if (c_pictureBoxLeds.InvokeRequired)
            {
                var d = new RenderDelegate(Render);
                c_pictureBoxLeds.Invoke(d, null);
            }
            else
            {
                c_pictureBoxLeds.Image = _bitmap;
                c_pictureBoxLeds.Refresh();

                using (Brush brush = new SolidBrush(Color.Black))
                {
                    _graphics.FillRectangle(brush, 0, 0, c_pictureBoxLeds.Width, c_pictureBoxLeds.Height);
                }

                if (_ledTestBoard.UdpEnabled)
                {
                    _udpSender.LedUpdateCycleDone();
                }
            }
        }

        public void UpdateLedColor(int ledGroupNumber, int ledNumber, float red, float green, float blue)
        {
            _ledTestBoard.UpdateLedColor(_graphics, ledGroupNumber, ledNumber, red, green, blue);

            if (_ledTestBoard.UdpEnabled)
            {
                _udpSender.UpdateLed(ledGroupNumber, ledNumber, 3, red, green, blue, 0.0F);
            }
        }

        private LedConfiguration _dragLedConfiguration;
        private Point _dragStartPoint;
        private Point _dragCurrentPoint;

        private void c_pictureBoxLeds_MouseDown(object sender, MouseEventArgs e)
        {
            _dragLedConfiguration = _ledTestBoard.FindLedConfigurationAtLocation(e.Location.X, e.Location.Y);

            if (_dragLedConfiguration != null)
            {
                _dragStartPoint = e.Location;
                _dragCurrentPoint = _dragStartPoint;
                // set cursor.
            }
        }

        private void c_pictureBoxLeds_MouseMove(object sender, MouseEventArgs e)
        {
            if (_dragLedConfiguration == null)
            {
                return;
            }

            if (e.Button == MouseButtons.Left)
            {
                int xOffset = e.Location.X - _dragCurrentPoint.X;
                int yOffset = e.Location.Y - _dragCurrentPoint.Y;

                _dragLedConfiguration.MoveOrigin(xOffset, yOffset);
            }
            else if (e.Button == MouseButtons.Right)
            {
                _dragLedConfiguration.Rotate(_dragStartPoint.X, _dragStartPoint.Y, _dragCurrentPoint.Y > e.Location.Y ? 1 : -1);
            }

            _dragCurrentPoint = e.Location;
        }

        private void c_pictureBoxLeds_MouseUp(object sender, MouseEventArgs e)
        {
            _dragLedConfiguration = null;
        }

        private void LedForm_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (ButtonPressed != null && e.KeyChar >= '0' && e.KeyChar <= '9')
            {
                ButtonPressed(e.KeyChar - '0');
            }
        }

        private void LedForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            _ledTestBoard.XLocation = Location.X;
            _ledTestBoard.YLocation = Location.Y;
            _ledTestBoard.XSize = Width;
            _ledTestBoard.YSize = Height;
        }

        private void c_pictureBoxLeds_Resize(object sender, System.EventArgs e)
        {
            _bitmap = new Bitmap(Width, Height, PixelFormat.Format32bppPArgb);
            _graphics = Graphics.FromImage(_bitmap);
        }
    }
}




