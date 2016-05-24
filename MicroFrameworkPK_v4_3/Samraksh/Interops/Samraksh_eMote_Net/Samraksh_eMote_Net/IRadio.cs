// ReSharper disable once CheckNamespace
namespace Samraksh.eMote.Net.Radio
{
	/// <summary>
	/// Kind of user
	/// </summary>
	public enum RadioUser
	{
		/// <summary>
		/// Radio Object is used by a C# application
		/// </summary>
		CSharp,
		/// <summary>
		/// Radio is being used by the CSMA MAC object
		/// </summary>
		CSMA,
		/// <summary>
		/// Radio is being used by the OMAC MAC object
		/// </summary>
		OMAC,
		/// <summary>
		/// Radio is idle
		/// </summary>
		Idle,
	}

	/// <summary>Name of radio</summary>
	public enum RadioName
	{
		/// <summary>On-board radio</summary>
		RF231,
		/// <summary>Long-range radio</summary>
		RF231LR,
		/// <summary>Long range (433 MHz) radio</summary>
		SI4468
	}

	#region Commented code
	/*/// <summary></summary>
	//public enum TxPowerValueSI4468
	//{
	//	/// <summary></summary>
	//	Power_SI4468_3dBm = 16,
	//	/// <summary></summary>
	//	Power_SI4468_2Point8dBm = 17
	//};

	///// <summary>
	///// Power levels supported by the RF231 radio
	///// </summary>
	//public enum TxPowerValueRF231
	//{
	//	/// <summary>+3.0 dB</summary>
	//	Power_3dBm = 0,
	//	/// <summary>+2.8 dB</summary>
	//	Power_2Point8dBm = 1,
	//	/// <summary>+2.3 dB</summary>
	//	Power_2Point3dBm = 2,
	//	/// <summary>+1.8 dB</summary>
	//	Power_1Point8dBm = 3,
	//	/// <summary>+1.3 dB</summary>
	//	Power_1Point3dBm = 4,
	//	/// <summary>+0.7 dB</summary>
	//	Power_0Point7dBm = 5,
	//	/// <summary>+0.0 dB</summary>
	//	Power_0Point0dBm = 6,
	//	/// <summary>-1.0 dB</summary>
	//	Power_Minus1dBm = 7,
	//	/// <summary>-2.0 dB</summary>
	//	Power_Minus2dBm = 8,
	//	/// <summary>-3.0 dB</summary>
	//	Power_Minus3dBm = 9,
	//	/// <summary>-4.0 dB</summary>
	//	Power_Minus4dBm = 10,
	//	/// <summary>-5.0 dB</summary>
	//	Power_Minus5dBm = 11,
	//	/// <summary>-7.0 dB</summary>
	//	Power_Minus7dBm = 12,
	//	/// <summary>-9.0 dB</summary>
	//	Power_Minus9dBm = 13,
	//	/// <summary>-12.0 dB</summary>
	//	Power_Minus12dBm = 14,
	//	/// <summary>-17.0 dB</summary>
	//	Power_Minus17dBm = 15,
	//};

	///// <summary></summary>
	//public class TxPowerValue
	//{
	//	/// <summary></summary>
	//	public UInt16 txPowerValue;
	//}

	///// <summary></summary>
	//public enum ChannelRF231
	//{
	//	/// <summary></summary>
	//	Channel_RF231_11 = 0,
	//	/// <summary></summary>
	//	Channel_RF231_12 = 1,
	//	/// <summary></summary>
	//	Channel_RF231_26 = 15
	//};

	///// <summary></summary>
	//public enum ChannelSI4468 
	//{
	//	/// <summary></summary>
	//	Channel_SI4468_01 = 16,
	//	/// <summary></summary>
	//	Channel_SI4468_02 = 17
	//};

	///// <summary>Channels the RF231 radio can use</summary>
	//public class Channel
	//{
	//	/// <summary></summary>
	//	public UInt16 channelType;

	//	/// <summary></summary>
	//	public Channel()
	//	{
	//		//channelType = ChannelRF231.Channel_RF231_26;
	//	}
	//};*/
	#endregion 

	// Separated power values and channels into enums according to radio type - Bill

	/// <summary>
	/// Power levels supported by the RF231 radio (onboard and long range)
	/// </summary>
	public enum RF231TxPower
	{
		/// <summary>+3.0 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_3dBm = 0,

		/// <summary>+2.8 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_2Point8dBm = 1,

		/// <summary>+2.3 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_2Point3dBm = 2,

		/// <summary>+1.8 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_1Point8dBm = 3,

		/// <summary>+1.3 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_1Point3dBm = 4,

		/// <summary>+0.7 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_0Point7dBm = 5,

		/// <summary>+0.0 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_0Point0dBm = 6,

		/// <summary>-1.0 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_Minus1dBm = 7,

