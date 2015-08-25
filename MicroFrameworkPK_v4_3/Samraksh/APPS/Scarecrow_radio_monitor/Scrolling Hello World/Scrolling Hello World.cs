// Copyright 2013 The Samraksh Company http://samraksh.com

using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;
using Samraksh.eMote.Net;
using Samraksh.eMote.DotNow;
//using Samraksh.eMote.RealTime;

namespace ReflexMaster {
    public class UnwrapMsg
    {
        public bool Response;
        public ushort MsgID;
        public ushort Src;
        public ushort dummySrc;

        public UnwrapMsg()
        {
        }
        public static int Size()
        {
            return 7;
        }
        public UnwrapMsg(byte[] rcv_msg, ushort size)
        {
            Response = rcv_msg[0] == 0 ? false : true;
            MsgID = (ushort)(rcv_msg[1] << 8);
            MsgID += (ushort)rcv_msg[2];
            Src = (ushort)(rcv_msg[3] << 8);
            Src += (ushort)rcv_msg[4];
            dummySrc = (ushort)(0xefef);
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

    public class Program
    {
        private static UInt16 myAddress;
        private static Samraksh.eMote.Net.Mac.CSMA myCSMA;
        private static ReceiveCallBack myReceiveCB;
        private static NeighborhoodChangeCallBack myNeighborCB;
        private static Samraksh.eMote.Net.Mac.MacConfiguration macConfig = new Samraksh.eMote.Net.Mac.MacConfiguration();

        private static EmoteLCD lcd = new EmoteLCD();
    
 
        static void NeighborChange(UInt16 noOfNeigbors)
        {
            Debug.Print("Neighbor change: " + noOfNeigbors.ToString() + "\r\n");

            UInt16[] neighborlist = myCSMA.GetNeighborList();

            if (neighborlist == null)
            {
                Debug.Print("The Neighborlist is null \n");
            }
            else
            {

                Debug.Print("My Neighbours are : ");

                for (int i = 0; i < 12; i++)
                {
                    if (neighborlist[i] != 0)
                    {
                        Debug.Print(neighborlist[i].ToString() + " ");
                    }
                }

                Debug.Print("\n");
            }
        }

        static void HandleMessage(byte[] msg, UInt16 size, UInt16 src, bool unicast, byte rssi, byte lqi)
        {
            if (size == UnwrapMsg.Size())
            {
                UnwrapMsg rcvMsg = new UnwrapMsg(msg, size);
                Debug.Print(rcvMsg.MsgID.ToString());
            }        
        }

        static void Receive(UInt16 noOfPackets)
        {
            if (myCSMA.GetPendingPacketCount() == 0)
            {
                Debug.Print("no packets");
                return;
            }

            Message rcvMsg = myCSMA.GetNextPacket();
            if (rcvMsg == null)
            {
                Debug.Print("null");
                return;
            }

            byte[] rcvPayload = rcvMsg.GetMessage();
            HandleMessage(rcvPayload, (UInt16)rcvMsg.Size, rcvMsg.Src, rcvMsg.Unicast, rcvMsg.RSSI, rcvMsg.LQI);
        }

        public static void Main()
        {
            lcd.Initialize();

            macConfig.NeighborLivenessDelay = 180;
            macConfig.CCASenseTime = 140; //Carries sensing time in micro seconds
            myReceiveCB = Receive;
            myNeighborCB = NeighborChange;
            Samraksh.eMote.Net.Mac.CSMA.Configure(macConfig, myReceiveCB, myNeighborCB);
            myCSMA = Samraksh.eMote.Net.Mac.CSMA.Instance;
            Debug.Print("CSMA Init done.");
            myAddress = myCSMA.GetAddress();
            Debug.Print("My default address is :  " + myAddress.ToString());

            while (true)
            {
                
            }
        }

    }
}

