using System;
using System.Drawing;
using System.IO;

namespace WinFade
{
  class LedConfigurationServo : LedConfiguration
  {
    public int MinAngle { get; set; }
    public int MaxAngle { get; set; }

    public LedConfigurationServo()
    {
      MinAngle = -90;
      MaxAngle = 90;
    }

    internal override void LoadCustom(FileLineParser fileLineParser)
    {
      MinAngle = fileLineParser.GetNumberAfterName("MinAngle");
      MaxAngle = fileLineParser.GetNumberAfterName("MaxAngle");
    }

    internal override void SaveCustom(StreamWriter writer)
    {
      writer.WriteLine("MinAngle={0}", MinAngle);
      writer.WriteLine("MaxAngle={0}", MaxAngle);
    }

    public override void Create()
    {
      _ledSpots.Clear();

      // only 1 spot per servo
      _ledSpots.Add(new LedSpot(0, 0));
    }

    public void ValuesChanged(EditLedSetup editLedSetup)
    {
      MinAngle = EditLedSetup.GetValueFromTextBox(editLedSetup.c_textBoxServoMinAngle);
      MaxAngle = EditLedSetup.GetValueFromTextBox(editLedSetup.c_textBoxServoMaxAngle);

      Create();
    }

    internal override void PopulateAndEnableForGroupType(EditLedSetup editLedSetup)
    {
      editLedSetup.c_radioButtonServo.Checked = true;

      int maxAngle = MaxAngle;  // cache as update to min will read max...
      editLedSetup.c_textBoxServoMinAngle.Text = MinAngle.ToString();
      editLedSetup.c_textBoxServoMaxAngle.Text = maxAngle.ToString();
      editLedSetup.c_textBoxServoMinAngle.Enabled = true;
      editLedSetup.c_textBoxServoMaxAngle.Enabled = true;
    }

    static internal void DisableForGroupType(EditLedSetup editLedSetup)
    {
      editLedSetup.c_textBoxServoMinAngle.Enabled = true;
      editLedSetup.c_textBoxServoMaxAngle.Enabled = true;
    }

    public Point RotatePoint(Point point, Point origin, double angleInDegrees)
    {
      double angle = angleInDegrees * Math.PI / 180; 

      double s = Math.Sin(angle);
      double c = Math.Cos(angle);

      point.X -= origin.X;
      point.Y -= origin.Y;

      double xNew = point.X * c - point.Y * s;
      double yNew = point.X * s + point.Y * c;

      return new Point((int) xNew + origin.X, (int) yNew + origin.Y);
    }

    public override void UpdateLedColor(Graphics graphics, int ledNumber, float red, float green, float blue)
    {
      if (_ledSpots.Count == 0 || _ledSpots.Count != LedCount)
      {
        Create();
      }

      LedSpot ledSpot = _ledSpots[ledNumber];

      using (Brush brush = new SolidBrush(Color.White))
      using (Pen myPen = new Pen(brush, 2))
      {
        int shortLength = LedSize / 5;
        int longLength = LedSize / 2;

        double angle = MinAngle + (MaxAngle - MinAngle) * red;

        Point center = new Point(XOffset, YOffset + shortLength);

        Point left = new Point(XOffset - shortLength, YOffset + longLength);
        Point right = new Point(XOffset + shortLength, YOffset + longLength);
        Point top = new Point(XOffset, YOffset - longLength);

        left = RotatePoint(left, center, angle);
        right = RotatePoint(right, center, angle);
        top = RotatePoint(top, center, angle);

        graphics.DrawLine(myPen, left, right);
        graphics.DrawLine(myPen, right, top);
        graphics.DrawLine(myPen, top, left);
      }

#if fred

      // store current values, then decode them into a single color to draw...

      if (ledNumber < 4)
      {
        _values[ledNumber] = red;
      }

      for (int colorMappingIndex = 0; colorMappingIndex < ColorMapping.Length; colorMappingIndex++)
      {
        char mappingCode = ColorMapping[colorMappingIndex];

        switch (mappingCode)
        {
          case 'R':
            red = _values[colorMappingIndex];
            break;

          case 'G':
            green = _values[colorMappingIndex];
            break;

          case 'B':
            blue = _values[colorMappingIndex];
            break;
        }
      }

      // white mapping. Add white components if they are higher than the current values...
      for (int colorMappingIndex = 0; colorMappingIndex < ColorMapping.Length; colorMappingIndex++)
      {
        char mappingCode = ColorMapping[colorMappingIndex];

        float value = _values[colorMappingIndex];

        switch (mappingCode)
        {
          case 'W':
            red = value > red ? value : red;
            green = value > green ? value : green;
            blue = value > blue ? value : blue;
            break;
        }
      }

      ledSpot.DrawLed(graphics, LedSize, XOffset, YOffset, red, green, blue);
#endif
    }
  }
}
