using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;
using System.Threading;
using Samraksh.SPOT.Net.Mac;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware.EmoteDotNow;

namespace MakerFairSensor
{
    public delegate void EncoderCallback(bool Direction);

    public class Program
    {
        static EncoderCallback RotaryCallback = new EncoderCallback(UpdatePosition);
        static bool SendPending = false;
        RotaryEncoder Encoder = new RotaryEncoder(RotaryCallback);
        const int MaxDegree = 180;
        static byte CurrentPosition = 0;

        static OutputPort DebugPortB = new OutputPort((Cpu.Pin)25, true);
        static InputPort inputPort = new InputPort((Cpu.Pin)24, false, Port.ResistorMode.PullUp);
        bool outputVal = false;
        UInt16 myAddress;
        UInt16 mySeqNo = 1;
        Timer sendTimer;
        EmoteLCD lcd;
        Random rand = new Random();

        static CSMA myCSMA;
        ReceiveCallBack myReceiveCB;
        NeighbourhoodChangeCallBack myNeighborCB;
        int neighborCount = 0;

        MacConfiguration macConfig = new MacConfiguration();

        void Initialize()
        {
            Debug.Print("Initializing:  Maker Faire Controller");
            Thread.Sleep(1000);
            lcd = new EmoteLCD();
            lcd.Initialize();
            UpdateDisplay();

            macConfig.NeighbourLivelinesDelay = 20;
            macConfig.CCASenseTime = 140; //Carries sensing time in micro seconds

            Debug.Print("Configuring:  CSMA...");
            try
            {
                myReceiveCB = Receive;
                myNeighborCB = NeighborChange;
                CSMA.Configure(macConfig, myReceiveCB, myNeighborCB);
                myCSMA = CSMA.Instance;
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
            sendTimer = new Timer(new TimerCallback(sendTimerCallback), null, 0, 200);
            Debug.Print("Timer init done.");
        }

        static void UpdatePosition(bool direction)
        {
        	SendPending=true;
            if (direction) {
                if(CurrentPosition <= MaxDegree -1)
                    CurrentPosition++;
            }
            else {
                if (CurrentPosition > 0)
                    CurrentPosition--;
            }
        }

        void sendTimerCallback(Object o)
        {
            DebugPortB.Write(outputVal);
            outputVal = !outputVal;
            if ((mySeqNo % 50) == 0)
                Debug.Print("Sending broadcast sensor msg:  " + mySeqNo.ToString());
            try
            {
                Send_SensorMsg();
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }
        }

        void NeighborChange(UInt16 noOfNeigbors)
        {
            neighborCount = noOfNeigbors;
            Debug.Print("NeighborChange: " + neighborCount.ToString());

            UpdateDisplay();
        }

        void UpdateDisplay()
        {
            int onesDigit = mySeqNo % 10;
            int tensDigit = ((mySeqNo - onesDigit) % 100) / 10;
            lcd.Write(LCD.CHAR_0 + (neighborCount % 10), LCD.CHAR_NULL, LCD.CHAR_0 + tensDigit, LCD.CHAR_0 + onesDigit);
        }

        void Receive(UInt16 noOfPackets)
        {
        }

        void HandleMessage(byte[] msg, UInt16 size, UInt16 src, bool unicast, byte rssi, byte lqi)
        {
        }

        void Send_SensorMsg()
        {
            if (SendPending)
            {
                mySeqNo++;
                byte[] msg = new byte[2];
                msg[0] = (byte)(CurrentPosition * 3);
                msg[1] = 174;
                SendPending = false;
                CurrentPosition = 0;
                myCSMA.Send((UInt16)Addresses.BROADCAST, msg, 0, (ushort)msg.Length);
            }            
            /*if (PreemptiveGC >= 10)
            {
                Debug.GC(true);
                PreemptiveGC = 0;
            }
            PreemptiveGC++;*/
            UpdateDisplay();
        }

        public static void Main()
        {
            bool lastRead = false;
            bool currentRead;
            Program p = new Program();
            p.Initialize();
            p.Start();
            while (true)
            {                
                currentRead = inputPort.Read();
                //Debug.Print("input port: " + currentRead.ToString());
                if (currentRead != lastRead)
                {
                    //Debug.Print("pin changed to: " + currentRead.ToString());
                    if (currentRead == true)
                    {
                        UpdatePosition(true);
                    }
                    lastRead = currentRead;
                }
                //Thread.Sleep(100);
            }
        }
    }
}
