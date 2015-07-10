using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;
using System.IO;
using System.Threading;

namespace ADC_Callback
{
    class Program
    {
        const string fileName = @"testTemp\rx_data.txt";
        const string fileOut = @"testTemp\results.txt";

        static void Main(string[] args)
        {
            ulong[] callbackTimeValue = new ulong[4];
            ulong[] dateTimeValue = new ulong[4];
            bool testResult = false;
            char[] trimChars = new char[] { ' ', ']' };           
            int lineNum = 0;
            
            try
            {
                using (StreamReader reader = new StreamReader(fileName))
                {
                    string line;
                    string[] timeString;

                    line = reader.ReadLine();
                    while ((line != null) && (lineNum<2))
                    {
                        System.Diagnostics.Debug.WriteLine(line);
                        line = line.Replace("   ", " ");
                        line = line.Replace("  ", " ");
                        timeString = line.Split(' ');
                        callbackTimeValue[lineNum] = ulong.Parse(timeString[0]);
                        dateTimeValue[lineNum] = ulong.Parse(timeString[1]);
                        
                        line = reader.ReadLine();
                        lineNum++;
                    }
                    reader.Close();
                }

                ulong callbackTimeDiff = callbackTimeValue[1] - callbackTimeValue[0];
                ulong dateTimeDiff = dateTimeValue[1] - dateTimeValue[0];

                System.Diagnostics.Debug.WriteLine("callback diff: " + callbackTimeDiff.ToString() + " dateTime diff: " + dateTimeDiff.ToString());

                if ( (callbackTimeDiff > 7990000) && (callbackTimeDiff < 8010000) && (dateTimeDiff < 10200000) && (dateTimeDiff > 9800000) )
                    testResult = true;
                else
                    testResult = false;

                using (StreamWriter writer = new StreamWriter(fileOut, false))
                {
                    if (testResult == true)
                        writer.Write("result = PASS\r\n");
                    else
                        writer.Write("result = FAIL\r\n");
                    writer.Write("accuracy = 0\r\n");
                    writer.Write("resultParameter1 = " + callbackTimeDiff.ToString() + "\r\n");
                    writer.Write("resultParameter2 = " + dateTimeDiff.ToString() + "\r\n");
                    writer.Write("resultParameter3 =\r\n");
                    writer.Write("resultParameter4 =\r\n");
                    writer.Write("resultParameter5 =\r\n");
                    writer.Close();
                }

            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine("test results read FAIL: " + ex.Message);
                using (StreamWriter writer = new StreamWriter(fileOut, false))
                {
                    writer.Write("result = FAIL\r\n");
                    writer.Write("accuracy = 0\r\n");
                    writer.Write("resultParameter1 = exception thrown\r\n");
                    writer.Write("resultParameter2 = null\r\n");
                    writer.Write("resultParameter3 = null\r\n");
                    writer.Write("resultParameter4 = null\r\n");
                    writer.Write("resultParameter5 = null\r\n");                    
                }
            }

        }
    }
}