		/// <summary>-2.0 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_Minus2dBm = 8,

		/// <summary>-3.0 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_Minus3dBm = 9,

		/// <summary>-4.0 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_Minus4dBm = 10,

		/// <summary>-5.0 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_Minus5dBm = 11,

		/// <summary>-7.0 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_Minus7dBm = 12,

		/// <summary>-9.0 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_Minus9dBm = 13,

		/// <summary>-12.0 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_Minus12dBm = 14,

		/// <summary>-17.0 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_Minus17dBm = 15,
	}

	/// <summary>
	/// Power levels supported by the SI4468 radio
	/// </summary>
	public enum SI4468TxPower
	{
		/// <summary>20 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_20dBm = 16,
		/// <summary>19.7 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_19Point7dBm = 17,
		/// <summary>19.4 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_19Point4dBm = 18,
		/// <summary>-20 dB</summary>
		// ReSharper disable once InconsistentNaming
		Power_Minus20dBm = 143
	}

	/// <summary>
	/// Channels supported by RF231 radio
	/// </summary>
	public enum RF231Channel
	{
		/// <summary>Channel 11 Frequency 2405 MHz</summary>
		// ReSharper disable once InconsistentNaming
		Channel_11 = 0,

		/// <summary>Channel 12 Frequency 2410 MHz</summary>
		// ReSharper disable once InconsistentNaming
		Channel_12 = 1,

		/// <summary>Channel 13 Frequency 2415 MHz</summary>
		// ReSharper disable once InconsistentNaming
		Channel_13 = 2,

		/// <summary>Channel 14 Frequency 2420 MHz</summary>
		// ReSharper disable once InconsistentNaming
		Channel_14 = 3,

		/// <summary>Channel 15 Frequency 2425 MHz</summary>
		// ReSharper disable once InconsistentNaming
		Channel_15 = 4,

		/// <summary>Channel 16 Frequency 2430 MHz</summary>
		// ReSharper disable once InconsistentNaming
		Channel_16 = 5,

		/// <summary>Channel 17 Frequency 2435 MHz</summary>
		// ReSharper disable once InconsistentNaming
		Channel_17 = 6,

		/// <summary>Channel 18 Frequency 2440 MHz</summary>
		// ReSharper disable once InconsistentNaming
		Channel_18 = 7,

		/// <summary>Channel 19 Frequency 2445 MHz</summary>
		// ReSharper disable once InconsistentNaming
		Channel_19 = 8,

		/// <summary>Channel 20 Frequency 2450 MHz</summary>
		// ReSharper disable once InconsistentNaming
		Channel_20 = 9,

		/// <summary>Channel 21 Frequency 2455 MHz</summary>
		// ReSharper disable once InconsistentNaming
		Channel_21 = 10,

		/// <summary>Channel 22 Frequency 2460 MHz</summary>
		// ReSharper disable once InconsistentNaming
		Channel_22 = 11,

		/// <summary>Channel 23 Frequency 2465 MHz</summary>
		// ReSharper disable once InconsistentNaming
		Channel_23 = 12,

		/// <summary>Channel 24 Frequency 2470 MHz</summary>
		// ReSharper disable once InconsistentNaming
		Channel_24 = 13,

		/// <summary>Channel 25 Frequency 2475 MHz</summary>
		// ReSharper disable once InconsistentNaming
		Channel_25 = 14,

		/// <summary>Channel 26 Frequency 2480 MHz</summary>
		// ReSharper disable once InconsistentNaming
		Channel_26 = 15,
	}

	/// <summary>
	/// Channels supported by SI4468 radio
	/// </summary>
	public enum SI4468Channel
	{
		/// <summary>Channel 1</summary>
		// ReSharper disable once InconsistentNaming
		Channel_01 = 16,
		/// <summary>Channel 2</summary>
		// ReSharper disable once InconsistentNaming
		Channel_02 = 17
	}

	/// <summary>
	/// Radio configuration interface
	/// </summary>
	public interface IRadioConfiguration
	{
		/// <summary>Transmission power</summary>
		int TxPower { get; set; }

		/// <summary>Channel</summary>
		int Channel { get; set; }

		/// <summary>Radio name</summary>
		RadioName RadioName { get; set; }
	}

	// Changed power value and channel to int (since enums cannot be inherited) - Bill

	/// <summary>
	/// RF231 Radio configuration
	/// </summary>
	public class RF231RadioConfiguration : IRadioConfiguration
	{
		/// <summary>Transmission power of the radio</summary>
		public int TxPower { get; set; }

		/// <summary>Channel the radio will transmit on</summary>
		public int Channel { get; set; }

		/// <summary>Type of radio</summary>
		public RadioName RadioName { get; set; }

