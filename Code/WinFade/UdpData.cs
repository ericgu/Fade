using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace WinFade
{
  [StructLayout(LayoutKind.Sequential)]
  unsafe struct UdpData
  {
    [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
    string _name;

    ushort _size;
    ushort _universe; // support multiple animations on one network
    ushort _sequenceNumber; // incrmental message sequenceNumber
    ushort _channelCount; // # of channels included in this message
    ushort _valuesPerChannel; // # of data values per channel

    [MarshalAs((UnmanagedType.LPArray))]
    private ushort[] _channelData;

    public void Init(ushort channelCount, ushort valuesPerChannel, ushort universe)
    {
      _name = "FADE1.0";

      //_size = size;

      _universe = universe;
      _channelCount = channelCount;
      _valuesPerChannel = valuesPerChannel;
      _sequenceNumber = 0;

      _channelData = new ushort[_channelCount * _valuesPerChannel];

      _size = (ushort) (16 + 10 + 2 * _channelCount * _valuesPerChannel);
    }

    public void SetValueForChannel(int channelNumber, int valueInChannel, ushort value)
    {
      int index = channelNumber * _valuesPerChannel + valueInChannel;

      _channelData[index] = value;
    }

    public void IncrementSequenceNumber()
    {
      _sequenceNumber++;
    }

    public byte[] GetByteArray()
    {
        byte[] bytes = new byte[_size];

        fixed (byte* pBytes = bytes)
        {
            byte* pName = pBytes;

            for (int i = 0; i < _name.Length; i++)
            {
                *pName = (byte) _name[i];
                pName++;
            }

            ushort* pValues = (ushort*) (pBytes + 16);

            *pValues = _size;
            pValues++;
            *pValues = _universe;
            pValues++;
            *pValues = _sequenceNumber;
            pValues++;
            *pValues = _channelCount;
            pValues++;
            *pValues = _valuesPerChannel;
            pValues++;

            foreach (ushort data in _channelData)
            {
                *pValues = data;
                pValues++;
            }
        }

        return bytes;


#if fred
      int size = Marshal.SizeOf(this);

      byte[] bytes = new byte[size];
      IntPtr intPtr = Marshal.AllocHGlobal(size);

      Marshal.StructureToPtr(this, intPtr, false);
      Marshal.Copy(intPtr, bytes, 0, size);

      Marshal.FreeHGlobal(intPtr);
#endif

        return bytes;
    }
  }
}
