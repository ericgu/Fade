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

        public void Save()
        {
            using (StreamWriter writer = File.CreateText(Filename))
            {
                writer.WriteLine("WinFade Project");
                writer.WriteLine(ProjectName);
                LedTestBoard.Save(writer);

                writer.WriteLine("Program Text");
                writer.Write(ProgramText);
            }
        }

        public void Load()
        {
            if (!File.Exists(Filename))
            {
                if (File.Exists("First.txt"))
                {
                    ProgramText = File.ReadAllText("First.txt");
                }

                return;
            }

            FileLineParser fileLineParser = new FileLineParser(Filename);

            fileLineParser.ReadLine(); // header
            ProjectName = fileLineParser.ReadLine();
            LedTestBoard.Load(fileLineParser);

            fileLineParser.ReadLine(); // Program Text
            ProgramText = fileLineParser.ReadToEnd();
        }
    }
}
