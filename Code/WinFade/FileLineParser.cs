using System;
using System.Collections.Generic;
using System.IO;

namespace WinFade
{
    public class FileLineParser
    {
        private int _currentLine;

        private readonly string[] _lines;

        public FileLineParser(string filename)
        {
            _lines = File.ReadAllLines(filename);
            _currentLine = 0;
        }

        public string GetAfterName(string name)
        {
            string line = ReadLine();
            if (line.StartsWith(name))
            {
                return line.Substring(name.Length + 1);
            }

            _currentLine--;     // didn't match, try this line again...
            return null;
        }

        public int GetNumberAfterName(string name)
        {
            return GetNumberAfterName(name, 0);
        }


        public int GetNumberAfterName(string name, int defaultIfMissing)
        {
            string value = GetAfterName(name);
            if (value != null)
            {
                return Int32.Parse(value);
            }

            return defaultIfMissing;
        }

        public string ReadLine()
        {
            //_reader.ReadLine();

            return _lines[_currentLine++];
        }

        public string ReadToEnd()
        {
            List<string> remaining = new List<string>();

            for (int i = _currentLine; i < _lines.Length; i++)
            {
                remaining.Add(_lines[i]);
            }

            string result = String.Join("\r\n", remaining);

            return result;
        }
    }
}
