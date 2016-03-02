using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

//using Samraksh.eMote.Net.Radio;
using System.Runtime.CompilerServices;

namespace Samraksh.eMote.Net.Radio
{

    /// <summary>Kind of radio</summary>
    public enum RadioType
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
    public enum Channel
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

        /*/// <summary>
        /// 
        /// </summary>
        public RadioName RadioName;*/

        /*/// <summary>Set the name of the radio</summary>
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
        }*/

        /// <summary>
        /// Radio configuration
        /// </summary>
        public static RadioConfiguration RadioBaseConfig = null;

        /// <summary>Radio configuration</summary>
        /// <value>Configuration</value>
        [Obsolete("Use RadioBaseConfig instead")]
        public static RadioConfiguration config {
            get { return RadioBaseConfig; }
            set { RadioBaseConfig = value; }
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
            if (RadioBaseConfig == null || Callbacks.GetReceiveCallback() == null)
                throw new RadioNotConfiguredException();

            Initialize(RadioBaseConfig);
        }

        /// <summary>Constructor for 802.15.4 radio, specifying driver name and data</summary>
        /// <param name="drvname">Driver name</param>
        /// <param name="drvData">Driver data</param>
        /// <exception caption="RadioNotConfigured Exception" cref="RadioNotConfiguredException"></exception>
        public Radio_802_15_4_Base(string drvname, ulong drvData)
            : base(drvname, drvData)
        {
            if (RadioBaseConfig == null)
                Debug.Print("The Configuration is null\n");

            if (RadioBaseConfig == null || Callbacks.GetReceiveCallback() == null)
                throw new RadioNotConfiguredException();

            Initialize(RadioBaseConfig);
        }

        /*
        private static Radio_802_15_4 RadioInstance;
        private static object syncObject = new Object();

        public static Radio_802_15_4 GetInstance()
        {
            if (RadioInstance == null)
            {
                lock (syncObject)
                {
                    if (RadioInstance == null)
                        RadioInstance = new Radio_802_15_4();
                }
            }

            return RadioInstance;
        }

        /// <summary>
        /// Get a shallow RadioInstance of the radio object, should only be used by the mac 
        /// </summary>
        /// <param name="user"></param>
        /// <returns>Returns a radio object</returns>
        public static Radio_802_15_4 GetShallowInstance(RadioUser user)
        {
            if (RadioInstance == null)
            {
                lock (syncObject)
                {
                    if (RadioInstance == null)
                    {
                        if (user == RadioUser.CSMAMAC)
                            RadioInstance = new Radio_802_15_4("CSMACallback", 4321);
                        else if (user == RadioUser.OMAC)
                            RadioInstance = new Radio_802_15_4("OMACCallback", 4322);
                        else if (user == RadioUser.CSharp)
                            RadioInstance = new Radio_802_15_4();
                    }
                }
            }

            return RadioInstance;
        }
        */

