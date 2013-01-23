using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;

namespace Samraksh.SPOT.Net
{
    namespace Mac
    {
        public enum Addresses
        {
            BROADCAST = 65535,
        };


        public struct MacConfiguration
        {
            public bool CCA;
            public byte NumberOfRetries;
            public byte CCASenseTime;
            public byte BufferSize;
            public byte RadioID;
        };

        public struct Link
        {
            public byte Quality;
            public byte LossRate;
            public byte AveDelay;
        };

        public enum NeighborStatus
        {
            Alive,
            Dead,
            Suspect
        };

        public struct Neighbor
        {
            public UInt16 MacAddress;
            public Link ForwardLink;
            public Link ReverseLink;
            public NeighborStatus Status;
            public UInt64 LastHeardTime;
            public byte ReceiveDutyCycle; //percentage
            public UInt16 FrameLength;
        };

        public struct NeighborTable
        {
            public byte NumberValidNeighbor;
            public Neighbor[] Neighbor;
        };

        public interface IMac
        {
            //Basic functions
            DeviceStatus Initialize(MacConfiguration config, ReceiveCallBack callback); //Initializes Return the ID of the Radio layer that was initialized
            DeviceStatus Configure(MacConfiguration config);
            DeviceStatus UnInitialize();
            NetOpStatus Send(UInt16 Address, byte[] message, UInt16 offset, UInt16 size);
            UInt16 GetAddress();
            bool SetAddress(UInt16 Address);
            byte GetID();

            //Neighbor functions
            NeighborTable GetNeighborTable();
            Neighbor GetNeighbors(UInt16 macAddress);

            //Buffer functions
            byte GetBufferSize();
            byte GetPendingPacketCount();
            DeviceStatus RemovePacket(byte[] msg);

            //MAC Aggregate APIs
            //bool MacLayer_Initialize();
            //bool MacLayer_UnInitialize();
            //byte MacLayer_NumberMacsSupported();
        }

    }
}