using System;
using Microsoft.SPOT;
using System.Threading;
using Microsoft.SPOT.Hardware;

using Samraksh.SPOT.Net;
using Samraksh.SPOT.Hardware.EmoteDotNow;

//Program assumes that there is a single sender sending 1200 packets at different power-levels
//The sender ID is set statically in SENDER_ADDRESS
//10 packets sent per second, each power-level is used to 1200 packets or 2-mins.
//Receiver calculates the number of packets received at each power level and puts it in an array and displays the PRR at each level
//Once instance of the test takes approx 10 mins.

//Test Methodoloy;
//The receiver remains fixed and connected to a laptop and the results can be read through serial port connected to MFDeploy
//The Sender connected to battery can move. A person walks to a given distance with sender. The receiver is started first, then the sender is started.
//The sender remains at this location for 10 mins. The receiver estimates PRR at 5 different power levels and prints through serial
//The person moves to the next location with laptop.



namespace RagngeEstimation
{
    public enum MsgType
    {
        RESET,
        EST
    };

    public class TxPwrMsg
    {
        public UInt16 Src;
        public byte PwrIndex;
        public UInt16 MsgID;    
        public byte[] data;
     

        public TxPwrMsg(UInt16 rndsize)
        {
            data = new byte[rndsize];
        }
        public TxPwrMsg(byte[] rcv_msg, ushort size)
        {
            //Debug.Print("Allocating heap bytes: " + (size - 5).ToString());
            data = new byte[size-5];
            Src = (UInt16)(rcv_msg[0] << 8);
            Src += (UInt16)rcv_msg[1];
            PwrIndex = (byte)rcv_msg[2];
            MsgID = (UInt16) (rcv_msg[3] << 8);
            MsgID += (UInt16)rcv_msg[4];
            Array.Copy(rcv_msg,5,data,0,size-5);
        }

        public byte[] ToBytes()
        {
            byte[] b_msg = new byte[5+data.Length];
            b_msg[0] = (byte)((Src >> 8) & 0xFF);
            b_msg[1] = (byte)(Src & 0xFF);
            b_msg[2] = (byte)(PwrIndex);
            b_msg[3] = (byte)((MsgID >> 8) & 0xFF);
            b_msg[4] = (byte)(MsgID & 0xFF);
            data.CopyTo(b_msg, 5);
            return b_msg;
        }
    }

    public class ReceivedPackets
    {
        public UInt16[] ReceivedPkts = new UInt16[5];
        public UInt16 RcvPowerIndex;    
    };

    public class Program
    {
        UInt16 myAddress;
        UInt16 mySeqNo = 0;
        Timer sendTimer, infoTimer;
        EmoteLCD lcd;
        const UInt16 MAX_PKTS = 1200;
        //const UInt16 SENDER_ADDRESS = 7123;
        const UInt16 SENDER_ADDRESS = 3785;    
        Random rand = new Random();
        static System.DateTime startTime;
        static System.TimeSpan curTime;
        bool Sender = false;
        bool Receiver = true;
        byte[] PowerLevels = new byte [] {15, 12, 8, 4, 0};
        byte PowerIndex = 0;
        ReceivedPackets rcvPkts = new ReceivedPackets();
        static OutputPort SendPort = new OutputPort((Cpu.Pin)30, true);
        static OutputPort ReceivePort = new OutputPort((Cpu.Pin)31, true);

        static Samraksh.SPOT.Net.Mac.CSMA myCSMA = new Samraksh.SPOT.Net.Mac.CSMA();        

        Samraksh.SPOT.Net.Mac.MacConfiguration macConfig = new Samraksh.SPOT.Net.Mac.MacConfiguration();
        ReceiveCallBack myReceive;


        public void PrintTable()
        {

            Debug.Print("-------------------------------------------------------------------");
            Debug.Print("|Power Level | Total Rvcd| PRR (%)|");
            Debug.Print("-------------------------------------------------------------------");

            for (int i = 0; i < rcvPkts.ReceivedPkts.Length; i++)
            {
                float prr = (float)(rcvPkts.ReceivedPkts[i]*100) / MAX_PKTS ;
                Debug.Print("|" + PowerLevels[i].ToString() + "\t|" + rcvPkts.ReceivedPkts[i].ToString() + "\t|" + prr.ToString() + "\t|");
            }

            Debug.Print("-------------------------------------------------------------------");
        }


