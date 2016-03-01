using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

//using Samraksh.eMote.Net.Radio;
using System.Runtime.CompilerServices;

namespace Samraksh.eMote.Net.Radio
{

    /// <summary>Kind of radio</summary>
    public enum RadioName
    {
        /// <summary>On-board radio</summary>
        RF231RADIO,
        /// <summary>Long-range radio</summary>
        RF231RADIOLR,
    }

    /// <summary>
    /// Power levels supported by the RF231 radio
    /// </summary>
    public enum TxPowerValue
    {
        /// <summary>+3.0 dB</summary>
        Power_3dBm,
        /// <summary>+2.8 dB</summary>
        Power_2Point8dBm,
        /// <summary>+2.3 dB</summary>
        Power_2Point3dBm,
        /// <summary>+1.8 dB</summary>
        Power_1Point8dBm,
        /// <summary>+1.3 dB</summary>
        Power_1Point3dBm,
        /// <summary>+0.7 dB</summary>
        Power_0Point7dBm,
        /// <summary>+0.0 dB</summary>
        Power_0Point0dBm,
        /// <summary>-1.0 dB</summary>
        Power_Minus1dBm,
        /// <summary>-2.0 dB</summary>
        Power_Minus2dBm,
        /// <summary>-3.0 dB</summary>
        Power_Minus3dBm,
        /// <summary>-4.0 dB</summary>
        Power_Minus4dBm,
        /// <summary>-5.0 dB</summary>
        Power_Minus5dBm,
        /// <summary>-7.0 dB</summary>
        Power_Minus7dBm,
        /// <summary>-9.0 dB</summary>
        Power_Minus9dBm,
        /// <summary>-12.0 dB</summary>
        Power_Minus12dBm,
        /// <summary>-17.0 dB</summary>
        Power_Minus17dBm,


    }

    /// <summary>Channels the RF231 radio can use</summary>
    public enum Channels
    {
        /// <summary>
        /// Channel 11 Frequency 2405 MHz
        /// </summary>
        Channel_11,
        /// <summary>
        /// Channel 12 Frequency 2410 MHz
        /// </summary>
        Channel_12,
        /// <summary>
        /// Channel 13 Frequency 2415 MHz
        /// </summary>
        Channel_13,
        /// <summary>
        /// Channel 14 Frequency 2420 MHz
        /// </summary>
        Channel_14,
        /// <summary>
        /// Channel 15 Frequency 2425 MHz
        /// </summary>
        Channel_15,
        /// <summary>
        /// Channel 16 Frequency 2430 MHz
        /// </summary>
        Channel_16,
        /// <summary>
        /// Channel 17 Frequency 2435 MHz
        /// </summary>
        Channel_17,
        /// <summary>
        /// Channel 18 Frequency 2440 MHz
        /// </summary>
        Channel_18,
        /// <summary>
        /// Channel 19 Frequency 2445 MHz
        /// </summary>
        Channel_19,
        /// <summary>
        /// Channel 20 Frequency 2450 MHz
        /// </summary>
        Channel_20,
        /// <summary>
        /// Channel 21 Frequency 2455 MHz
        /// </summary>
        Channel_21,
        /// <summary>
        /// Channel 22 Frequency 2460 MHz
        /// </summary>
        Channel_22,
        /// <summary>
        /// Channel 23 Frequency 2465 MHz
        /// </summary>
        Channel_23,
        /// <summary>
        /// Channel 24 Frequency 2470 MHz
        /// </summary>
        Channel_24,
        /// <summary>
        /// Channel 25 Frequency 2475 MHz
        /// </summary>
        Channel_25,
        /// <summary>
        /// Channel 26  Frequency 2480 MHz
        /// </summary>
        Channel_26,
    }


    /// <summary>
    /// 802.15.4 radio configuration
    /// </summary>
    public class Radio_802_15_4_Base : NativeEventDispatcher, IRadio
    {
        // Size of the radio message
        const byte RadioPacketSize = 128;

        // Size of the radio configuration byte array for marshalling purposes
        // Note we are marshalling because NETMF does not support passing custom types to native code
        const byte RadioConfigSize = 3;

        byte[] dataBuffer = new byte[RadioPacketSize];

        Packet packet;

        private RadioName radioName;

