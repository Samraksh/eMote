using System;
using Microsoft.SPOT;
using Samraksh.eMote.Net;

namespace Samraksh.eMote.Net
{
    namespace Mac
    {
        /// <summary>
        /// MAC addresses
        /// </summary>
        public enum Addresses
        {
            /// <summary>
            /// Indicates destination is all nodes in range. 
            /// <para>Other values indicate a particular node.</para>
            /// </summary>
            BROADCAST = 65535,
        };


		/// <summary>
		/// MAC configuration
		/// </summary>
		public class MacConfiguration {
			/// <summary>
			/// Enable or disable MAC CCA (clear channel assessment)
			/// </summary>
			public bool CCA;
			/// <summary>
			/// Number of times to try sending before MAC gives up
			/// </summary>
			public byte NumberOfRetries;

			/// <summary>
			/// Duration of CCA
			/// </summary>
			public byte CCASenseTime;

			/// <summary>
			/// Size of send buffer
			/// </summary>
			public byte BufferSize;

			/// <summary>
			/// Radio ID current MAC
			/// </summary>
			public byte RadioID;

			/// <summary>
			/// Delay before a neighbor is deemed dead
			/// </summary>
			public UInt32 NeighborLivenessDelay;

			/// <summary>
			/// Delay before a neighbor is deemed dead
			/// </summary>
			[Obsolete("Use NeighborLivenessDelay instead")]
			public UInt32 NeighbourLivelinesDelay {
				get { return NeighborLivenessDelay; }
				set { NeighborLivenessDelay = value; }
			}

			/// <summary>
            /// Configuration of the radio power and channel 
            /// </summary>
            public Radio.RadioConfiguration radioConfig;


            /// <summary>
            /// MAC configuration constructor
            /// </summary>
            public MacConfiguration()
            {
                this.CCA = true;
                this.BufferSize = 8;
                this.NumberOfRetries = 0;
                this.RadioID = 1;
                this.CCASenseTime = 140;
                this.NeighborLivenessDelay = 300;
                this.radioConfig = new Radio.RadioConfiguration();
            }

            /// <summary>
            /// MAC configuration constructor
            /// </summary>
            /// <param name="config">Configuration to apply</param>
            public MacConfiguration(MacConfiguration config)
            {
                this.CCA = config.CCA;
                this.BufferSize = config.BufferSize;
                this.NumberOfRetries = config.NumberOfRetries;
                this.RadioID = config.RadioID;
                this.CCASenseTime = config.CCASenseTime;
                this.NeighborLivenessDelay = config.NeighborLivenessDelay;
                this.radioConfig = new Radio.RadioConfiguration(config.radioConfig);
            }

            /// <summary>
            /// MAC configuration constructor
            /// </summary>
            /// <param name="CCA">Clear Channel Assessment</param>
            /// <param name="numberOfRetries">Number of retries</param>
            /// <param name="ccaSenseTime">Clear Channel Assessment time</param>
            /// <param name="bufferSize">Size of send buffer</param>
            /// <param name="radioID">Radio ID</param>
            /// <param name="neighborLivelinessDelay">Delay before a neighbor is deemed dead</param>
            /// <param name="config">Radio configuration</param>
            public MacConfiguration(bool CCA, byte numberOfRetries, byte ccaSenseTime, byte bufferSize, byte radioID, UInt32 neighborLivelinessDelay, Radio.RadioConfiguration config)
            {
                this.CCA = CCA;
                this.NumberOfRetries = numberOfRetries;
                this.CCASenseTime = ccaSenseTime;
                this.BufferSize = bufferSize;
                this.RadioID = radioID;

                this.radioConfig = new Radio.RadioConfiguration(config);

            }

            /// <summary>
            /// MAC configuration constructor
            /// </summary>
            /// <param name="CCA">Enable Clear Channel Assessment</param>
            /// <param name="numberOfRetries">Number of retries for sending</param>
            /// <param name="ccaSenseTime">Carrier sense time</param>
            /// <param name="bufferSize">BufferSize of the radio</param>
            /// <param name="radioID">Radio ID</param>
            public MacConfiguration(bool CCA, byte numberOfRetries, byte ccaSenseTime, byte bufferSize, byte radioID)
            {
                this.CCA = CCA;
                this.NumberOfRetries = numberOfRetries;
                this.CCASenseTime = ccaSenseTime;
                this.BufferSize = bufferSize;
                this.RadioID = radioID;
                this.radioConfig = new Radio.RadioConfiguration();
            }

        };

        /// <summary>
        /// Link quality
        /// </summary>
        public struct Link
        {
            /// <summary>Average Received Signal Strength Indication, RSSI </summary>
            public byte AveRSSI;
            /// <summary>Link quality</summary>
            public byte LinkQuality;
            /// <summary>Average delay</summary>
            public byte AveDelay;
        };

       /// <summary>
       /// Neighbor status
       /// </summary>
        public enum NeighborStatus
        {
            /// <summary>Neighbor is alive</summary>
            Alive,
            /// <summary>Neighbor is dead</summary>
            Dead,
            /// <summary>Neighbor is suspect</summary>
            Suspect
        };