        void Initialize()
        {
            Debug.Print("Initializing:  RangeEstimateTxPower");
            Thread.Sleep(1000);

            startTime = System.DateTime.Now;
            lcd = new EmoteLCD();
            lcd.Initialize();
            lcd.Write(LCD.CHAR_I, LCD.CHAR_N, LCD.CHAR_I, LCD.CHAR_7);
            
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
               
        void Start()
        {
            Debug.Print("Starting timer...");
            
            PowerIndex = 0xFF;
            if (myAddress == SENDER_ADDRESS) { Sender = true; }
            else {Sender = false;}
            if (Sender)
            {
                SwitchPower();
                sendTimer = new Timer(new TimerCallback(sendTimerCallback), null, 0, 100);
            }
            else
            {
                infoTimer = new Timer(new TimerCallback(infoTimerCallback), null, 0, 20000);
            }
            Debug.Print("Timer init done.");
        }

        private void infoTimerCallback(object state)
        {
            curTime = System.DateTime.Now - startTime;
            Debug.Print("PRR Estimate at Time:  " + curTime.Hours + ":" + curTime.Minutes + ":" + curTime.Seconds + " from start.");
            PrintTable();           
        }

        void sendTimerCallback(Object o)
        {
            if (mySeqNo == MAX_PKTS)
            {
                SwitchPower();
                return;
            }
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
            TxPwrMsg msg = new TxPwrMsg(40);
            rand.NextBytes(msg.data);
            msg.PwrIndex = PowerIndex;
            msg.MsgID = mySeqNo;
            msg.Src = myAddress;
            byte[] byteMsg =  msg.ToBytes();
            myCSMA.Send((UInt16)Samraksh.SPOT.Net.Mac.Addresses.BROADCAST, byteMsg, 0, (ushort)byteMsg.Length);
            SendPort.Write(true);
            SendPort.Write(false);
            int char0 = (mySeqNo % 10) + (int)LCD.CHAR_0;
            int char1 = (mySeqNo % 100) + (int)LCD.CHAR_0;
            int char2 = (mySeqNo % 1000) + (int)LCD.CHAR_0;
            int char3 = (mySeqNo % 10000) + (int)LCD.CHAR_0;
            lcd.Write((LCD)char3, (LCD)char2, (LCD)char1, (LCD)char0);     
        }

        void SwitchPower()
        {
            int i=0;
            if (sendTimer != null)
            {
                sendTimer.Dispose();
            }
            PowerIndex++;

            if (PowerIndex > PowerLevels.Length)
            {
                Debug.Print("I am done testing. Good Bye.");
            }
            else
            {
                while (i < 10)
                {
                    Debug.Print("Switching Power to: " + PowerLevels[PowerIndex].ToString());
                    int char0 = (PowerLevels[PowerIndex] % 10) + (int)LCD.CHAR_0;
                    int char1 = (PowerLevels[PowerIndex] / 10) + (int)LCD.CHAR_0;
                    lcd.Write(LCD.CHAR_0, LCD.CHAR_0, (LCD)char1, (LCD)char0);
                    Thread.Sleep(200);
                    i++;
                }
                //Switch Power to new powerlevel

                Debug.Print("Setting Power to: " + PowerLevels[PowerIndex].ToString());
                myCSMA.SetTxPower(PowerLevels[PowerIndex]);

                mySeqNo = 0;
                if (Sender)
                {
                    sendTimer = new Timer(new TimerCallback(sendTimerCallback), null, 0, 100);
                }
            }
        }
        void HandleMessage(byte[] msg, UInt16 size, UInt16 src, bool unicast, byte rssi, byte lqi)
        {
            try
            {
                /*if (unicast) { }
                else
                {
                    Debug.Print("Got a broadcast message from src: " + src.ToString() + ", size: " + size.ToString() + ", rssi: " + rssi.ToString() + ", lqi: " + lqi.ToString());
                }
                */
                
                if (size >= 5)
                {
                    TxPwrMsg rcvMsg = new TxPwrMsg(msg, size);
                    rcvPkts.ReceivedPkts[rcvMsg.PwrIndex]++;
                    //Debug.Print("Pwr Index : " + rcvMsg.PwrIndex.ToString() + " , Total received " + rcvPkts.ReceivedPkts[rcvMsg.PwrIndex].ToString());
                    
                    if (rcvMsg.MsgID % 50 == 0)
                    {
                        Debug.Print("Received est msg from : " + src.ToString() + " with seq no: " + rcvMsg.MsgID.ToString());
                    }
                    if (rcvMsg.MsgID % 2 == 0)
                    {
                        lcd.Write(LCD.CHAR_R, LCD.CHAR_0, LCD.CHAR_R, LCD.CHAR_0);
                    }
                    else {
                        lcd.Write(LCD.CHAR_0, LCD.CHAR_R, LCD.CHAR_0, LCD.CHAR_R);
                    }    
                }

                ReceivePort.Write(true);
                ReceivePort.Write(false);
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }
        }
       
        /// <summary>
        /// Main Starts here
        /// </summary>
        
        
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
