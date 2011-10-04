using System;
using System.IO;
using System.Text;
using System.Collections;
using System.Reflection;
using System.Threading;
using System.Diagnostics;
using System.IO.Ports;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using MyEncryptionLib;

namespace EncryptTest
{
    public class Program
    {
        public static void Main()
        {
            Debug.Print("Testing SSL...");
			SSL myEncrypt = new SSL();	
		    string toEncrypt = "";
            int strLen = toEncrypt.Length;
            int nofPackets = strLen / 16;
            //int start = 0, end = 0;
			
            string encryptPacket = "";
			myEncrypt.encrypt(encryptPacket); 
         /*
		 for (int i = 0; i < nofPackets; i++)
            {
                start = i * 16;
                end = (i + 1) * 16;
                Debug.Print("NoofPackets: " + nofPackets + " Packet: " + i);
                encryptPacket = toEncrypt.Substring(start, 16);
                //Debug.Print(encryptPacket);
                myEncrypt.encrypt(encryptPacket); 
                for (int j = 0; j < 10000; j++) ;
            } */
      //      myEncrypt.encrypt(toEncrypt);           
			Debug.Print("Done!");
        }

    }
}
