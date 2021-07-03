using System;
using System.ComponentModel;
using System.IO;
using System.Net;
using System.Text;
using System.Threading;

namespace WinFade
{
    internal class Remote
    {
        private string _name;

        public delegate void NameFetchedDelegate(string name);

        public event NameFetchedDelegate NameFetched;

        public string IPAddress { get; set; }
        public string Program { get; set; }

        public void PressButton(string buttonNumber)
        {
            string buttonUrl = "http://" + IPAddress + "/Button?ButtonNumber=" + buttonNumber;
            WebRequest req = WebRequest.Create(buttonUrl);

            WebResponse res = req.GetResponse();
            StreamReader sr = new StreamReader(res.GetResponseStream());
            string returnvalue = sr.ReadToEnd();
        }

        public void Update(string programText)
        {
            WebRequest req = WebRequest.Create("http://" + IPAddress);
            string postData = "Program=" + programText;

            byte[] send = Encoding.Default.GetBytes(postData);
            req.Method = "POST";
            req.ContentType = "application/x-www-form-urlencoded";
            req.ContentLength = send.Length;

            Stream sout = req.GetRequestStream();
            sout.Write(send, 0, send.Length);
            sout.Flush();
            sout.Close();

            WebResponse res = req.GetResponse();
            StreamReader sr = new StreamReader(res.GetResponseStream());
            string returnvalue = sr.ReadToEnd();
        }

        public void GetNameWorker(object unused)
        {
            string mainUrl = "http://" + IPAddress;
            WebRequest req = WebRequest.Create(mainUrl);

            try
            {
                WebResponse res = req.GetResponse();
                StreamReader sr = new StreamReader(res.GetResponseStream());
                string returnValue = sr.ReadToEnd();
                int start = returnValue.IndexOf("<title");
                if (start != -1)
                {
                    int end = returnValue.IndexOf(":");
                    _name = returnValue.Substring(start + 7, end - start - 7);

                    if (NameFetched != null)
                    {
                        NameFetched("Connected to " + _name);
                    }
                }

                start = returnValue.IndexOf("name=\"Program\">");
                if (start != -1)
                {
                    int end = returnValue.IndexOf("</textarea>");
                    Program = returnValue.Substring(start + 15, end - start - 15);
                    Program = Program.Replace("\n", "\r\n");
                }
            }
            catch (Exception)
            {
                if (NameFetched != null)
                {
                    NameFetched("Disconnected");
                }
            }
        }


        public void GetName()
        {
            ThreadPool.QueueUserWorkItem(new WaitCallback(GetNameWorker));
        }
    }
}