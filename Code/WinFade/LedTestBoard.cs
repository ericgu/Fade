using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;

namespace WinFade
{
    public class LedTestBoard
    {
        private List<LedConfiguration> _ledConfigurations;

        public LedTestBoard()
        {
            _ledConfigurations = new List<LedConfiguration>();
        }

        public List<LedConfiguration> LedConfigurations
        {
            get { return _ledConfigurations; }
        }

        public void AddNewDevice(int groupNumber, string ledType, int ledCount, int pin1, int pin2, int pin3, int pin4)
        {
            if (!_ledConfigurations.Exists(ledConfiguration => ledConfiguration.GroupNumber == groupNumber))
            {
                LedConfiguration ledConfiguration = new LedConfiguration();
                ledConfiguration.GroupNumber = groupNumber;
                _ledConfigurations.Add(ledConfiguration);
                ledConfiguration.LedCount = ledCount;
            }
        }

        public void UpdateLedColor(Graphics graphics, int ledGroupNumber, int ledNumber, float red, float green, float blue)
        {
            foreach (LedConfiguration ledConfiguration in _ledConfigurations.Where(ledConfiguration =>
                ledConfiguration.GroupNumber == ledGroupNumber))
            {
                if (ledNumber < ledConfiguration.LedCount)
                {
                    ledConfiguration.UpdateLedColor(graphics, ledNumber, red, green, blue);
                    return;
                }

                ledNumber -= ledConfiguration.LedCount;
            }
        }

        private static string BasePath
        {
            get
            {
                string path = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "WinFade");

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

        public void Save(string filename)
        {
            using (StreamWriter writer = File.CreateText(FullFilename(filename)))
            {
                Save(writer);
            }
        }

        public void Save(StreamWriter writer)
        {
            writer.WriteLine("ConfigurationCount={0}", _ledConfigurations.Count);

            foreach (LedConfiguration ledGroup in _ledConfigurations)
            {
                ledGroup.Save(writer);
                writer.WriteLine();
            }
        }


        public void Load(string filename)
        {
            string fullFilename = FullFilename(filename);

            if (!File.Exists(fullFilename))
            {
                return;
            }

            _ledConfigurations.Clear();

            using (StreamReader reader = File.OpenText(fullFilename))
            {
                Load(reader);
            }
        }

        public void Load(StreamReader reader)
        {
            string readLine = reader.ReadLine();
            readLine = readLine.Substring("ConfigurationCount=".Length);
            int ledConfigurationCount = Int32.Parse(readLine);
            _ledConfigurations.Clear();

            for (int ledConfigurationNumber = 0; ledConfigurationNumber < ledConfigurationCount; ledConfigurationNumber++)
            {
                LedConfiguration ledConfiguration = LedConfiguration.Load(reader);
                _ledConfigurations.Add(ledConfiguration);
                reader.ReadLine();
            }
        }

        public LedConfiguration FindLedConfigurationAtLocation(int eLocationX, int eLocationY)
        {
            foreach (LedConfiguration ledConfiguration in LedConfigurations)
            {
                if (ledConfiguration.PointInLed(eLocationX, eLocationY))
                {
                    return ledConfiguration;
                }
            }

            return null;
        }

        public void ChangeLedConfigurationToType(int selectedIndex, LedGroupType ledGroupType)
        {
            LedConfiguration newConfiguration = LedConfiguration.CreateLedConfigurationForType(ledGroupType.ToString());

            LedConfiguration currentConfiguration = _ledConfigurations[selectedIndex];

            currentConfiguration.CopyTo(newConfiguration);

            _ledConfigurations[selectedIndex] = newConfiguration;
        }

        public void PasteCustomConfigurationText(int selectedIndex, string contents)
        {
            LedConfigurationCustom currentConfiguration = _ledConfigurations[selectedIndex] as LedConfigurationCustom;

            string[] lines = contents.Replace("\n", "").Split('\r');

            foreach (string line in lines)
            {
                string value = line;
                int index = value.IndexOf("=");
                if (index != -1)
                {
                    value = value.Substring(index + 1);
                }

                currentConfiguration._ledSpots.Add(new LedSpot(value));
            }

            currentConfiguration.LedCount = currentConfiguration._ledSpots.Count;
        }
    }
}
