using System;
using System.IO;

namespace WinFade
{
    public class Project
    {
        public string ProjectName { get; set; }
        public string Filename { get; set; }
        
        public string ProgramText { get; set; }

        public LedTestBoard LedTestBoard { get; set; }

        public Project()
        {
            LedTestBoard = new LedTestBoard();
        }

        public static string BasePath
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
            return Path.Combine(BasePath, filename);
        }

        public void Save()
        {
            using (StreamWriter writer = File.CreateText(Filename))
            {
                writer.WriteLine("WinFade Project");
                writer.WriteLine(ProjectName);
                LedTestBoard.Save(writer);

                writer.WriteLine("Program Text");
                writer.WriteLine(ProgramText);
            }
        }

        public void Load()
        {
            if (!File.Exists(Filename))
            {
                return;
            }

            using (StreamReader reader = File.OpenText(Filename))
            {
                reader.ReadLine();                  // header
                ProjectName = reader.ReadLine();
                LedTestBoard.Load(reader);

                reader.ReadLine();                  // Program Text
                ProgramText = reader.ReadToEnd();
            }
        }

    }
}
