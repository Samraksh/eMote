using System;
using Microsoft.SPOT;
using System.Threading;
using Microsoft.SPOT.Hardware;

using Samraksh.SPOT.Net;
//using Samraksh.SPOT.Hardware.EmoteDotNow;

namespace LinkEstimation
{
    public class EstMsg
    {
        public UInt16 Src;
        public UInt16 MsgID;
        public byte[] data;

        public EstMsg(UInt16 rndsize)
        {
            data = new byte[rndsize];
        }
        public EstMsg(byte[] rcv_msg, ushort size)
        {
            Debug.Print("Allocating heap bytes: " + (size - 4).ToString());
            data = new byte[size-4];
            Src = (UInt16)(rcv_msg[0] << 8);
            Src += (UInt16)rcv_msg[1];
            MsgID = (UInt16) (rcv_msg[2] << 8);
            MsgID += (UInt16)rcv_msg[3];
            Array.Copy(rcv_msg,4,data,0,size-4);  
        }

        public byte[] ToBytes()
        {
            byte[] b_msg = new byte[4+data.Length];
            b_msg[0] = (byte)((Src >> 8) & 0xFF);
            b_msg[1] = (byte)(Src & 0xFF);
            b_msg[2] = (byte)((MsgID >> 8) & 0xFF);
            b_msg[3] = (byte)(MsgID & 0xFF);
            data.CopyTo(b_msg, 4);
            return b_msg;
        }
    }

    public class LinkTable
    {
        public class Node {
            public UInt16 Src;
            public UInt16 LastSeqNo;
            public UInt32 Sent;
            public UInt32 Rcvd;
            public float PDR;
        };
        public const byte MaxNeighbors = 8;
        byte FreeNeighborIndex = 0;
        Node[] Table;

        public LinkTable()
        {
             Table = new Node[MaxNeighbors];
             for (int i = 0; i < MaxNeighbors; i++)
             {
                 Table[i] = new Node();
             }
            
        }

        
        public void PrintTable()
        {
            
            Debug.Print("-------------------------------------------------------------------");
            Debug.Print("|SRC ID | Last Seq No| Total Rvcd| PDR (%)|");
            Debug.Print("-------------------------------------------------------------------");
            
            for (int i = 0; i < MaxNeighbors; i++)
            {
                if (Table[i].Src != 0)
                {
                    Debug.Print("|" + Table[i].Src.ToString() + "\t|" + Table[i].LastSeqNo.ToString() + "\t|" + Table[i].Rcvd.ToString() + "\t|" + Table[i].PDR.ToString() + "\t|");                   
                }
            }

            Debug.Print("-------------------------------------------------------------------");
        }

        public void Update(UInt16 src, UInt16 SeqNo)
        {
            bool Found = false;
            //Find and update the neighbor in table
            //Debug.Print("Updating table for Src:  " + src.ToString() + " for SeqNo: " + SeqNo.ToString());
            for (int i = 0; i < MaxNeighbors; i++)
            {
                if (Table[i].Src == src)
                {
                    Found = true;
                    Table[i].Rcvd++;
                    //check for seqno roll over
                    if ((int)(SeqNo - Table[i].LastSeqNo) > 65000)
                    {
                        Table[i].Sent++;
                    }
                    else
                    {
                        //are there drops?
                        if (Table[i].LastSeqNo == (UInt16)(SeqNo - 1))
                        {
                            Table[i].Sent++;
                        }
                        else    //drops have happened
                        {
                            Table[i].Sent += (UInt32)(SeqNo - Table[i].LastSeqNo);
                        }
                    }
                    Table[i].LastSeqNo = SeqNo;
                    Table[i].PDR = (1 - (float)((float)Table[i].Rcvd / (float)Table[i].Sent)) * 100;
                }
            }
            //Neighbor not found, add new entry to table if possible
            if (!Found)
            {
                if (FreeNeighborIndex < MaxNeighbors)
                {
                    Table[FreeNeighborIndex].Src = src;
                    Table[FreeNeighborIndex].Rcvd++;
                    Table[FreeNeighborIndex].LastSeqNo = SeqNo;
                    Table[FreeNeighborIndex].PDR = 0;
                    FreeNeighborIndex++;
                }
            }
        }

    }

    public class Program
    {
        UInt16 myAddress;
        UInt16 mySeqNo = 0;
        Timer sendTimer, infoTimer;
        //EmoteLCD lcd;
        static LinkTable myLinkTable = new LinkTable();        
        Random rand = new Random();
        static System.DateTime startTime;
        static System.TimeSpan curTime;
        
        static OutputPort SendPort = new OutputPort((Cpu.Pin)30, true);
        static OutputPort ReceivePort = new OutputPort((Cpu.Pin)31, true);

        static Samraksh.SPOT.Net.Mac.CSMA myCSMA = new Samraksh.SPOT.Net.Mac.CSMA();
        //static Samraksh.SPOT.Net.Mac.NeighborTable myMacTable= new Samraksh.SPOT.Net.Mac.NeighborTable();
        

        Samraksh.SPOT.Net.Mac.MacConfiguration macConfig = new Samraksh.SPOT.Net.Mac.MacConfiguration();
        ReceiveCallBack myReceive;

