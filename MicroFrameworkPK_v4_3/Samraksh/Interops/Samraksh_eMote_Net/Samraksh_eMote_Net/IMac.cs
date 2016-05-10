using System;
using Samraksh.eMote.Net.Radio;

namespace Samraksh.eMote.Net
{
	namespace MAC
	{
		/// <summary>
		/// MAC address type
		/// </summary>
		/// <remarks>
		/// Only used with CSMA
		/// </remarks>
		public enum AddressType
		{
			/// <summary>
			/// Indicates destination is all nodes in range. 
			/// <para>Other values indicate a particular node.</para>
			/// </summary>
			Broadcast = 65535,
		}

		/// <summary>
		/// Link quality
		/// </summary>
		public class Link
		{
			/// <summary>Average Received Signal Strength Indication, RSSI </summary>
			public byte AverageRSSI;
			/// <summary>Link quality</summary>
			public byte LinkQuality;
			/// <summary>Average delay</summary>
			public byte AverageDelay;
		}

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
		}

		/// <summary>
		/// Neighbor details
		/// </summary>
		public class Neighbor
		{
			/// <summary>MAC address of neighbor</summary>
			public ushort MACAddress;
			/// <summary>Forward link of neighbor</summary>
			public Link ForwardLink;
			/// <summary>Reverse link of neighbor</summary>
			public Link ReverseLink;
			/// <summary>Status of neighbor</summary>
			public NeighborStatus Status;
			/// <summary>Packets received from neighbor</summary>
			public ushort PacketsReceived;
			/// <summary>Last time heard from neighbor</summary>
			public ulong LastHeardTime;
			/// <summary>Receive duty cycle of neighbor</summary>
			public byte ReceiveDutyCycle; //percentage
			/// <summary>Frame length of neighbor</summary>
			public ushort FrameLength;
		}


		#region commented code	Removed by Bill. NeighborTable is not used in Samraksh.NET and it's hard to see how it might be useful for a user.
		///// <summary>
		///// List of neighbors and their details
		///// </summary>
		//public class NeighborTable
		//{
		//	/// <summary>Number of neighbor's valid neighbors</summary>
		//	public byte NumberValidNeighbor;
		//	/// <summary>Neighbor</summary>
		//	public Neighbor[] Neighbor;
		//}
		#endregion

		#region commented code
		///*/// <summary>MAC configuration</summary>
		//public class MACConfiguration {
		//	/// <summary>Enable or disable MAC CCA (clear channel assessment)</summary>
		//	public bool CCA;
		//	/// <summary>Number of times to try sending before MAC gives up</summary>
		//	public byte NumberOfRetries;
		//	/// <summary>Duration of CCA</summary>
		//	public byte CCASenseTime;
		//	/// <summary>Size of send buffer</summary>
		//	public byte BufferSize;
		//	//public PayloadType PayloadType;
		//	/// <summary>Radio type used by current MAC</summary>
		//	//public RadioType RadioType;
		//	/// <summary>Delay before a neighbor is deemed dead</summary>
		//	public UInt32 NeighborLivenessDelay;
		//	/// <summary>Configuration of the radio power and channel</summary>
		//	public RadioConfiguration MACRadioConfig;

		//	/// <summary>
		//	/// MAC configuration constructor
		//	/// </summary>
		//	public MACConfiguration()
		//	{
		//		this.CCA = true;
		//		this.BufferSize = 8;
		//		this.NumberOfRetries = 0;
		//		//this.RadioType = RadioType.RF231RADIO;
		//		this.CCASenseTime = 140;
		//		this.NeighborLivenessDelay = 100;
		//		//this.PayloadType = PayloadType.MFM_Data;
		//		this.MACRadioConfig = new RadioConfiguration();
		//	}

		//	/// <summary>
		//	/// MAC configuration constructor
		//	/// </summary>
		//	/// <param name="config">Configuration to apply</param>
		//	public MACConfiguration(MACConfiguration config)
		//	{
		//		this.CCA = config.CCA;
		//		this.BufferSize = config.BufferSize;
		//		this.NumberOfRetries = config.NumberOfRetries;
		//		//this.RadioType = config.RadioType;
		//		this.CCASenseTime = config.CCASenseTime;
		//		this.NeighborLivenessDelay = config.NeighborLivenessDelay;
		//		//this.PayloadType = config.PayloadType;
		//		this.MACRadioConfig = new RadioConfiguration(config.MACRadioConfig);
		//	}
		//};*/
		#endregion

