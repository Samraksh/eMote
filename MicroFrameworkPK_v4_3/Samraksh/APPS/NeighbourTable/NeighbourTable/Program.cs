using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;
using System.Threading;
using Samraksh.SPOT.Net.Mac;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware.EmoteDotNow;

namespace Samraksh.SPOT.Tests
{
    public class PingMsg
    {
        public bool Response;
        public ushort MsgID;
        public UInt16 Src;
        public UInt16 dummySrc;

        public PingMsg()
        {
        }
        public static int Size()
        {
            return 7;
        }
        public PingMsg(byte[] rcv_msg, ushort size)
        {
            Response = rcv_msg[0] == 0 ? false : true;
            MsgID = (UInt16)(rcv_msg[1] << 8);
            MsgID += (UInt16)rcv_msg[2];
            Src = (UInt16)(rcv_msg[3] << 8);
            Src += (UInt16)rcv_msg[4];
            dummySrc = (UInt16)(0xefef);

        }

        public byte[] ToBytes()
        {
            byte[] b_msg = new byte[7];
            b_msg[0] = Response ? (byte)1 : (byte)0;
            b_msg[1] = (byte)((MsgID >> 8) & 0xFF);
            b_msg[2] = (byte)(MsgID & 0xFF);
            b_msg[3] = (byte)((Src >> 8) & 0xFF);
            b_msg[4] = (byte)(Src & 0xFF);
            b_msg[5] = (byte)(0xef);
            b_msg[6] = (byte)(0xef);
            return b_msg;
        }
    }

    public class NeighbourTableTesting
    {
        OMAC myOMAC;
        EmoteLCD lcd = new EmoteLCD();
        int neighborCount;
    
        static OutputPort SendPort = new OutputPort((Cpu.Pin)30, true);
        static OutputPort ReceivePort = new OutputPort((Cpu.Pin)31, true);
        static OutputPort NeighbourCheckPort = new OutputPort((Cpu.Pin)24, true);

        UInt16 mySeqNo = 0;

        UInt16 myAddress;

        MacConfiguration omacConfig = null;

        PingMsg sendMsg = new PingMsg();

        ReceiveCallBack rcallback;
        
        //Timer sendTimer;

        NeighbourhoodChangeCallBack ncallback;

        void HandleMessage(UInt16 numberOfPackets)
        {
            ReceivePort.Write(true);
            ReceivePort.Write(false);
        }

        void HandleNeighbourChange(UInt16 neighboursChanged)
        {
            neighborCount = neighboursChanged;
			Debug.Print("Neighbor change: " + neighboursChanged.ToString() + "\r\n");
            NeighbourCheckPort.Write(true);
            NeighbourCheckPort.Write(false);

            UInt16[] neighbourlist = myOMAC.GetNeighbourList();

            if (neighbourlist == null)
            {
                Debug.Print("The Neighborlist is null \n");
            }
            else
            {

                Debug.Print("My Neighbours are : ");

                for (int i = 0; i < 12; i++)
                {
                    if (neighbourlist[i] != 0)
                    {
                        Debug.Print(neighbourlist[i].ToString() + " ");
                    }
                }

                Debug.Print("\n");
            }

            int onesDigit = neighborCount % 10;
            int tensDigit = ((neighborCount - onesDigit) % 100) / 10;           
            lcd.Write(LCD.CHAR_0, LCD.CHAR_0, LCD.CHAR_0 + tensDigit, LCD.CHAR_0 + onesDigit);
             
        }

        public NeighbourTableTesting()
        {
            omacConfig = new MacConfiguration();
            omacConfig.NeighbourLivelinesDelay = 60;
            lcd.Initialize();
            lcd.Write(LCD.CHAR_0, LCD.CHAR_0, LCD.CHAR_0, LCD.CHAR_0);

            rcallback = HandleMessage;

            ncallback = HandleNeighbourChange;

            if (OMAC.Configure(omacConfig, rcallback, ncallback) != DeviceStatus.Success)
            {
                Debug.Print("The OMAC Configure call failed \n");
            }

            try
            {
                myOMAC = OMAC.Instance;
            }
            catch (MacNotConfiguredException m)
            {
                Debug.Print("Exception in NeighbourTable Test : Mac not configured "+m.ToString()+"\n");
            }
            catch (Exception e)
            {
                Debug.Print("Unknown exception from mac grab instance "+e.ToString()+"\n");
            }

            myAddress = myOMAC.GetAddress();

            Debug.Print("OMAC Init done. \n");

        }

        void Send_Ping(PingMsg ping)
        {
            //UInt16 sender = ping.Src;
            //Debug.GC(true);
            Debug.Print("Sending Broadcast message : " + mySeqNo.ToString() + " from " + myAddress.ToString());

            ping.Response = false;
            ping.MsgID = mySeqNo++;
            ping.Src = myAddress;

            SendPort.Write(true);
            SendPort.Write(false);

            byte[] msg = ping.ToBytes();
            myOMAC.Send((UInt16)Addresses.BROADCAST, msg, 0, (ushort)msg.Length);



        }

        void updateDisplay()
        {
            int onesDigit = neighborCount % 10;
            int tensDigit = ((neighborCount - onesDigit) % 100) / 10;
            lcd.Write(LCD.CHAR_0, LCD.CHAR_0, LCD.CHAR_0 + tensDigit, LCD.CHAR_0 + onesDigit);
        }

        void Sender(Object state)
        {
            Send_Ping(sendMsg);
        }

        public void Run()
        {
            Debug.Print("Starting Timer ...\n");
            //sendTimer = new Timer(Sender, null, 0, 1000);
        }

        public static void Main()
        {
            NeighbourTableTesting ntest = new NeighbourTableTesting();

            ntest.Run();

            while (true)
            {
                ntest.updateDisplay();
                Thread.Sleep(5000);
                Debug.Print("Running...\r\n");                
            }

        }

    }
}