		/// <summary>
		/// Neighbor status
		/// </summary>
		[Obsolete("Deprecated. Use NeighborStatus instead")]
		public enum NeighbourStatus {
			/// <summary>Neighbor is alive</summary>
			Alive,
			/// <summary>Neighbor is dead</summary>
			Dead,
			/// <summary>Neighbor is suspect</summary>
			Suspect
		};

		/// <summary>
		/// Neighbor details
		/// </summary>
		public class Neighbor {
			/// <summary>MAC address of neighbor</summary>
			public UInt16 MacAddress;
			/// <summary>Forward link of neighbor</summary>
			public Link ForwardLink;
			/// <summary>Reverse link of neighbor</summary>
			public Link ReverseLink;
			/// <summary>Status of neighbor</summary>
			public NeighborStatus Status;
			/// <summary>Packets received from neighbor</summary>
			public UInt16 PacketsReceived;
			/// <summary>Last time heard from neighbor</summary>
			public UInt64 LastHeardTime;
			/// <summary>Receive duty cycle of neighbor</summary>
			public byte ReceiveDutyCycle; //percentage
			/// <summary>Frame length of neighbor</summary>
			public UInt16 FrameLength;

		};

		/// <summary>
		/// Neighbor details
		/// </summary>
		[Obsolete("Deprecated. Use Neighbor instead")]
		public class Neighbour {
			/// <summary>MAC address of neighbor</summary>
			public UInt16 MacAddress;
			/// <summary>Forward link of neighbor</summary>
			public Link ForwardLink;
			/// <summary>Reverse link of neighbor</summary>
			public Link ReverseLink;
			/// <summary>Status of neighbor</summary>
			public NeighbourStatus Status;
			/// <summary>Packets received from neighbor</summary>
			public UInt16 PacketsReceived;
			/// <summary>Last time heard from neighbor</summary>
			public UInt64 LastHeardTime;
			/// <summary>Receive duty cycle of neighbor</summary>
			public byte ReceiveDutyCycle; //percentage
			/// <summary>Frame length of neighbor</summary>
			public UInt16 FrameLength;

		};

		/// <summary>
		/// List of neighbors and their details
		/// </summary>
		public struct NeighborTable {
			/// <summary>Number of neighbor's valid neighbors</summary>
			public byte NumberValidNeighbor;
			/// <summary>Neighbor</summary>
			public Neighbor[] Neighbor;
		};

		/// <summary>
		/// List of neighbors and their details
		/// </summary>
		[Obsolete("Deprecated. Use NeighborTable instead")]
		public struct NeighbourTable {
			/// <summary>Number of neighbor's valid neighbors</summary>
			public byte NumberValidNeighbour;
			/// <summary>Neighbor</summary>
			public Neighbour[] Neighbor;
		};

		/// <summary>
       /// MAC interface
       /// </summary>
        public interface IMac
        {
            //Basic functions
            //DeviceStatus Initialize(MacConfiguration config, ReceiveCallBack callback); //Initializes Return the ID of the Radio layer that was initialized
            //DeviceStatus Configure(MacConfiguration config);

            /// <summary>
            /// Unitialize MAC
            /// </summary>
            /// <returns>Success of operation</returns>
            DeviceStatus UnInitialize();
            
            /// <summary>
            /// Send message
            /// </summary>
            /// <param name="address">Address of recipient (can use Addresses.BROADCAST)</param>
            /// <param name="message">Byte array of to send</param>
            /// <param name="offset">Offset into array</param>
            /// <param name="size">Size of message</param>
            /// <returns></returns>
            NetOpStatus Send(UInt16 address, byte[] message, UInt16 offset, UInt16 size);
            
            /// <summary>
            /// Get address of MAC instance
            /// </summary>
            /// <returns>Address</returns>
            UInt16 GetAddress();

            /// <summary>
            /// Set address of MAC instance
            /// </summary>
            /// <param name="address">Address</param>
            /// <returns>Success of operation</returns>
            bool SetAddress(UInt16 address);

            /// <summary>
            /// Get MAC Id
            /// </summary>
            /// <returns>MAC Id</returns>
            byte GetID();

            //Neighbor methods

			/// <summary>
			/// Get neighbor status
			/// </summary>
			/// <param name="macAddress">MAC address of neighbor</param>
			/// <returns>Neighbor status</returns>
			Neighbor GetNeighborStatus(UInt16 macAddress);

			/// <summary>
			/// Get neighbor status
			/// </summary>
			/// <param name="macAddress">MAC address of neighbor</param>
			/// <returns>Neighbor status</returns>
			[Obsolete("Deprecated. Use GetNeighborStatus instead")]
			Neighbor GetNeighbourStatus(UInt16 macAddress);

			//Buffer methods

            /// <summary>
            /// Get buffer size of MAC instance
            /// </summary>
            /// <returns>Buffer size</returns>
            byte GetBufferSize();

            /// <summary>
            /// Get pending packet count of MAC instance
            /// </summary>
            /// <returns></returns>
            byte GetPendingPacketCount();

            /// <summary>
            /// Remove packet from pending
            /// </summary>
            /// <param name="msg">Message to remove</param>
            /// <returns>Status of result</returns>
            DeviceStatus RemovePacket(byte[] msg);

            //MAC Aggregate APIs
            //bool MacLayer_Initialize();
            //bool MacLayer_UnInitialize();
            //byte MacLayer_NumberMacsSupported();
        }

    }
}