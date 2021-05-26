using System.Drawing;

namespace WinFade
{
    internal class LedSpot
  {
    public float X { get; set; }
    public float Y { get; set; }
    public float Size { get; set; }

    public LedSpot(float x, float y, float size)
    {
        X = x;
        Y = y;
        Size = size;
    }

    public LedSpot(string line)
    {
        string[] parts = line.Split(',');

        X = float.Parse(parts[0]);
        Y = float.Parse(parts[1]);
        Size = float.Parse(parts[2]);
    }

    public void DrawLed(Graphics graphics, float red, float green, float blue)
    {
        Color color = Color.FromArgb((int) (255 * red), (int) (255 * green), (int) (255 * blue));
        using (SolidBrush myBrush = new SolidBrush(color))
        {
            int x = (int) X;
            int y = (int) Y;
            graphics.FillEllipse(myBrush, new Rectangle(x, y, (int) Size, (int) Size));
        }
    }
  }
}
