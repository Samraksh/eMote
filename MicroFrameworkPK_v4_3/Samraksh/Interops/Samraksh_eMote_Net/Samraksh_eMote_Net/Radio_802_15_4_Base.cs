using System.Runtime.CompilerServices;

// ReSharper disable once CheckNamespace
namespace Samraksh.eMote.Net.Radio
{
	/// <summary>
	/// 802.15.4 radio configuration
	/// </summary>
	// ReSharper disable once InconsistentNaming
	public class Radio_802_15_4_Base : IRadio
	{
		// Removed RadioPacketSize & RadioConfigSize - not used - BIll

		//// Size of the radio packet
		//const byte RadioPacketSize = 128;

		//// Size of the radio configuration byte array for marshalling purposes
		//// Note we are marshalling because NETMF does not support passing custom types to native code
		//const byte RadioConfigSize = 3;

		// Removed dataBuffer & marshalBuffer; not used - Bill
		//byte[] dataBuffer = new byte[RadioPacketSize];

		//// Create a buffer that you can use when you want to marshal
		//byte[] marshalBuffer = new byte[RadioConfigSize];

		/// <summary>
		/// Current user of the radio (C# or MAC objects)
		/// </summary>
		/// <value>Current user</value>
		public static RadioUser CurrUser = RadioUser.Idle;

		/// <summary>Radop address</summary>
		public ushort RadioAddress
		{
			get
			{
				return GetRadioAddress((byte)_radioName);
			}
			set
			{
				_radioAddress = value;
				SetRadioAddress((byte)_radioName, _radioAddress);
			}
		}
		private ushort _radioAddress;

		/// <summary>Transmission power of the radio</summary>
		public int TxPower
		{
			get
			{
				return _txPower;
			}
			set
			{
				_txPower = value;
				SetTxPower((byte)_radioName, _txPower);
			}
		}
		private int _txPower;

		/// <summary>channel the radio will transmit on</summary>
		public int Channel
		{
			get
			{
				return _channel;
			}
			set
			{
				_channel = value;
				SetChannel((byte)_radioName, _channel);
			}
		}
		private int _channel;

		/// <summary>Type of radio</summary>
		public RadioName RadioName
		{
			get
			{
				return _radioName;
			}
			set
			{
				_radioName = value;
				SetRadioName((byte)_radioName);
			}
		}
		private RadioName _radioName;

		#region Removed constructor; does nothing, is not needed - Bill
		///// <summary>
		///// Constructor for 802.15.4 radio
		///// </summary>
		///// <exception caption="RadioNotConfigured Exception" cref="RadioNotConfiguredException"></exception>
		//public Radio_802_15_4_Base()
		//{
		//}

		// Removed constructor; does nothing, is not needed - Bill
		///// <summary>Constructor for 802.15.4 radio, specifying driver name and data</summary>
		///// <param name="drvname">Driver name</param>
		///// <param name="drvData">Driver data</param>
		///// <exception caption="RadioNotConfigured Exception" cref="RadioNotConfiguredException"></exception>
		//public Radio_802_15_4_Base(string drvname, ulong drvData)
		//{
		//}
		#endregion

 		/// <summary>Set radio name</summary>
		/// <param name="radioName">Radio type</param>
		/// <returns></returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern DeviceStatus SetRadioName(byte radioName);

		/// <summary>Set the transmit power of the radio</summary>
		/// <param name="radioName">Radio name</param>
		/// <param name="txPower">Transmission power to use</param>
		/// <returns>Status of operation</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern DeviceStatus SetTxPower(byte radioName, int txPower);

		/// <summary>Set the radio _channel</summary>
		/// <param name="radioName">Radio name</param>
		/// <param name="channel">channel to use</param>
		/// <returns>Status of operation</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern DeviceStatus SetChannel(byte radioName, int channel);

		/// <summary>Get the address of the device</summary>
		/// <remarks>This is the address by which the device is known to the rest of the world.</remarks>
		/// <param name="radioName">Radio type</param>
		/// <returns>Address of the device</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern ushort GetRadioAddress(byte radioName);