        private DeviceStatus Initialize()
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
            RadioBaseConfig = new RadioConfiguration(config);
            NativeEventHandler eventHandler = new NativeEventHandler(Callbacks.ReceiveFunction);
            OnInterrupt += eventHandler;
            marshalBuffer[0] = (byte)config.Channel;
            marshalBuffer[1] = (byte)config.TxPower;
            marshalBuffer[2] = (byte)config.RadioType;
            return InternalInitialize(marshalBuffer);
        }
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus InternalInitialize(byte[] config);    // Changed to private by Bill Leal 2/6/2013 per Mukundan Sridharan.
        
		/*/// <summary>Set configuration for 802.15.4 radio.</summary>
		/// <param name="config">Configuration to use</param>
		/// <returns>Status of operation</returns>
		public static DeviceStatus Configure(RadioConfiguration config) 
        {
            Radio_802_15_4_Base.RadioBaseConfig = new RadioConfiguration(config);
            Callbacks.SetReceiveCallback(config.OnReceiveCallback);
            Callbacks.SetNeighborChangeCallback(config.OnNeighborChangeCallback);

			return DeviceStatus.Success;
		}*/

        /// <summary>
        /// 
        /// </summary>
        /// <param name="RadioConfig"></param>
        /// <returns></returns>
        public DeviceStatus ReConfigure(RadioConfiguration RadioConfig)
        {
            if (RadioBaseConfig == null || RadioConfig == null)
            {
                throw new RadioNotConfiguredException();
            }
            //RadioBaseConfig = new RadioConfiguration(config);
            if (RadioConfig.OnReceiveCallback == null || RadioConfig.OnNeighborChangeCallback == null)
            {
                throw new RadioNotConfiguredException();
            }
            Callbacks.SetReceiveCallback(RadioConfig.OnReceiveCallback);
            Callbacks.SetNeighborChangeCallback(RadioConfig.OnNeighborChangeCallback);

            marshalBuffer[0] = (byte)RadioConfig.TxPower;
            marshalBuffer[1] = (byte)RadioConfig.Channel;
            marshalBuffer[2] = (byte)RadioConfig.RadioType;

            return ReConfigure(marshalBuffer);
        }

        /// <summary>Reconfigure the radio object with new configuration</summary>
        /// <returns>Status of operation</returns>
        private DeviceStatus Configure()
        {
            if (RadioBaseConfig == null)
            {
                throw new RadioNotConfiguredException();
            }
            //RadioBaseConfig = new RadioConfiguration(config);
            Callbacks.SetReceiveCallback(RadioBaseConfig.OnReceiveCallback);
            Callbacks.SetNeighborChangeCallback(RadioBaseConfig.OnNeighborChangeCallback);

            marshalBuffer[0] = (byte)RadioBaseConfig.TxPower;
            marshalBuffer[1] = (byte)RadioBaseConfig.Channel;
            marshalBuffer[2] = (byte)RadioBaseConfig.RadioType;

            return ReConfigure(marshalBuffer);
        }

        //Make the interop call to set the new configuration 
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus ReConfigure(byte[] buffer);

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
		public static DeviceStatus Configure(RadioConfiguration config, ReceiveCallBack rcallback, NeighbourhoodChangeCallBack ncallback) 
        {
            Radio_802_15_4_Base.RadioBaseConfig = new RadioConfiguration(config);
			Callbacks.SetReceiveCallback(rcallback);
			Callbacks.SetNeighbourChangeCallback(ncallback);

			return DeviceStatus.Success;
		}

        /*/// <summary>Get the next packet from the radio driver.</summary>
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
        private extern DeviceStatus GetNextPacket(byte[] nativeBuffer);*/

        /// <summary>Releases the message packet's memory.</summary>
        /// <remarks>Normally the packet's memory will be released during a subsequent GetNextPacket call. This method releases the memory immediately.</remarks>
        /// <returns></returns>
        public void ClearPacket()
        {
            packet = null;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public void SetReceiveCallback(ReceiveCallBack OnReceiveCallback)
        {
            RadioBaseConfig.OnReceiveCallback = OnReceiveCallback;
            Callbacks.SetReceiveCallback(OnReceiveCallback);
        }

        /// <summary>
        /// 
        /// </summary>
        public void SetNeighborChangeCallback(NeighborhoodChangeCallBack OnNeighborChangeCallback)
        {
            RadioBaseConfig.OnNeighborChangeCallback = OnNeighborChangeCallback;
            Callbacks.SetNeighborChangeCallback(OnNeighborChangeCallback);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public DeviceStatus SetRadioType(RadioType RadioType)
        {
            RadioBaseConfig.RadioType = RadioType;
            return Configure();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="TxPower"></param>
        /// <returns></returns>
        public DeviceStatus SetTxPower(TxPowerValue TxPower)
        {
            RadioBaseConfig.TxPower = TxPower;
            return Configure();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="Channel"></param>
        /// <returns></returns>
        public DeviceStatus SetChannel(Channel Channel)
        {
            RadioBaseConfig.Channel = Channel;
            return Configure();
        }

        /// <summary>Set the transmit power of the radio</summary>
        /// <param name="radioType">Radio ID</param>
        /// <param name="TxPower">Transmission power to use</param>
        /// <returns>Status of operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus SetTxPower(byte radioType, int TxPower);

        /// <summary>Set the radio channel</summary>
        /// <param name="radioType">Radio ID</param>
        /// <param name="Channel">Channel to use</param>
        /// <returns>Status of operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus SetChannel(byte radioType, int Channel);

        /// <summary>Return the transmission power value of the radio</summary>
        /// <param name="radioType">Radio ID</param>
        /// <returns>Transmission power value</returns>
        public TxPowerValue GetTxPowerValue(byte radioType)
        {
            return (TxPowerValue)GetTxPower(radioType);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern int GetTxPower(byte radioType);

        /// <summary>
        /// Return the current active channel of the radio 
        /// </summary>
        /// <param name="radioType">Radio ID</param>
        /// <returns>Active channel</returns>
        public Channel GetChannel(byte radioType)
        {
            return (Channel)GetActiveChannel(radioType);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern int GetActiveChannel(byte radioType);

        /// <summary>Uninitialize native MAC, radio and interop drivers</summary>
        /// <returns>Status of operation.</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus UnInitialize();

        /*/// <summary>Get the ID of this 802.15.4 radio RadioInstance</summary>
        /// <returns>The ID of the RadioInstance</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetID();*/

        /// <summary>Get the address of the device</summary>
        /// <remarks>This is the address by which the device is known to the rest of the world.</remarks>
        /// <param name="radioType">Radio ID</param>
        /// <returns>Address of the device</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt16 GetAddress(byte radioType);

        /// <summary>Set the address of the device</summary>
        /// <param name="radioType">Radio ID</param>
        /// <param name="address">Address of the device</param>
        /// <remarks>This is the address by which the device is known to the rest of the world. 
        ///     A return value of false can occur if another layer locks the address and prevents changes.
        /// </remarks>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetAddress(byte radioType, UInt16 address);

        /// <summary>Turn radio on</summary>
        /// <param name="radioType">Radio ID</param>
        /// <returns>Status of operation</returns>
        /// <seealso cref="M:Samraksh.eMote.Net.Radio.Radio_802_15_4_Base.Sleep(System.Byte)">Sleep Method</seealso>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus TurnOnRx(byte radioType);

        /// <summary>Turn radio off</summary>
        /// <param name="radioType">Radio ID</param>
        /// <returns>Status of operation</returns>
        /// <seealso cref="M:Samraksh.eMote.Net.Radio.Radio_802_15_4_Base.Sleep(System.Byte)">Sleep Method</seealso>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus TurnOffRx(byte radioType);

        /// <summary>Put the radio to sleep</summary>
        /// <param name="radioType">Radio ID</param>
        /// <param name="level">Sleep level</param>
        /// <returns>Status of operation</returns>
        /// <seealso cref="M:Samraksh.eMote.Net.Radio.Radio_802_15_4_Base.TurnOn">TurnOn Method</seealso>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus Sleep(byte radioType, byte level);

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
        public extern NetOpStatus SendStrobe(byte radioType, UInt16 size);	//Send preloaded message

        /// <summary>Load and send a message</summary>
        /// <param name="radioType">Radio ID</param>
        /// <param name="message">Packet to be sent</param>
        /// <param name="size">Size of message</param>
        /// <returns>Result of operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus Send(byte radioType, byte[] message, UInt16 size);

        /// <summary>Load and send a time-stamped message, with specified time stamp</summary>
        /// <param name="radioType">Radio ID</param>
        /// <param name="message">Packet to be sent</param>
        /// <param name="size">Size of message</param>
        /// <param name="eventTime">The time stamp.</param>
        /// <remarks>The offset for the timestamp in the packet is specified by TimeStampOffset  member of the RadioConfiguration structure passed as parameter during radio module initialization.</remarks>
        /// <returns>The result of the method: E_RadioInit, E_RadioSync, E_RadioConfig, E_MacInit, E_MacConfig, E_MacSendError, E_MacBufferFull, S_Success</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus SendTimeStamped(byte radioType, byte[] message, UInt16 size, UInt32 eventTime);

        /// <summary>Assess channel activity</summary>
        /// <remarks>Default is 140 microseconds.</remarks>
        /// <param name="radioType">Radio ID</param>
        /// <returns>True iff channel is free</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ClearChannelAssesment(byte radioType);

        /// <summary>Assess channel activity</summary>
        /// <param name="radioType">Radio ID</param>
        /// <param name="numberOfMicroSecond">Number of microseconds to check</param>
        /// <returns>True iff channel is free</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ClearChannelAssesment(byte radioType, UInt16 numberOfMicroSecond);
    }
}
