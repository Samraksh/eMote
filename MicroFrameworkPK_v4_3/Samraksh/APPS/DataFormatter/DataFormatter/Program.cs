using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace DataFormatter
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Enter the file to be parsed : ");
            string fileName = Console.ReadLine();

            byte[] startOfRecordDelimiter = new byte[4];
            byte[] endOfRecordDelimiter = new byte[4];

            startOfRecordDelimiter[0] = 0xff;
            //startOfRecordDelimiter[1] = 0x00;
            //startOfRecordDelimiter[2] = 0x00;
            //startOfRecordDelimiter[3] = 0xf0;

            endOfRecordDelimiter[0] = 0xfe;
            //endOfRecordDelimiter[1] = 0x00;
            //endOfRecordDelimiter[2] = 0xad;
            //endOfRecordDelimiter[3] = 0xda;

            UInt16 start  = 0;
            UInt16 end = 0;

            byte[] inputArray = new byte[10000];

            byte[] startDelimiter = new byte[4];
            byte[] endDelimiter = new byte[4];

            UInt16 k = 0;

            int pos = 0;

            using (BinaryReader reader = new BinaryReader(File.Open(fileName, FileMode.Open)))
            {

                int length = (int)reader.BaseStream.Length;
                while (pos < length)
                {
                    k += 2;

                    string temp = reader.ReadString();

                    
                
                    pos += temp.Length;

                }

            }

            

        }
    }
}
