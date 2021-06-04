using System.Drawing;

namespace WinFade
{
    internal class LedSpot
  {
    public float X { get; set; }
    public float Y { get; set; }

    public LedSpot(float x, float y)
    {
        X = x;
        Y = y;
    }

    public LedSpot(string line)
    {
        string[] parts = line.Split(',');

        X = float.Parse(parts[0]);
        Y = float.Parse(parts[1]);
    }

    public void DrawLed(Graphics graphics, int size, int xOffset, int yOffset, float red, float green, float blue)
    {
        Color color = Color.FromArgb((int) (255 * red), (int) (255 * green), (int) (255 * blue));
        using (SolidBrush myBrush = new SolidBrush(color))
        {
            int x = (int) X + xOffset;
            int y = (int) Y + yOffset;
            graphics.FillEllipse(myBrush, new Rectangle(x, y, size, size));
        }
    }
  }
}
