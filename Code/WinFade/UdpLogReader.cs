using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WinFade
{
    class UdpLogReader
    {

        IAsyncResult _asyncResult;

        UdpClient _udpClient;

        public delegate void UdpMessageHandler(string message, IPAddress sourceIpAddress);

        private UdpMessageHandler _udpMessageHandler;

        public void Start(UdpMessageHandler udpMessageHandler, int udpLogPortNumber)
        {
            _udpMessageHandler = udpMessageHandler;

            _udpClient = new UdpClient(udpLogPortNumber);

            StartListening();
        }

        private void StartListening()
        {
            _asyncResult = _udpClient.BeginReceive(Receive, null);
        }

        private void Receive(IAsyncResult ar)
        {
            if (_udpClient != null)
            {
                IPEndPoint ip = new IPEndPoint(IPAddress.Any, 12345);
                try
                {
                    byte[] bytes = _udpClient.EndReceive(ar, ref ip);
                    string message = Encoding.ASCII.GetString(bytes);
                    _udpMessageHandler(message, ip.Address);

                    // Console.WriteLine("From {0} received: {1} ", ip.Address.ToString(), message);
                    StartListening();
                }
                catch (ObjectDisposedException)
                {
                }
            }
        }

        public void Abort()
        {
            _udpClient.Close();
            _udpClient = null;
        }

    }
}
