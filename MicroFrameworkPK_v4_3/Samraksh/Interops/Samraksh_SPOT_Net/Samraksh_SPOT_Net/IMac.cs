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
            /// <summary>
            /// Flag that enables/disables CCA 
            /// </summary>
            public bool CCA;
            /// <summary>
            /// Controls number of retries for sending before MAC should give up
            /// </summary>
            public byte NumberOfRetries;

            /// <summary>
            /// Controls the amount of time clear channel assessment should be done
            /// </summary>
            public byte CCASenseTime;

            /// <summary>
            /// Size of the send buffer
            /// </summary>
            public byte BufferSize;

            /// <summary>
            /// Radio ID of the radio the mac is current running on
            /// </summary>
            public byte RadioID;

            /// <summary>
            /// Delay before a neighbour is deemed dead and removed from the neighbour table
            /// </summary>
            public UInt32 NeighbourLivelinesDelay;

            /// <summary>
            /// Configuration of the radio power and channel 
            /// </summary>
            public Radio.RadioConfiguration radioConfig;


            /// <summary>
            /// Default constructor, all values are set to default CCA = true, BufferSize = 8, NumberOfRetries = 0, RadioID = 1, CCASenseTime = 140ms, NeighbourLivelinessDelay = 300s
            /// </summary>
            public MacConfiguration()
            {
                this.CCA = true;
                this.BufferSize = 8;
                this.NumberOfRetries = 0;
                this.RadioID = 1;
                this.CCASenseTime = 140;
                this.NeighbourLivelinesDelay = 300;
                this.radioConfig = new Radio.RadioConfiguration();
            }

            public MacConfiguration(MacConfiguration config)
            {
                this.CCA = config.CCA;
                this.BufferSize = config.BufferSize;
                this.NumberOfRetries = config.NumberOfRetries;
                this.RadioID = config.RadioID;
                this.CCASenseTime = config.CCASenseTime;
                this.NeighbourLivelinesDelay = config.NeighbourLivelinesDelay;
                this.radioConfig = new Radio.RadioConfiguration(config.radioConfig);
            }

            /// <summary>
            /// Constructor allows you to set each of the mac parameters 
            /// </summary>
            /// <param name="CCA">Enable Clear Channel Assessment</param>
            /// <param name="NumberOfRetries">Number of retries</param>
            /// <param name="CCASenseTime">Clear Channel Assessment time</param>
            /// <param name="BufferSize">Size of the send buffer</param>
            /// <param name="RadioID">Radio ID</param>
            /// <param name="NeighbourLivelinessDelay">Delay before a neighbour is deemed dead in the neighbour table</param>
            /// <param name="config">Radio configuration</param>
            public MacConfiguration(bool CCA, byte NumberOfRetries, byte CCASenseTime, byte BufferSize, byte RadioID, UInt32 NeighbourLivelinessDelay, Radio.RadioConfiguration config)
            {
                this.CCA = CCA;
                this.NumberOfRetries = NumberOfRetries;
                this.CCASenseTime = CCASenseTime;
                this.BufferSize = BufferSize;
                this.RadioID = RadioID;

                this.radioConfig = new Radio.RadioConfiguration(config);

            }

            /// <summary>
            /// Constructor allows you to set CCA, NumberOfRetries, CCASenseTime, BufferSize and RadioID
            /// </summary>
            /// <param name="CCA">Enable Clear Channel Assessment</param>
            /// <param name="NumberOfRetries">Number of retries for sending</param>
            /// <param name="CCASenseTime">Carrier sense time</param>
            /// <param name="BufferSize">BufferSize of the radio</param>
            /// <param name="RadioID">Radio ID</param>
            public MacConfiguration(bool CCA, byte NumberOfRetries, byte CCASenseTime, byte BufferSize, byte RadioID)
            {
                this.CCA = CCA;
                this.NumberOfRetries = NumberOfRetries;
                this.CCASenseTime = CCASenseTime;
                this.BufferSize = BufferSize;
                this.RadioID = RadioID;
                this.radioConfig = new Radio.RadioConfiguration();
            }

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