        /// <summary>Set the name of the radio</summary>
        /// <param name="radioName">Name of the radio</param>
        public void SetRadioName(RadioName radioName)
        {
            this.radioName = radioName;
        }

        /// <summary>Get the kind radio</summary>
        /// <returns>Radio kind</returns>
        public RadioName GetRadioName()
        {
            return this.radioName;
        }

        /// <summary>
        /// Radio configuration
        /// </summary>
        public static RadioConfiguration Config = null;

        /// <summary>Radio configuration</summary>
        /// <value>Configuration</value>
        [Obsolete("Use Config instead")]
        public static RadioConfiguration config {
            get { return Config; }
            set { Config = value; }
        }

        // Create a buffer that you can use when you want to marshal
        byte[] marshalBuffer = new byte[RadioConfigSize];


        /// <summary>
        /// Current user of the radio (C# or MAC objects)
        /// </summary>
        /// <value>Current user</value>
        public static RadioUser CurrUser = RadioUser.IDLE;

        /// <summary>
        /// Current user of the radio (C# or MAC objects)
        /// </summary>
        [Obsolete("Use CurrUser instead")]
        public static RadioUser currUser { 
            get{return CurrUser; }
            set { CurrUser = value; }
        }

        /// <summary>
        /// Constructor for 802.15.4 radio
        /// </summary>
        /// <exception caption="RadioNotConfigured Exception" cref="RadioNotConfiguredException"></exception>
        public Radio_802_15_4_Base()
            : base("RadioCallback_802_15_4", 1234)
        {
            if (Config == null || Callbacks.GetReceiveCallback() == null)
                throw new RadioNotConfiguredException();

            Initialize(Config);
        }

        /// <summary>Constructor for 802.15.4 radio, specifying driver name and data</summary>
        /// <param name="drvname">Driver name</param>
        /// <param name="drvData">Driver data</param>
        /// <exception caption="RadioNotConfigured Exception" cref="RadioNotConfiguredException"></exception>
        public Radio_802_15_4_Base(string drvname, ulong drvData)
            : base(drvname, drvData)
        {
            if (Config == null)
                Debug.Print("The Configuration is null\n");
            
            if (Config == null ||  Callbacks.GetReceiveCallback() == null)
                throw new RadioNotConfiguredException();

            ShallowInitialize(Config);
        }

        /// <summary>Releases the message packet's memory.</summary>
        /// <remarks>Normally the packet's memory will be released during a subsequent GetNextPacket call. This method releases the memory immediately.</remarks>
        /// <returns></returns>
        public void ClearPacket()
        {
            packet = null;
        }

        /*
        private static Radio_802_15_4 instance;
        private static object syncObject = new Object();

        public static Radio_802_15_4 GetInstance()
        {
            if (instance == null)
            {
                lock (syncObject)
                {
                    if (instance == null)
                        instance = new Radio_802_15_4();
                }
            }

            return instance;
        }

        /// <summary>
        /// Get a shallow instance of the radio object, should only be used by the mac 
        /// </summary>
        /// <param name="user"></param>
        /// <returns>Returns a radio object</returns>
        public static Radio_802_15_4 GetShallowInstance(RadioUser user)
        {
            if (instance == null)
            {
                lock (syncObject)
                {
                    if (instance == null)
                    {
                        if (user == RadioUser.CSMAMAC)
                            instance = new Radio_802_15_4("CSMACallback", 4321);
                        else if (user == RadioUser.OMAC)
                            instance = new Radio_802_15_4("OMACCallback", 4322);
                        else if (user == RadioUser.CSharp)
                            instance = new Radio_802_15_4();
                    }
                }
            }

            return instance;
        }
         * */