		/// <summary>
		/// MAC interface
		/// </summary>
		public interface IMAC
		{
			// several below added by Bill to force MACPipe to conform to signature of MACBase (so that there can be a one-line change to use MACPipe instead of MACBase)

			/// <summary>The type of MAC (OMAC, CSMA)</summary>
			MACType MACType { get; }

			/// <summary>The radio object the MAC is using</summary>
			Radio_802_15_4_Base MACRadioObj { get; }

			/// <summary>Raised when a packet has been received</summary>
			event MACBase.IMACEventHandler OnReceive;

			/// <summary>Raised when neighborhood changes</summary>
			event MACBase.IMACEventHandler OnNeighborChange;

			/// <summary>True iff MAC CCA (clear channel assessment) is enabled</summary>
			bool CCA { get; set; }

			/// <summary>Number of times to try sending before MAC gives up</summary>
			byte NumberOfRetries { get; set; }

			/// <summary>Amount of time (in milliseconds) to assess whether channel is clear (CCA)</summary>
			byte CCASenseTime { get; set; }

			/// <summary>Size of send buffer</summary>
			byte BufferSize { get; set; }

			/// <summary>Delay (in milliseconds) before a Neighbor is deemed dead</summary>
			uint NeighborLivenessDelay { get; set; }

			/// <summary>
			/// Get the next packet from the MAC buffer
			/// </summary>
			/// <returns>Next packet if any, else null</returns>
			Packet NextPacket();

			/// <summary>
			///		Get the list of neighbors from the MAC
			/// </summary>
			/// <param name="neighborListArray">
			///		Array is filled with the addresses of active neighbors, padded with zeroes at the end.
			/// </param>
			/// <returns>
			///		Result status
			/// </returns>
			DeviceStatus NeighborList(ushort[] neighborListArray);

			/// <summary>Send packet</summary>
			/// <param name="address">Address of recipient</param>
			/// <param name="payload">Payload (in byte array) to send</param>
			/// <param name="offset">Offset into array</param>
			/// <param name="size">Size of payload</param>
			/// <returns>Result status</returns>
			NetOpStatus Send(ushort address, byte[] payload, ushort offset, ushort size);

			/// <summary>Send packet with time value</summary>
			/// <param name="address">Address of recipient</param>
			/// <param name="payload">Payload (in byte array) to send</param>
			/// <param name="offset">Offset into array</param>
			/// <param name="size">Size of message</param>
			/// <param name="eventTime">Time value to add to packet</param>
			/// <returns>Result status</returns>
			NetOpStatus Send(ushort address, byte[] payload, ushort offset, ushort size, DateTime eventTime);

			/// <summary>Send packet with payload type</summary>
			/// <param name="address">Address of recipient</param>
			/// <param name="payloadType">Payload type of packet</param>
			/// <param name="payload">Payload (in byte array) to send</param>
			/// <param name="offset">Offset into array</param>
			/// <param name="size">Size of payload</param>
			/// <returns>Result status</returns>
			NetOpStatus Send(ushort address, PayloadType payloadType, byte[] payload, ushort offset, ushort size);

			/// <summary>Send packet with payload type and time value</summary>
			/// <param name="address">Address of recipient</param>
			/// <param name="payloadType">Payload type of packet</param>
			/// <param name="payload">Payload (in byte array) to send</param>
			/// <param name="offset">Offset into array</param>
			/// <param name="size">Size of payload</param>
			/// <param name="eventTime">Time value to add to packet</param>
			/// <returns>Result status</returns>
			NetOpStatus Send(ushort address, PayloadType payloadType, byte[] payload, ushort offset, ushort size, DateTime eventTime);

			//Neighbor methods

			/// <summary>
			/// Get neighbor status
			/// </summary>
			/// <param name="macAddress">MAC address of neighbor</param>
			/// <returns>Neighbor status</returns>
			Neighbor NeighborStatus(ushort macAddress);

			/// <summary>
			/// Get pending packet count of MAC instance
			/// </summary>
			/// <returns></returns>
			byte PendingSendPacketCount();

			/// <summary>
			/// Get pending packet count of MAC instance
			/// </summary>
			/// <returns></returns>
			byte PendingReceivePacketCount();

			/// <summary>
			/// Remove packet from pending
			/// </summary>
			/// <param name="packet">Packet to remove</param>
			/// <returns>Status of result</returns>
			DeviceStatus RemovePacket(byte[] packet);
		}
	}
}