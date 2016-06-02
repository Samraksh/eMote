using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.IO.Ports;

namespace RadarDataLogging
{
    class Program
    {
        static void Main(string[] args)
        {
            UInt16 data = 0;
            
            SerialPort inCOM = new SerialPort();
            inCOM.PortName = "COM7";
            inCOM.BaudRate = 115200;
            try
            {
                inCOM.Open();
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine("failed to open serial port: " + ex.ToString());
                return;
            }
            string outFileName = @"D:\Users\Chris\Documents\Visual Studio 2013\Projects\RadarAlgorithmGraph\RadarAlgorithmGraph\recorded.bbs";

            FileStream fs = new FileStream(outFileName, FileMode.Create, FileAccess.Write);
            BinaryWriter binWriter = new BinaryWriter(fs);
            byte[] COMBuffer = new byte[500];
            int bytesRead = 0;
            while (true)
            {
                bytesRead = inCOM.Read(COMBuffer, 0, 500);
                binWriter.Write(COMBuffer, 0, bytesRead);
                binWriter.Flush();
                /*System.Diagnostics.Debug.WriteLine("-----------------------------");
                //System.Diagnostics.Debug.WriteLine(bytesRead.ToString());
                for (int k = 0; k < bytesRead; k++)
                {
                    System.Diagnostics.Debug.WriteLine(COMBuffer[k].ToString());
                }*/
            }

            inCOM.Close();
            fs.Close();
        }
    }
}
