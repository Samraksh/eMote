using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;
using System.Threading;
using Samraksh.SPOT.Net.Mac;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware.EmoteDotNow;

namespace MakerFairController
{    
    public class Program
    {
        UInt16 myAddress;
        EmoteLCD lcd;

        static CSMA myCSMA;
        ReceiveCallBack myReceiveCB;
        NeighbourhoodChangeCallBack myNeighborCB;
        int neighborCount = 0;
        UInt16 mySeqNo = 1;

        Message rcvMsg;
        byte[] rcvPayload;

        MacConfiguration macConfig = new MacConfiguration();

        ControlLoop ServoControl = new ControlLoop();

        void Initialize()
        {
            Debug.Print("Initializing:  MakerFairController");
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

            ServoControl.Start();
        }

        void NeighborChange(UInt16 noOfNeigbors)
        {
            neighborCount = noOfNeigbors;
            Debug.Print("NeighborChange: " + neighborCount.ToString());

            UInt16[] neighbourlist = myCSMA.GetNeighbourList();

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
            mySeqNo++;
            //Debug.Print("received message " + noOfPackets.ToString());
            rcvMsg = myCSMA.GetNextPacket();
            rcvPayload = rcvMsg.GetMessage();
            HandleMessage(rcvPayload, (UInt16)rcvMsg.Size, rcvMsg.Src, rcvMsg.Unicast, rcvMsg.RSSI, rcvMsg.LQI);
            UpdateDisplay();
        }

        void HandleMessage(byte[] msg, UInt16 size, UInt16 src, bool unicast, byte rssi, byte lqi)
        {
            try
            {
                /*if (unicast)
                {
                    Debug.Print("Got a Unicast message from src: " + src.ToString() + ", size: " + size.ToString() + ", rssi: " + rssi.ToString() + ", lqi: " + lqi.ToString());
                }
                else
                {
                    Debug.Print("Got a broadcast message from src: " + src.ToString() + ", size: " + size.ToString() + ", rssi: " + rssi.ToString() + ", lqi: " + lqi.ToString());
                }*/
                if (size == 2)
                {
                    //Debug.Print("msg[0]: " + msg[0].ToString() + " msg[1]: " + msg[1].ToString());
                    if (msg[0] != 0 && msg[1] == 174)
                    {
                        ServoControl.Move(msg[0]);
                    }
                }
            }
            catch (Exception e)
            {
                Debug.Print(e.ToString());
            }
        }

        public static void Main()
        {
            Program p = new Program();
            p.Initialize();
            while (true)
            {
                p.UpdateDisplay();
                Thread.Sleep(1000);
            }
        }
    }
}
