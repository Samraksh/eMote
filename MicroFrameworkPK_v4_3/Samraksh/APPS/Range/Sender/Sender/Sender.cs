using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;
using System.Threading;

namespace Sender
{
    public class Sender
    {
        public static Samraksh.SPOT.Net.Mac.CSMA csmaObject = new Samraksh.SPOT.Net.Mac.CSMA();
        public static bool recieved = false;

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

            payload[0] = (byte)((1 >> 8) & 0xff);
            payload[1] = (byte)(1 & 0xff);

            while (true)
            {
                recieved = false;
                payload[2] = (byte)((seqNum >> 8) & 0xff);
                payload[3] = (byte)(seqNum & 0xff);
                seqNum++;
                payload[4] = 0;
                csmaObject.Send(0xffff, payload, 0, 5);

                Timer watchdog = new Timer(timerCallback, null, 0, 1000);

                while (!recieved)
                {
                    Thread.Sleep(10);
                }
                

            }

        }

        public static void timerCallback(Object state)
        {
            recieved = true;

            Debug.Print("Did not see reciever, timing out and retrying");

            //lcdObject.display(NULL, NULL, NULL, 2);
        }

        public static void MessageRecievedHandler(byte[] message, ushort size)
        {
            recieved = true;

            Debug.Print("Saw Reciever");
            //lcdObject.display(NULL,NULL,NULL,1);
        }

    }
}
