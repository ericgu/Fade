using System.Drawing;
using System.Drawing.Imaging;
using System.Windows.Forms;

namespace WinFade
{
    public partial class LedForm : Form
    {
        readonly Bitmap _bitmap;
        readonly Graphics _graphics;
        private readonly LedGroups _ledGroups;

        private delegate void RenderDelegate();

        public LedForm()
        {
            InitializeComponent();

            _bitmap = new Bitmap(Width, Height, PixelFormat.Format32bppPArgb);
            _graphics = Graphics.FromImage(_bitmap);
            _ledGroups = new LedGroups();
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
            }
        }

        public void UpdateLedColor(int ledGroupNumber, int ledNumber, float red, float green, float blue)
        {
            _ledGroups.UpdateLedColor(_graphics, ledGroupNumber, ledNumber, red, green, blue);
        }

    }
}




