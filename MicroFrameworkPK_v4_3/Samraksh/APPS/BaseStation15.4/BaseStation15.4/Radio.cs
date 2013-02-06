using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;

namespace BaseStation15._4
{
    class Radio
    {
        Samraksh.SPOT.Net.Mac.CSMA csmaObject = new Samraksh.SPOT.Net.Mac.CSMA();
        Samraksh.SPOT.Net.Mac.MacConfiguration macConfig = new Samraksh.SPOT.Net.Mac.MacConfiguration();
        ReceiveCallBack recieve;
        Serial serialPortInstance;
        CircularBuffer.CircularBuffer buffer;

        public Radio()
        {
            macConfig.CCA = true;
            macConfig.BufferSize = 8;
            macConfig.NumberOfRetries = 0;
            //macConfig.RadioID = (byte) myRadioID;
            macConfig.RadioID = (byte)1;
            // Nived.Sivadas
            // The carrier sense is not trully handled in the radio hardware and can not be modified for RF231
            macConfig.CCASenseTime = 140; //Carries sensing time in micro seconds

            recieve = HandleMessage;

            try
            {
                csmaObject.Initialize(macConfig, recieve);
            }
            catch (Exception e)
            {
                Debug.Print("Failed CSMA Init");
            }

        }

        void HandleMessage(byte[] msg, ushort size)
        {
            serialPortInstance = Serial.GetInstance(buffer);
            serialPortInstance.Send(msg);
        }

    }

}
