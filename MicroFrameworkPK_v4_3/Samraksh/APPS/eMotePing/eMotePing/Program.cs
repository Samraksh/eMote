using System;
using Microsoft.SPOT;
using System.Threading;

using Samraksh.SPOT.Net;


namespace Samraksh.SPOT.Net.Mac.Ping
{      
    public class PingMsg 
    {
        public bool Response;
        public ushort MsgID;
        public UInt16 Src;

        public PingMsg()
        {
        }
        public PingMsg(byte[] rcv_msg, ushort size)
        {
            Response = rcv_msg[0]==0 ? false: true ;
            MsgID = (UInt16) (rcv_msg[1] << 8);
            MsgID += (UInt16)rcv_msg[2];
            Src = (UInt16)(rcv_msg[3] << 8);
            Src += (UInt16)rcv_msg[4];
        }

        public byte[] ToBytes()
        {
            byte[] b_msg = new byte[5];
            b_msg[0] = Response ? (byte)1: (byte)0;
            b_msg[1] = (byte) ((MsgID >> 8) & 0xFF);
            b_msg[2] = (byte)(MsgID & 0xFF);
            b_msg[3] = (byte)((Src >> 8) & 0xFF);
            b_msg[4] = (byte)(Src & 0xFF);
            return b_msg;
        }
    }

    public class Program
    {  
        UInt16 myAddress;
        UInt16 mySeqNo = 0;
        Timer sendTimer;
        PingMsg sendMsg = new PingMsg();

        Radio.Radio_802_15_4 my_15_4 = new Radio.Radio_802_15_4();
        Radio.RadioConfiguration radioConfig = new Radio.RadioConfiguration();
        int myRadioID;

        Mac.CSMA myCSMA = new CSMA();
        Mac.MacConfiguration macConfig = new MacConfiguration();
        ReceiveCallBack myReceive;

        void Initialize()
        {
            my_15_4.Initialize(radioConfig, null);
            myRadioID = my_15_4.GetID();

            macConfig.CCA = true;
            macConfig.BufferSize = 8;
            macConfig.NumberOfRetries = 0;
            macConfig.RadioID = (byte) myRadioID;
            macConfig.CCASenseTime = 140; //Carries sensing time in micro seconds

            myReceive = HandleMessage; //Assign the delegate to a function

            myCSMA.Initialize(macConfig, myReceive);
            myAddress = myCSMA.GetAddress();
        }
        void Start()
        {
            sendTimer = new Timer(new TimerCallback(sendTimerCallback), null, 1000, 1000);  
        }
        
        void sendTimerCallback(Object o)
        {
            Send_Ping(sendMsg);
        }

        void HandleMessage(byte[] msg, ushort size)
        {
            PingMsg rcvMsg = new PingMsg(msg,size);
            Send_Pong(rcvMsg);
        }
   
        void Send_Pong(PingMsg ping)
        {
            UInt16 sender = ping.Src;
            ping.Response = true;

            ping.Src = myAddress;

            byte[] msg = ping.ToBytes();
            myCSMA.Send(sender, msg, 0, (ushort)msg.Length);
        }

        void Send_Ping(PingMsg ping)
        {
            //UInt16 sender = ping.Src;
            ping.Response = false;
            ping.MsgID=mySeqNo++;
            ping.Src = myAddress;

            byte[] msg = ping.ToBytes();
            myCSMA.Send((UInt16)Mac.Addresses.BROADCAST, msg, 0, (ushort)msg.Length);
        }

        public static void Main()
        {
            Program p = new Program();
            p.Initialize();
            p.Start();
        }
    }
}