        void Initialize()
        {
            Debug.Print("Initializing:  EmotePingwLCD");
            Thread.Sleep(1000);

            startTime = System.DateTime.Now;
            /*lcd = new EmoteLCD();
            lcd.Initialize();
            lcd.Write(LCD.CHAR_I, LCD.CHAR_N, LCD.CHAR_I, LCD.CHAR_7);
            */
            myReceive = HandleMessage; //Assign the delegate to a function

            Debug.Print("Initializing:  CSMA...");
            try
            {
                myCSMA.Initialize(macConfig, myReceive);
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }
            Debug.Print("CSMA Init done.");
            myAddress = myCSMA.GetAddress();
            Debug.Print("My default address is :  " + myAddress.ToString());

        }
        public bool LookUpMacTable(UInt16 node)
        {
            Samraksh.SPOT.Net.Mac.Neighbor link = new Samraksh.SPOT.Net.Mac.Neighbor();
            bool ret;
            //Debug.Print("Looking up status of node: " + node.ToString());
            link.Status = Samraksh.SPOT.Net.Mac.NeighborStatus.Suspect;
            ret =myCSMA.GetNeighborStatus(node, ref link);
            if (ret)
            {
                Debug.Print("Looked up the link status for node: " + link.MacAddress.ToString());
                Debug.Print(" Link | Status |Packets | RSS | LQI | Delay|");
                Debug.Print("Revese |"+  link.Status.ToString() + "|" + link.PacketsReceived.ToString() + "|" + link.ReverseLink.AveRSSI.ToString() + "|" + link.ReverseLink.LinkQuality.ToString() + "|" + link.ReverseLink.AveDelay.ToString() + "|");
            }
            else
            {
                Debug.Print("Link status look up failed for node: " + node.ToString());
            }
            return ret;
        }

        /*public void PrintMacTable()
        {
            try
            {
                myMacTable = myCSMA.GetNeighborTable();
                Debug.Print("-------------------------------------------------------------------");
                Debug.Print("|SRC ID | Last Time Heard| Status| Quality (0-255)|");
                Debug.Print("-------------------------------------------------------------------");

                for (byte i = 0; i < myMacTable.NumberValidNeighbor; i++)
                {
                    if (myMacTable.Neighbor[i].MacAddress != 0)
                    {
                        Debug.Print("|" + myMacTable.Neighbor[i].MacAddress.ToString() + "\t|" + myMacTable.Neighbor[i].LastHeardTime.ToString() + "\t|" + myMacTable.Neighbor[i].Status.ToString() + "\t|" + myMacTable.Neighbor[i].ReverseLink.Quality.ToString() + "\t|");
                    }
                }
                Debug.Print("-------------------------------------------------------------------");
            }
            catch (Exception e)
            {
                Debug.Print("Exception getting MAC's Table: " + e.ToString());
            }
        }*/


        void Start()
        {
            Debug.Print("Starting timer...");
            sendTimer = new Timer(new TimerCallback(sendTimerCallback), null, 0, 100);
            infoTimer = new Timer(new TimerCallback(infoTimerCallback), null, 0, 10000);
            Debug.Print("Timer init done.");
        }

        private void infoTimerCallback(object state)
        {
            curTime = System.DateTime.Now - startTime;
            Debug.Print("Link State at Time:  " + curTime.Hours + ":" + curTime.Minutes + ":" + curTime.Seconds + " from start.");
            
            myLinkTable.PrintTable();
            //PrintMacTable();
            
        }

        void sendTimerCallback(Object o)
        {
            //mySeqNo++;
            if (mySeqNo % 50 == 0)
            {
                Debug.Print("Sending broadcast ping msg:  " + mySeqNo.ToString());
            }
            try
            {
                SendBroadcast();
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }

        }

        private void SendBroadcast()
        {
            mySeqNo++;
            //EstMsg msg = new EstMsg((UInt16)(rand.Next() % 100));
            EstMsg msg = new EstMsg(40);
            rand.NextBytes(msg.data);
            msg.MsgID = mySeqNo;
            msg.Src = myAddress;
            byte[] byteMsg =  msg.ToBytes();
            myCSMA.Send((UInt16)Samraksh.SPOT.Net.Mac.Addresses.BROADCAST, byteMsg, 0, (ushort)byteMsg.Length);
            SendPort.Write(true);
            SendPort.Write(false);

            //int char0 = (mySeqNo % 10) + (int)LCD.CHAR_0;
            //lcd.Write(LCD.CHAR_S, LCD.CHAR_S, LCD.CHAR_S, (LCD)char0);
        }

        void HandleMessage(byte[] msg, UInt16 size, UInt16 src, bool unicast, byte rssi, byte lqi)
        {
            try
            {
                if (unicast) { }
                else
                {
                    Debug.Print("Got a broadcast message from src: " + src.ToString() + ", size: " + size.ToString() + ", rssi: " + rssi.ToString() + ", lqi: " + lqi.ToString());
                }
            
                UInt16 Src, MsgID;
                if (size >= 4)
                {
                    Src = (UInt16)(msg[0] << 8);
                    Src += (UInt16)msg[1];
                    MsgID = (UInt16)(msg[2] << 8);
                    MsgID += (UInt16)msg[3];

                    //EstMsg rcvMsg = new EstMsg(msg, size);
                    //myLinkTable.Update(Src, MsgID);
                    LookUpMacTable(src);

                    //Debug.Print("Received est msg from : " + Src.ToString() + " with seq no: " + MsgID.ToString());
                    //lcd.Write(LCD.CHAR_P, LCD.CHAR_P, LCD.CHAR_P, LCD.CHAR_P);
                }
               

                ReceivePort.Write(true);
                ReceivePort.Write(false);
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }
        }
   
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        public static void Main()
        {
            Debug.Print("Changing app");
            Program p = new Program();
            p.Initialize();
            p.Start();
            Thread.Sleep(Timeout.Infinite);
        }

    }
}