        /// <summary>Get the next packet from the radio driver.</summary>
        /// <remarks>The radio does not maintain a buffer so the onus is on the application to sample this data as quickly as possible on getting a receive interrupt.
        /// Otherwise the packet is overwritten in the radio layer. For buffer support use the MAC interface</remarks>
        /// <returns>A data packet of message type to the caller</returns>
        public Packet GetNextPacket()
        {
            if (GetNextPacket(dataBuffer) != DeviceStatus.Success)
                return null;

            packet = new Packet(dataBuffer);

            return packet;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus GetNextPacket(byte[] nativeBuffer);


        private DeviceStatus ShallowInitialize(RadioConfiguration config)
        {
            NativeEventHandler eventHandler = new NativeEventHandler(Callbacks.ReceiveFunction);
            OnInterrupt += eventHandler;

            return DeviceStatus.Success;
        }
        
        /// <summary>
        /// Initialize native radio and interop drivers.
        /// </summary>
        /// <param name="config">MAC configuration.</param>
        /// <returns>The status after the method call: Success, Fail, Ready, Busy</returns>
        private DeviceStatus Initialize(RadioConfiguration config)
        {
            NativeEventHandler eventHandler = new NativeEventHandler(Callbacks.ReceiveFunction);
            OnInterrupt += eventHandler;
            marshalBuffer[0] = (byte)config.GetChannel();
            marshalBuffer[1] = (byte) config.GetTxPower();
            marshalBuffer[2] = (byte)config.GetRadioName();
            return InternalInitialize(marshalBuffer);
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus InternalInitialize(byte[] config);    // Changed to private by Bill Leal 2/6/2013 per Mukundan Sridharan.


		/// <summary>Set configuration for 802.15.4 radio.</summary>
		/// <param name="config">Configuration to use</param>
		/// <param name="rcallback">Packet receive callback</param>
		/// <param name="ncallback">Neighbor change callback</param>
		/// <returns>Status of operation</returns>
		public static DeviceStatus Configure(RadioConfiguration config) {
			Radio_802_15_4_Base.Config = new RadioConfiguration(config);
			Callbacks.SetReceiveCallback(config.GetReceiveCallBack());
			Callbacks.SetNeighborChangeCallback(config.GetNeighborChangeCallBack());

			return DeviceStatus.Success;
		}

        /*/// <summary>
        /// Set Radio configuration for 802.15.4 radio.
        /// </summary>
        /// <param name="config">Configuration to use</param>
        /// <remarks>Used to change the Radio configuration during and after initialization, using this function can change the callback if a different callback is used. Please use reconfigure to change power and channel</remarks>
        /// <returns>Status of operation</returns>
        public static DeviceStatus Configure(RadioConfiguration config)
        {
            DeviceStatus result = DeviceStatus.Success;
            Radio_802_15_4_Base.Config = new RadioConfiguration(config);
            return result;
        }*/

		/// <summary>Set configuration for 802.15.4 radio.</summary>
		/// <param name="config">Configuration to use</param>
		/// <param name="rcallback">Packet receive callback</param>
		/// <param name="ncallback">Neighbor change callback</param>
		/// <returns>Status of operation</returns>
		[Obsolete("Deprecated. Use Configure with NeighborhoodChangeCallBack instead")]
		public static DeviceStatus Configure(RadioConfiguration config, ReceiveCallBack rcallback, NeighbourhoodChangeCallBack ncallback) {
			Radio_802_15_4_Base.Config = new RadioConfiguration(config);
			Callbacks.SetReceiveCallback(rcallback);
			Callbacks.SetNeighbourChangeCallback(ncallback);

			return DeviceStatus.Success;

		}

        /// <summary>Set the transmit power of the radio</summary>
        /// <param name="radioID">Radio ID</param>
        /// <param name="TxPower">Transmission power to use</param>
        /// <returns>Status of operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetTxPower(byte radioID, int TxPower);

        /// <summary>Set the radio channel</summary>
        /// <param name="radioID">Radio ID</param>
        /// <param name="Channel">Channel to use</param>
        /// <returns>Status of operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetChannel(byte radioID, int Channel);

        /// <summary>Return the transmission power value of the radio</summary>
        /// <param name="radioID">Radio ID</param>
        /// <returns>Transmission power value</returns>
        public TxPowerValue GetTxPowerValue(byte radioID)
        {
            return (TxPowerValue)GetTxPower(radioID);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern int GetTxPower(byte radioID);


        /// <summary>
        /// Return the current active channel of the radio 
        /// </summary>
        /// <param name="radioID">Radio ID</param>
        /// <returns>Active channel</returns>
        public Channels GetActiveChannel(byte radioID)
        {
            return (Channels)GetChannel(radioID);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern int GetChannel(byte radioID);

        /// <summary>Reconfigure the radio object with new configuration</summary>
        /// <param name="config">Configuration to use</param>
        /// <returns>Status of operation</returns>
        public DeviceStatus ReConfigure(RadioConfiguration config)
        {
            marshalBuffer[0] = (byte) config.GetTxPower();
            marshalBuffer[1] = (byte) config.GetChannel();

            return ReConfigure(marshalBuffer);

        }

        //Make the interop call to set the new configuration 
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus ReConfigure(byte[] data);

        /// <summary>Uninitialize native MAC, radio and interop drivers</summary>
        /// <returns>Status of operation.</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus UnInitialize();

        /*/// <summary>Get the ID of this 802.15.4 radio instance</summary>
        /// <returns>The ID of the instance</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetID();*/

        /// <summary>Get the address of the device</summary>
        /// <remarks>This is the address by which the device is known to the rest of the world.</remarks>
        /// <param name="radioID">Radio ID</param>
        /// <returns>Address of the device</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt16 GetAddress(byte radioID);

        /// <summary>Set the address of the device</summary>
        /// <param name="radioID">Radio ID</param>
        /// <param name="address">Address of the device</param>
        /// <remarks>This is the address by which the device is known to the rest of the world. 
        ///     A return value of false can occur if another layer locks the address and prevents changes.
        /// </remarks>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetAddress(byte radioID, UInt16 address);

        /// <summary>Turn radio on</summary>
        /// <param name="radioID">Radio ID</param>
        /// <returns>Status of operation</returns>
        /// <seealso cref="M:Samraksh.eMote.Net.Radio.Radio_802_15_4_Base.Sleep(System.Byte)">Sleep Method</seealso>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus TurnOnRx(byte radioID);

        /// <summary>Put the radio to sleep</summary>
        /// <param name="radioID">Radio ID</param>
        /// <param name="level">Sleep level</param>
        /// <returns>Status of operation</returns>
        /// <seealso cref="M:Samraksh.eMote.Net.Radio.Radio_802_15_4_Base.TurnOn">TurnOn Method</seealso>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus Sleep(byte radioID, byte level);

        /// <summary>
        /// Load the message into the transmit buffer of the radio.
        /// </summary>
        /// <param name="message">Packet to load</param>
        /// <param name="size">Size of message</param>
        /// <returns>The result of the method: E_RadioInit, E_RadioSync, E_RadioConfig, E_MacInit, E_MacConfig, E_MacSendError, E_MacBufferFull, S_Success</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus PreLoad(byte[] message, UInt16 size);

        /// <summary>Send the message in the transmit buffer</summary>
        /// <returns>Result of operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus SendStrobe();	//Send preloaded message

        /// <summary>Load and send a message</summary>
        /// <param name="radioID">Radio ID</param>
        /// <param name="message">Packet to be sent</param>
        /// <param name="size">Size of message</param>
        /// <returns>Result of operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus Send(byte radioID, byte[] message, UInt16 size);

        /// <summary>Load and send a time-stamped message, with specified time stamp</summary>
        /// <param name="radioID">Radio ID</param>
        /// <param name="message">Packet to be sent</param>
        /// <param name="size">Size of message</param>
        /// <param name="eventTime">The time stamp.</param>
        /// <remarks>The offset for the timestamp in the packet is specified by TimeStampOffset  member of the RadioConfiguration structure passed as parameter during radio module initialization.</remarks>
        /// <returns>The result of the method: E_RadioInit, E_RadioSync, E_RadioConfig, E_MacInit, E_MacConfig, E_MacSendError, E_MacBufferFull, S_Success</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus SendTimeStamped(byte radioID, byte[] message, UInt16 size, UInt32 eventTime);

        /// <summary>Assess channel activity</summary>
        /// <remarks>Default is 140 microseconds.</remarks>
        /// <param name="radioID">Radio ID</param>
        /// <returns>True iff channel is free</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ClearChannelAssesment(byte radioID);

        /// <summary>Assess channel activity</summary>
        /// <param name="radioID">Radio ID</param>
        /// <param name="numberOfMicroSecond">Number of microseconds to check</param>
        /// <returns>True iff channel is free</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ClearChannelAssesment(byte radioID, UInt16 numberOfMicroSecond);
    }
}
