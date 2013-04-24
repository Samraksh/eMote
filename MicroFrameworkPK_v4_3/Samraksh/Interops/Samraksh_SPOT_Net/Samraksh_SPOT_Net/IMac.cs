using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;

namespace Samraksh.SPOT.Net
{
    namespace Mac
    {
        /// <summary>
        /// MAC addresses
        /// </summary>
        public enum Addresses
        {
            BROADCAST = 65535,
        };


        /// <summary>
        /// MAC configuration
        /// </summary>
        public class MacConfiguration
        {
            public bool CCA;
            public byte NumberOfRetries;
            public byte CCASenseTime;
            public byte BufferSize;
            public byte RadioID;
            public Radio.RadioConfiguration radioConfig;
        };

        /// <summary>
        /// Link quality
        /// </summary>
        public struct Link
        {
            public byte AveRSSI;
            public byte LinkQuality;
            public byte AveDelay;
        };

       /// <summary>
       /// Neighbor status
       /// </summary>
        public enum NeighborStatus
        {
            Alive,
            Dead,
            Suspect
        };

        /// <summary>
        /// Neighbor details
        /// </summary>
        public struct Neighbor
        {
            public UInt16 MacAddress;
            public Link ForwardLink;
            public Link ReverseLink;
            public NeighborStatus Status;
            public UInt16 PacketsReceived;
            public UInt64 LastHeardTime;
            public byte ReceiveDutyCycle; //percentage
            public UInt16 FrameLength;
            
        };

        /// <summary>
        /// List of neighbors and their details
        /// </summary>
        public struct NeighborTable
        {
            public byte NumberValidNeighbor;
            public Neighbor[] Neighbor;
        };

       /// <summary>
       /// MAC interface
       /// </summary>
        public interface IMac
        {
            //Basic functions
            //DeviceStatus Initialize(MacConfiguration config, ReceiveCallBack callback); //Initializes Return the ID of the Radio layer that was initialized
            //DeviceStatus Configure(MacConfiguration config);
            DeviceStatus UnInitialize();
            NetOpStatus Send(UInt16 Address, byte[] message, UInt16 offset, UInt16 size);
            UInt16 GetAddress();
            bool SetAddress(UInt16 Address);
            byte GetID();

            //Neighbor functions
            //bool GetNeighborTable(NeighborTable table);
            bool GetNeighborStatus(UInt16 macAddress,ref Neighbor neighbor);

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