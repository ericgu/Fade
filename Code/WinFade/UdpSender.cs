using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace WinFade
{
    class UdpSender
    {
        private UdpClient _udpClient;
        private UdpData _udpData;
        private int _port;

        public UdpSender()
        {
            _udpClient = new UdpClient();

            _udpData = new UdpData();
        }

        public void Init(ushort channelCount, ushort valuesPerChannel, ushort port, ushort universe)
        {
            _port = port;
            _udpClient.Client.Bind(new IPEndPoint(IPAddress.Any, port));
            _udpData.Init(channelCount, valuesPerChannel, universe);
        }

        public void UpdateLed(int ledGroupNumber, int channel, int brightnessCount, float brightness1,
            float brightness2,
            float brightness3, float brightness4)
        {
            if (brightnessCount >= 1)
            {
                _udpData.SetValueForChannel(channel, 0, (ushort) (brightness1 * UInt16.MaxValue));
            }

            if (brightnessCount >= 2)
            {
                _udpData.SetValueForChannel(channel, 1, (ushort) (brightness2 * UInt16.MaxValue));
            }

            if (brightnessCount >= 3)
            {
                _udpData.SetValueForChannel(channel, 2, (ushort) (brightness3 * UInt16.MaxValue));
            }

            if (brightnessCount >= 4)
            {
                _udpData.SetValueForChannel(channel, 3, (ushort) (brightness4 * UInt16.MaxValue));
            }
        }

        public void LedUpdateCycleDone()
        {
            byte[] data = _udpData.GetByteArray();

            _udpClient.Send(data, data.Length, "255.255.255.255", _port);

            _udpData.IncrementSequenceNumber();
        }
    }
}
