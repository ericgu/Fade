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

        internal virtual void LoadCustom(FileLineParser fileLineParser)
        {

        }

        internal static LedConfiguration Load(FileLineParser fileLineParser)
        {
            string groupType = fileLineParser.GetAfterName("GroupType");
            LedConfiguration newConfiguration = CreateLedConfigurationForType(groupType);

            newConfiguration.GroupNumber = fileLineParser.GetNumberAfterName("GroupNumber");
            newConfiguration._ledSpots = new List<LedSpot>();

            newConfiguration.LedSize = fileLineParser.GetNumberAfterName("LedSize");
            newConfiguration.XOffset = fileLineParser.GetNumberAfterName("XOffset");
            newConfiguration.YOffset = fileLineParser.GetNumberAfterName("YOffset");

            newConfiguration.LoadCustom(fileLineParser);

            newConfiguration.LedCount = fileLineParser.GetNumberAfterName("LedSpotCount");
            fileLineParser.ReadLine(); //  Blank line
            fileLineParser.ReadLine(); //  X, Y

            for (int i = 0; i < newConfiguration.LedCount; i++)
            {
                string line = fileLineParser.GetAfterName("LedSpot");

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