		/// <summary>Radio configuration constructor</summary>
		/// <param name="power">Transmission power</param>
		/// <param name="channel">Channel</param>
		public RF231RadioConfiguration(RF231TxPower power = RF231TxPower.Power_3dBm,
			RF231Channel channel = RF231Channel.Channel_26)
		{
			RadioName = RadioName.RF231;
			TxPower = (int)power;
			Channel = (int)channel;
		}
	}

	/// <summary>
	/// RF231 Radio configuration
	/// </summary>
	public class RF231LRRadioConfiguration : IRadioConfiguration
	{
		/// <summary>Transmission power of the radio</summary>
		public int TxPower { get; set; }

		/// <summary>Channel the radio will transmit on</summary>
		public int Channel { get; set; }

		/// <summary>Type of radio</summary>
		public RadioName RadioName { get; set; }

		/// <summary>Radio configuration constructor</summary>
		/// <param name="power">Transmission power</param>
		/// <param name="channel">Channel</param>
		public RF231LRRadioConfiguration(RF231TxPower power = RF231TxPower.Power_3dBm, RF231Channel channel= RF231Channel.Channel_26)
		{
			RadioName = RadioName.RF231LR;
			TxPower = (int)power;
			Channel = (int)channel;
		}
	}

	/// <summary>SI4468 Radio configuration</summary>
	public class SI4468RadioConfiguration : IRadioConfiguration
	{
		/// <summary>Transmission power of the radio</summary>
		public int TxPower { get; set; }

		/// <summary>Channel the radio will transmit on</summary>
		public int Channel { get; set; }

		/// <summary>Type of radio</summary>
		public RadioName RadioName { get; set; }

		/// <summary>Radio configuration constructor</summary>
		/// <param name="power">Transmission power</param>
		/// <param name="channel">Channel</param>
		public SI4468RadioConfiguration(SI4468TxPower power = SI4468TxPower.Power_20dBm, SI4468Channel channel = SI4468Channel.Channel_02)
		{
			RadioName = RadioName.SI4468;
			TxPower = (int)power;
			Channel = (int)channel;
		}
	}

	/// <summary>
	/// Radio interface
	/// </summary>
	public interface IRadio
	{
		//DeviceStatus Initialize(RadioConfiguration config, ReceiveCallBack callback); //Initializes Return the ID of the Radio layer that was initialized
		//DeviceStatus Configure(RadioConfiguration config, ReceiveCallBack callback);  //Change configuration after initialization

		/// <summary>
		/// Unitialize radio
		/// </summary>
		/// <returns>Success of operation</returns>
		DeviceStatus UnInitialize();

		/// <summary>
		/// Turn radio on
		/// </summary>
		/// <param name="radioType">Radio ID</param>
		/// <returns>Success of operation</returns>
		DeviceStatus TurnOnRx(byte radioType);

		/// <summary>
		/// Put radio to sleep
		/// </summary>
		/// <param name="radioType">Radio ID</param>
		/// <param name="level">Sleep level</param>
		/// <returns>Success of operation</returns>
		DeviceStatus Sleep(byte radioType, byte level);

		/// <summary>
		/// Preload radio
		/// </summary>
		/// <param name="packet">Packet to preload</param>
		/// <param name="size">Size of packet</param>
		/// <returns>Success of operation</returns>
		NetOpStatus PreLoad(byte[] packet, ushort size);

		/// <summary>
		/// Send preloaded packet
		/// </summary>
		/// <returns>Success of operation</returns>
		NetOpStatus SendStrobe(byte radioType, ushort size);

		/// <summary>
		/// Send packet on radio
		/// </summary>
		/// <param name="radioType">Radio ID</param>
		/// <param name="packet">Packet to send</param>
		/// <param name="size">Size of packet</param>
		/// <returns>Success of operation</returns>
		NetOpStatus Send(byte radioType, byte[] packet, ushort size);

		/// <summary>
		/// Sent time-stamped packet
		/// </summary>
		/// <param name="radioType">Radio ID</param>
		/// <param name="packet">Packet buffer to send</param>
		/// <param name="size">Size of packet</param>
		/// <param name="eventTime">Time stamp of packet</param>
		/// <returns>Send status</returns>
		NetOpStatus SendTimeStamped(byte radioType, byte[] packet, ushort size, uint eventTime);

		/// <summary>
		/// Check if channel is clear
		/// </summary>
		/// <param name="radioType">Radio ID</param>
		/// <returns>True iff clear</returns>
		bool ClearChannelAssesment(byte radioType);

		/// <summary>
		/// Check if channel has been clear for the specified interval of time
		/// </summary>
		/// <param name="radioType">Radio ID</param>
		/// <param name="numberOfMicroSecond">Interval (microseconds)</param>
		/// <returns>True iff clear</returns>
		bool ClearChannelAssesment(byte radioType, ushort numberOfMicroSecond);
	}
}