		/// <summary>Set the address of the device</summary>
		/// <param name="radioName">Radio type</param>
		/// <param name="address">Address of the device</param>
		/// <remarks>This is the address by which the device is known to the rest of the world. 
		///     A return value of false can occur if another layer locks the address and prevents changes.
		/// </remarks>
		/// <returns>Success / failure</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern bool SetRadioAddress(byte radioName, ushort address);

		/// <summary>Uninitialize native MAC, radio and interop drivers</summary>
        /// <param name="radioName">Radio type</param>
        /// <returns>Status of operation.</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern DeviceStatus UnInitialize(byte radioName);

        /// <summary>Turn radio on</summary>
		/// <param name="radioName">Radio name</param>
		/// <returns>Status of operation</returns>
		/// <seealso cref="M:Samraksh.eMote.Net.Radio.Radio_802_15_4_Base.Sleep(System.Byte)">Sleep Method</seealso>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern DeviceStatus TurnOnRx(byte radioName);

		/// <summary>Turn radio off</summary>
		/// <param name="radioName">Radio name</param>
		/// <returns>Status of operation</returns>
		/// <seealso cref="M:Samraksh.eMote.Net.Radio.Radio_802_15_4_Base.Sleep(System.Byte)">Sleep Method</seealso>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern DeviceStatus TurnOffRx(byte radioName);

		/// <summary>Put the radio to sleep</summary>
		/// <param name="radioName">Radio name</param>
		/// <param name="level">Sleep level</param>
		/// <returns>Status of operation</returns>
		/// <seealso cref="M:Samraksh.eMote.Net.Radio.Radio_802_15_4_Base.TurnOn">TurnOn Method</seealso>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern DeviceStatus Sleep(byte radioName, byte level);

		/// <summary>Assess _channel activity</summary>
		/// <remarks>Default is 140 microseconds.</remarks>
		/// <param name="radioName">Radio name</param>
		/// <returns>True iff _channel is free</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern bool ClearChannelAssesment(byte radioName);

		/// <summary>Assess _channel activity</summary>
		/// <param name="radioName">Radio name</param>
		/// <param name="numberOfMicroSecond">Number of microseconds to check</param>
		/// <returns>True iff _channel is free</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern bool ClearChannelAssesment(byte radioName, ushort numberOfMicroSecond);

		/// <summary>Load the packet into the transmit buffer of the radio</summary>
		/// <param name="packet">Packet to load</param>
		/// <param name="size">Size of packet</param>
		/// <returns>The result of the method: E_RadioInit, E_RadioSync, E_RadioConfig, E_MacInit, E_MacConfig, E_MacSendError, E_MacBufferFull, S_Success</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern NetOpStatus PreLoad(byte[] packet, ushort size);

		/// <summary>Send the packet in the transmit buffer</summary>
		/// <returns>Result of operation</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern NetOpStatus SendStrobe(byte radioName, ushort size);	//Send preloaded packet

		/// <summary>Load and send a packet</summary>
		/// <param name="radioName">Radio name</param>
		/// <param name="packet">Packet to be sent</param>
		/// <param name="size">Size of packet</param>
		/// <returns>Result of operation</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern NetOpStatus Send(byte radioName, byte[] packet, ushort size);

		/// <summary>Load and send a time-stamped packet, with specified time stamp</summary>
		/// <param name="radioName">Radio name</param>
		/// <param name="packet">Packet to be sent</param>
		/// <param name="size">Size of packet</param>
		/// <param name="eventTime">The time stamp.</param>
		/// <remarks>The offset for the timestamp in the packet is specified by TimeStampOffset  member of the RadioConfiguration structure passed as parameter during radio module initialization.</remarks>
		/// <returns>The result of the method: E_RadioInit, E_RadioSync, E_RadioConfig, E_MacInit, E_MacConfig, E_MacSendError, E_MacBufferFull, S_Success</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern NetOpStatus SendTimeStamped(byte radioName, byte[] packet, ushort size, uint eventTime);
	}
}
