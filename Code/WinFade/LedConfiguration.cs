using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;

namespace WinFade
{
    public class LedConfiguration
    {
        internal List<LedSpot> _ledSpots;

        public int XOffset { get; set; }
        public int YOffset { get; set; }
        public int LedSize { get; set; }

        private LedGroupType _groupType;

        public LedGroupType GroupType
        {
            get { return _groupType; }
            set
            {
                if (value != _groupType)
                {
                    _groupType = value;
                    Create();
                }
            }
        }



        protected LedConfiguration()
        {
            _ledSpots = new List<LedSpot>();

            LedSize = 30;
            XOffset = 0;
            YOffset = 0;
            GroupType = LedGroupType.Strip;
        }

        private int _ledCount;

        public int GroupNumber { get; set; }

        public int LedCount
        {
            get { return _ledCount; }

            set
            {
                _ledCount = value;
            }
        }

        public virtual void Create()
        {
        }

        public override string ToString()
        {
            return String.Format("Group {0}, {1}, {2}", GroupNumber, GroupType, LedCount);
        }

        internal virtual void SaveCustom(StreamWriter writer)
        {
        }

        internal void Save(StreamWriter writer)
        {
            writer.WriteLine("GroupType={0}", GroupType.ToString());

            writer.WriteLine("GroupNumber={0}", GroupNumber);
            writer.WriteLine("LedSize={0}", LedSize);
            writer.WriteLine("XOffset={0}", XOffset);
            writer.WriteLine("YOffset={0}", YOffset);

            SaveCustom(writer);

            writer.WriteLine("LedSpotCount={0}", LedCount);
            writer.WriteLine();
            writer.WriteLine("// X, Y");

            // cleanup. Truncate if the list is longer than LedCount, extend it if it is shorter...

            while (_ledSpots.Count < LedCount)
            {
                _ledSpots.Add(new LedSpot(50, 50));
            }

            while (_ledSpots.Count > LedCount)
            {
                _ledSpots.RemoveAt(_ledSpots.Count - 1);
            }


            foreach (LedSpot ledSpot in _ledSpots)
            {
                writer.WriteLine("LedSpot={0},{1}", ledSpot.X, ledSpot.Y);
            }
        }

        protected static string GetAfterName(StreamReader reader, string name)
        {
            string line = reader.ReadLine();
            if (line.StartsWith(name))
            {
                return line.Substring(name.Length + 1);
            }

            return null;
        }

        internal static int GetNumberAfterName(StreamReader reader, string name)
        {
            return Int32.Parse(GetAfterName(reader, name));
        }

        internal virtual void LoadCustom(StreamReader reader)
        {

        }

        internal static LedConfiguration Load(StreamReader reader)
        {
            string groupType = GetAfterName(reader, "GroupType");
            LedConfiguration newConfiguration = CreateLedConfigurationForType(groupType);

            newConfiguration.GroupNumber = GetNumberAfterName(reader, "GroupNumber");
            newConfiguration._ledSpots = new List<LedSpot>();

            newConfiguration.LedSize = GetNumberAfterName(reader, "LedSize");
            newConfiguration.XOffset = GetNumberAfterName(reader, "XOffset");
            newConfiguration.YOffset = GetNumberAfterName(reader, "YOffset");

            newConfiguration.LoadCustom(reader);

            newConfiguration.LedCount = GetNumberAfterName(reader, "LedSpotCount");
            reader.ReadLine(); //  Blank line
            reader.ReadLine(); //  X, Y

            for (int i = 0; i < newConfiguration.LedCount; i++)
            {
                string line = GetAfterName(reader, "LedSpot");

                if (line != null)
                {
                    newConfiguration._ledSpots.Add(new LedSpot(line));
                }
            }

            return newConfiguration;
        }

        public static LedConfiguration CreateLedConfigurationForType(string groupType)
        {
            LedConfiguration newConfiguration = null;

            switch (groupType)
            {
                case "Strip":
                    newConfiguration = new LedConfigurationStrip();
                    newConfiguration.GroupType = LedGroupType.Strip;
                    break;

                case "Ring":
                    newConfiguration = new LedConfigurationRing();
                    newConfiguration.GroupType = LedGroupType.Ring;
                    break;

                case "Matrix":
                    newConfiguration = new LedConfigurationMatrix();
                    newConfiguration.GroupType = LedGroupType.Matrix;
                    break;

                case "Custom":
                    newConfiguration = new LedConfigurationCustom();
                    newConfiguration.GroupType = LedGroupType.Custom;
                    break;

                case "Pwm":
                    newConfiguration = new LedConfigurationPwm();
                    newConfiguration.GroupType = LedGroupType.Pwm;
                    break;
            }

            return newConfiguration;
        }

        internal virtual void PopulateAndEnableForGroupType(EditLedSetup editLedSetup)
        {

        }

        public virtual void UpdateLedColor(Graphics graphics, int ledNumber, float red, float green, float blue)
        {
            if (_ledSpots.Count == 0)
            {
                Create();
            }

            LedSpot ledSpot = _ledSpots[ledNumber];

            ledSpot.DrawLed(graphics, LedSize, XOffset, YOffset, red, green, blue);
        }

        public bool PointInLed(int pointX, int pointY)
        {
            foreach (LedSpot ledSpot in _ledSpots)
            {
                if (pointX > ledSpot.X + XOffset && pointX < ledSpot.X + XOffset + LedSize &&
                    pointY > ledSpot.Y + YOffset && pointY < ledSpot.Y + YOffset + LedSize)
                {
                    return true;
                }
            }

            return false;
        }

        public void Rotate(int xCenter, int yCenter, int direction) // direction 1 = +, -1 = -
        {
            xCenter -= XOffset;
            yCenter -= YOffset;

            float rotationAngleInRadians = direction == 1 ? 0.01F : -0.01F;

            foreach (LedSpot ledSpot in _ledSpots)
            {
                float x = (float) (Math.Cos(rotationAngleInRadians) * (ledSpot.X - xCenter) - Math.Sin(rotationAngleInRadians) * (ledSpot.Y - yCenter) + xCenter);
                float y = (float) (Math.Sin(rotationAngleInRadians) * (ledSpot.X - xCenter) + Math.Cos(rotationAngleInRadians) * (ledSpot.Y - yCenter) + yCenter);

                ledSpot.X = x;
                ledSpot.Y = y;
            }
        }

        public void MoveOrigin(int xOffset, int yOffset)
        {
            XOffset += xOffset;
            YOffset += yOffset;
        }

        public void CopyTo(LedConfiguration newConfiguration)
        {
            newConfiguration.GroupNumber = GroupNumber;
            newConfiguration.LedCount = LedCount;
            newConfiguration.LedSize = LedSize;
            newConfiguration.XOffset = XOffset;
            newConfiguration.YOffset = YOffset;
        }
    }
}
