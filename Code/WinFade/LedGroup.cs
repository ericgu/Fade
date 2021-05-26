using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;

namespace WinFade
{
    class LedGroup
    {
        readonly List<LedSpot> _ledSpots;
        private int _groupNumber;

        public LedGroup(int groupNumber)
        {
            _groupNumber = groupNumber;

            _ledSpots = new List<LedSpot>();

            CreateLeds(groupNumber);
            Save("FirstTest", "FirstTestFile");
            Load("FirstTestFile");
        }

        private static string BasePath
        {
            get
            {
                string path = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData),
                    "WinFade");

                if (!Directory.Exists(path))
                {
                    Directory.CreateDirectory(path);
                }

                return path;
            }
        }

        private string FullFilename(string filename)
        {
            return Path.Combine(BasePath, filename) + ".FadeLedConfig";
        }

        public void Save(string name, string filename)
        {
            using (StreamWriter writer = File.CreateText(FullFilename(filename)))
            {
                writer.WriteLine(name);
                writer.WriteLine(_ledSpots.Count);
                writer.WriteLine("// X, Y, Size");

                foreach (LedSpot ledSpot in _ledSpots)
                {
                    writer.WriteLine("{0},{1},{2}", ledSpot.X, ledSpot.Y, ledSpot.Size);
                }
            }
        }


        public void Load(string filename)
        {
            string fullFilename = FullFilename(filename);

            if (!File.Exists(fullFilename))
            {
                return;
            }

            _ledSpots.Clear();

            using (StreamReader reader = File.OpenText(fullFilename))
            {
                string name = reader.ReadLine();

                int count = Int32.Parse(reader.ReadLine());

                reader.ReadLine();

                while (!reader.EndOfStream)
                {
                    string line = reader.ReadLine();

                    _ledSpots.Add(new LedSpot(line));
                }
            }
        }

        public static List<string> ConfigFileList
        {
            get
            {
                DirectoryInfo directoryInfo = new DirectoryInfo(BasePath);
                return directoryInfo.GetFiles("*.FadeLedConfig").Select(fileInfo => fileInfo.Name.Replace(".FadeLedConfig", "")).ToList();
            }
        }


        public void CreateLeds(int groupNumber)
        {
            float size = 30;

            float x = 0;
            float y = 100 * groupNumber;

            for (int i = 0; i < 40; i++)
            {
                _ledSpots.Add(new LedSpot(x, y, size));
                x += 50;
            }
        }

        public void UpdateLedColor(Graphics graphics, int ledNumber, float red, float green, float blue)
        {
            LedSpot ledSpot = _ledSpots[ledNumber];

            ledSpot.DrawLed(graphics, red, green, blue);
        }
    }
}
