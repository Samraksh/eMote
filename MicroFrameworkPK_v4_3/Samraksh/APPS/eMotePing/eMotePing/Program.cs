using System;
using Microsoft.SPOT;
using System.Threading;

using Samraksh.SPOT.Net;
using Samraksh.SPOT.Hardware.EmoteDotNow;

namespace Samraksh.SPOT.Net.Mac.Ping
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
        public PingMsg(byte[] rcv_msg, ushort size)
        {
            Response = rcv_msg[0]==0 ? false: true ;
            MsgID = (UInt16) (rcv_msg[1] << 8);
            MsgID += (UInt16)rcv_msg[2];
            Src = (UInt16)(rcv_msg[3] << 8);
            Src += (UInt16)rcv_msg[4];
            dummySrc = (UInt16)(0xefef);
            
        }

        public byte[] ToBytes()
        {
            byte[] b_msg = new byte[7];
            b_msg[0] = Response ? (byte)1: (byte)0;
            b_msg[1] = (byte) ((MsgID >> 8) & 0xFF);
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
        UInt16 myAddress;
        UInt16 mySeqNo = 0;
        Timer sendTimer;
        EmoteLCD lcd;
        PingMsg sendMsg = new PingMsg();

        //Radio.Radio_802_15_4 my_15_4 = new Radio.Radio_802_15_4();
        //Radio.RadioConfiguration radioConfig = new Radio.RadioConfiguration();
        //int myRadioID;

        Mac.CSMA myCSMA = new CSMA();
        Mac.MacConfiguration macConfig = new MacConfiguration();
        ReceiveCallBack myReceive;

        void Initialize()
        {
            
            Debug.Print("Initializing:  EmotePingwLCD");
            Thread.Sleep(1000);
            lcd = new EmoteLCD();
            lcd.Initialize();
            lcd.Write(LCD.CHAR_I, LCD.CHAR_N, LCD.CHAR_I, LCD.CHAR_7);
           
            /*
            Debug.Print("Initializing:  Radio");
            try
            {
                my_15_4.Initialize(radioConfig, null);
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }

            Debug.Print("Radio init done.");

            myRadioID = my_15_4.GetID();
             *
            Debug.Print("My radio ID is : " + myRadioID);
            */
         
            macConfig.CCA = true;
            macConfig.BufferSize = 8;
            macConfig.NumberOfRetries = 0;
            //macConfig.RadioID = (byte) myRadioID;
            macConfig.RadioID = (byte)1;
            macConfig.CCASenseTime = 140; //Carries sensing time in micro seconds

            myReceive = HandleMessage; //Assign the delegate to a function
            
            Debug.Print("Initializing:  CSMA...");
            try{
                  myCSMA.Initialize(macConfig, myReceive);
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }
            Debug.Print("CSMA Init done.");
            myAddress = myCSMA.GetAddress();
            Debug.Print("My default address is :  " + myAddress.ToString());

            /*myCSMA.SetAddress(52);
            myAddress = myCSMA.GetAddress();
            Debug.Print("My New address is :  " + myAddress.ToString());
             */ 
           
        }
        void Start()
        {
            Debug.Print("Starting timer...");
            sendTimer = new Timer(new TimerCallback(sendTimerCallback), null, 0, 2000);
            Debug.Print("Timer init done.");
        }
        
        void sendTimerCallback(Object o)
        {
            //mySeqNo++;
            Debug.Print("Sending broadcast ping msg:  " + mySeqNo.ToString());
            try
            {
                Send_Ping(sendMsg);
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }

        }

        void HandleMessage(byte[] msg, ushort size)
        {
            try
            {
                PingMsg rcvMsg = new PingMsg(msg, size);

                if (rcvMsg.Response)
                {
                    //This is a response to my message
                    Debug.Print("Received response from: " + rcvMsg.Src.ToString());
                    lcd.Write(LCD.CHAR_P, LCD.CHAR_P, LCD.CHAR_P, LCD.CHAR_P);
                }
                else
                {
                    Debug.Print("Sending a Pong to SRC: " + rcvMsg.Src.ToString());
                    lcd.Write(LCD.CHAR_R, LCD.CHAR_R, LCD.CHAR_R, LCD.CHAR_R);
                    Send_Pong(rcvMsg);
                }
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }
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
            lcd.Write(LCD.CHAR_S, LCD.CHAR_S, LCD.CHAR_S, LCD.CHAR_S);
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