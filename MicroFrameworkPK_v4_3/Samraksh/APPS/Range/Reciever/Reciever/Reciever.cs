using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;
using System.Threading;


namespace Reciever
{
    public class Reciever
    {
        public static Samraksh.SPOT.Net.Mac.CSMA csmaObject = new Samraksh.SPOT.Net.Mac.CSMA();

        public static void Main()
        {
            Samraksh.SPOT.Net.Mac.MacConfiguration macconfig;
            byte[] payload = new byte[5];
            ushort seqNum = 0;

            macconfig.CCA = true;
            macconfig.NumberOfRetries = 5;
            macconfig.CCASenseTime = 160;
            macconfig.BufferSize = 16;
            macconfig.RadioID = 1;

            ReceiveCallBack MessageRecieved = MessageRecievedHandler;

            csmaObject.Initialize(macconfig, MessageRecieved);

            while (true)
            {
                Thread.Sleep(1000);
            }

        }

        public static void MessageRecievedHandler(byte[] message, ushort size)
        {
            csmaObject.Send(0xffff, message, 0, size);

            Debug.Print("Saw Reciever");
            //lcdObject.display(NULL,NULL,NULL,1);
        }

    }
}
