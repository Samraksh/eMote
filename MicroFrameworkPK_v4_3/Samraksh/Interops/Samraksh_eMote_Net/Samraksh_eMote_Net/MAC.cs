using System;
using Microsoft.SPOT;
using Samraksh.eMote.Net.Mac;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Samraksh.eMote.Net
{
    /*/// <summary>
    /// Custom exception class for eMote.Net namespace
    /// </summary>
    public class eMoteNetException : Exception
    {
        public eMoteNetException() { }
        public eMoteNetException(string packet) { }
        public eMoteNetException(string packet, Exception innerException) { }
    }*/

    /// <summary>Kinds of protocol</summary>
    public enum MACType
    {
        /// <summary>
        /// 
        /// </summary>
        CSMA,
        /// <summary>
        /// 
        /// </summary>
        OMAC,
    }

    // Making this private as the user should really never be seeing this 
    /// <summary>Base class for wireless protocols</summary>
    /// <seealso cref="Mac.CSMA" cat="Inherited by">CSMA Class</seealso>
    /// <seealso cref="OMAC" cat="Inherited by">OMAC Class</seealso>
    public class MACBase : IMAC
    {
        /// <summary>
        /// Specifies the marshalling buffer size, used by the config to pass data to native code 
        /// </summary>
        const byte MarshalBufferSize = 12;

        const byte MacPacketSize = 126;
        /// <summary>
        /// Specifies the neighbor size
        /// </summary>
        const byte NeighborSize = 22; //Look at IMAC.cs to figure out the size of the Neighbor structure.

        const byte MaxNeighbors = 255;

		UInt16[] NeighborList = new UInt16[MaxNeighbors];

		byte[] ByteNeighbor = new byte[NeighborSize];

		byte[] MarshalBuffer = new byte[MarshalBufferSize];

        /// <summary>Configure MAC (obsolete)</summary>
        /// <value>Accesses MACConfig</value>
        [Obsolete("Use MACConfig instead")]
        public static MACConfiguration macconfig {
            get { return MACConfig; }
            set { MACConfig = value; }
        }

        /// <summary>
        /// MAC Config
        /// </summary>
        public static MACConfiguration MACConfig = null;
       
        private Packet packet;

        private MACType MACType;

        private static Neighbor neighbor = new Neighbor();

        static byte[] dataBuffer = new byte[MacPacketSize];
        
        /// <summary>
        /// 
        /// </summary>
        public static Radio.Radio_802_15_4_Base MACRadio;

        /*/// <summary>
        /// 
        /// </summary>
        /// <param name="macConfig"></param>
        public MACBase(MACConfiguration macConfig)
        {
            if (MACConfig == null)
            {
                MACConfig = new MACConfiguration(macConfig);
                
                // Configure the radio 
                if (Radio.Radio_802_15_4.Configure(MACConfig.MACRadioConfig) != DeviceStatus.Success)
                    throw new MACNotConfiguredException("Radio could not be configured");
            }
            else
            {
                // Configure the radio
                if (Radio.Radio_802_15_4.Configure(MACConfig.MACRadioConfig) != DeviceStatus.Success)
                    throw new MACNotConfiguredException("Radio could not be configured");
            }
        }*/

        /// <summary>
        /// Constructor that takes in a mactype parameter and initializes the corresponding mac object
        /// </summary>
        /// <param name="MACType">CSMA, OMAC or other MAC</param>
        /// <exception caption="MacNotConfigured Exception" cref="MACNotConfiguredException"></exception>
        /// <exception caption="System Exception" cref="System.SystemException"></exception>
        public MACBase(MACType MACType)
        {
            if (MACConfig == null || Callbacks.GetReceiveCallback() == null)
                throw new MACNotConfiguredException();

            if (MACType == MACType.CSMA)
            {
                Radio.Radio_802_15_4.CurrUser = Radio.RadioUser.CSMAMAC;
                if (MACConfig.MACRadioConfig.RadioType == Radio.RadioType.RF231RADIO)
                    MACRadio = Radio.Radio_802_15_4.GetInstance(Radio.RadioUser.CSMAMAC);
                else if (MACConfig.MACRadioConfig.RadioType == Radio.RadioType.RF231RADIOLR)
                    MACRadio = Radio.Radio_802_15_4_LR.GetInstance(Radio.RadioUser.CSMAMAC);
            }
            else if (MACType == MACType.OMAC)
            {
                Radio.Radio_802_15_4.CurrUser = Radio.RadioUser.OMAC;
                if (MACConfig.MACRadioConfig.RadioType == Radio.RadioType.RF231RADIO)
                    MACRadio = Radio.Radio_802_15_4.GetInstance(Radio.RadioUser.OMAC);
                else if (MACConfig.MACRadioConfig.RadioType == Radio.RadioType.RF231RADIOLR)
                    MACRadio = Radio.Radio_802_15_4_LR.GetInstance(Radio.RadioUser.OMAC);
            }

            this.MACType = MACType;
            //this.neighbor = new Neighbor();

            DeviceStatus status = Initialize(MACConfig, MACType);

            if (status != DeviceStatus.Success)
            {
                throw new MACNotConfiguredException("Mac initialization failed. One reason for failure could be that a USB cable is attached to the DotNow.\n");
            }
        }

        /// <summary>
        /// Initialize the mac after marshalling the config
        /// </summary>
        /// <param name="config"></param>
        /// <param name="mactype"></param>
        /// <returns></returns>
        private DeviceStatus Initialize(MACConfiguration config, MACType mactype)
        {
            if (config.CCA)
                MarshalBuffer[0] = 1;
            else
                MarshalBuffer[0] = 0;

            MarshalBuffer[1] = config.NumberOfRetries;
            MarshalBuffer[2] = config.CCASenseTime;
            MarshalBuffer[3] = config.BufferSize;
            MarshalBuffer[4] = config.RadioType;
            MarshalBuffer[5] = (byte) (config.NeighborLivenessDelay & 0xff);
            MarshalBuffer[6] = (byte)((config.NeighborLivenessDelay& 0xff00) >> 8);
            MarshalBuffer[7] = (byte)((config.NeighborLivenessDelay& 0xff0000) >> 16);
            MarshalBuffer[8] = (byte)((config.NeighborLivenessDelay& 0xff000000) >> 24);
            // Breaking the object boundary, but shallow instances of the radio can not initialize
            MarshalBuffer[9] = (byte)config.MACRadioConfig.TxPower;
            MarshalBuffer[10] = (byte)config.MACRadioConfig.Channel;
            MarshalBuffer[11] = (byte)config.MACRadioConfig.RadioType;

            return InternalInitialize(MarshalBuffer, (byte) mactype);
        }

        /*/// <summary>Configure the MAC object. Must be called before a call to get RadioInstance</summary>
        /// <param name="macConfig">MAC configuration to use</param>
        /// <param name="receiveCallback">Method to call when packet received</param>
        /// <param name="neighborChangeCallback">Method to call when neighborhood changed</param>
        /// <returns>Status of operation</returns>
        //public static DeviceStatus Configure(MACConfiguration config, ReceiveCallBack receiveCallback, NeighborhoodChangeCallBack neighborChangeCallback)
        public DeviceStatus Configure(MACConfiguration macConfig)
        {
            if (MACConfig == null)
            {
                MACConfig = new MACConfiguration(macConfig);
                //Callbacks.SetReceiveCallback(receiveCallback);
                //Callbacks.SetNeighborChangeCallback(neighborChangeCallback);

                // Configure the radio 
                if (Radio.Radio_802_15_4.Configure(MACConfig.MACRadioConfig) == DeviceStatus.Busy)
                    return DeviceStatus.Busy;
            }
            else
            {
                //Callbacks.SetReceiveCallback(receiveCallback);
                //Callbacks.SetNeighborChangeCallback(neighborChangeCallback);

                // Configure the radio
                if (Radio.Radio_802_15_4.Configure(MACConfig.MACRadioConfig) == DeviceStatus.Busy)
                    return DeviceStatus.Busy;
            }
            return DeviceStatus.Success;
        }*/

        /// <summary>Reconfigure MAC</summary>
        private DeviceStatus Configure()
        {
            //MACConfig = macConfig;
            if (MACConfig == null)
            {
                throw new MACNotConfiguredException("MAC not configured");
            }

            if (this.MACType != MACType.CSMA && this.MACType != MACType.OMAC)
            {
                throw new MACNotConfiguredException("MAC type not known");
            }

            //if (MACRadio.RadioName != macConfig.MACRadioConfig.RadioName)
            //{
            /*if (this.MACType == MACType.CSMA)
            {
                if (MACConfig.MACRadioConfig.RadioType == Radio.RadioType.RF231RADIOLR)
                    MACRadio = Radio.Radio_802_15_4_LR.GetInstance(Radio.RadioUser.CSMAMAC);
                else
                    MACRadio = Radio.Radio_802_15_4.GetInstance(Radio.RadioUser.CSMAMAC);
            }
            else if (this.MACType == MACType.OMAC)
            {
                if (MACConfig.MACRadioConfig.RadioType == Radio.RadioType.RF231RADIOLR)
                    MACRadio = Radio.Radio_802_15_4_LR.GetInstance(Radio.RadioUser.OMAC);
                else
                    MACRadio = Radio.Radio_802_15_4.GetInstance(Radio.RadioUser.OMAC);
            }
            else
            {
                throw new MACNotConfiguredException("MAC type not known");
            }*/
            //}

            if (MACConfig.CCA)
                MarshalBuffer[0] = 1;
            else
                MarshalBuffer[0] = 0;

            MarshalBuffer[1] = MACConfig.NumberOfRetries;
            MarshalBuffer[2] = MACConfig.CCASenseTime;
            MarshalBuffer[3] = MACConfig.BufferSize;
            MarshalBuffer[4] = MACConfig.RadioType;
            MarshalBuffer[5] = (byte)(MACConfig.NeighborLivenessDelay & 0xff);
            MarshalBuffer[6] = (byte)((MACConfig.NeighborLivenessDelay & 0xff00) >> 8);
            MarshalBuffer[7] = (byte)((MACConfig.NeighborLivenessDelay & 0xff0000) >> 16);
            MarshalBuffer[8] = (byte)((MACConfig.NeighborLivenessDelay & 0xff000000) >> 24);
            // Breaking the object boundary, but shallow instances of the radio can not initialize
            MarshalBuffer[9] = (byte)MACConfig.MACRadioConfig.TxPower;
            MarshalBuffer[10] = (byte)MACConfig.MACRadioConfig.Channel;
            MarshalBuffer[11] = (byte)MACConfig.MACRadioConfig.RadioType;

            //Change radio's properties as well
            MACRadio.ReConfigure(MACConfig.MACRadioConfig);
            /*MACRadio.SetRadioType(MACConfig.MACRadioConfig.RadioType);
            MACRadio.SetTxPower(MACConfig.MACRadioConfig.TxPower);
            MACRadio.SetChannel(MACConfig.MACRadioConfig.Channel);*/

            return InternalReConfigure(MarshalBuffer);
        }

        /// <summary>
        /// Get the next packet from the mac buffer
        /// </summary>
        /// <returns>Next packet</returns>
        public Packet GetNextPacket()
        {
            for (UInt16 i = 0; i < MacPacketSize; i++)
                dataBuffer[i] = 0;

            if (GetNextPacket(dataBuffer) != DeviceStatus.Success)
                return null;

            if (dataBuffer[0] == 0)
                return null;

            packet = new Packet(dataBuffer);

            return packet;
        }

        /*/// <summary>Returns the radio being used by the MAC</summary>
        /// <returns>The RadioInstance of the radio</returns>
        public Radio.Radio_802_15_4_Base GetRadio()
        {
            return MACRadio;
        }*/

        /// <summary>Set the liveness delay</summary>
        /// <param name="livenessDelay"></param>
        /// <returns>Result of setting this parameter</returns>
        public DeviceStatus SetNeighborLivenessDelay(UInt32 livenessDelay) {
            MACConfig.NeighborLivenessDelay = livenessDelay;
            return Configure();
        }

        /// <summary>Set the liveness delay</summary>
        /// <param name="livelinessDelay"></param>
        /// <returns>Result of setting this parameter</returns>
        [Obsolete("Use SetNeighborLivenessDelay instead")]
        public DeviceStatus SetNeighbourLivelinessDelay(UInt32 livelinessDelay)
        {
            MACConfig.NeighbourLivelinesDelay = livelinessDelay;
            return Configure();
        }

        /*/// <summary>
        /// Get the current liveness delay parameter
        /// </summary>
        /// <returns></returns>
        public UInt32 GetNeighborLivenessDelay() 
        {
            return MACConfig.NeighborLivenessDelay;
        }*/

        /// <summary>
        /// Get the current liveliness delay parameter
        /// </summary>
        /// <returns></returns>
        [Obsolete("Use GetNeighborLivenessDelay instead")]
        public UInt32 GetNeighbourLivelinessDelay()
        {
            return MACConfig.NeighbourLivelinesDelay;
        }

        /// <summary>Enable or disable Clear Channel Assessment</summary>
        /// <param name="CCA">True to enable, false to disable</param>
        /// <returns>Status of operation</returns>
        public DeviceStatus SetCCA(bool CCA)
        {
            MACConfig.CCA = CCA;
            return Configure();
        }

        /// <summary>Set the count of send retries</summary>
        /// <param name="NumberOfRetries">Send retry count</param>
        /// <returns>DeviceStatus</returns>
        public DeviceStatus SetNumberOfRetries(byte NumberOfRetries)
        {
            MACConfig.NumberOfRetries = NumberOfRetries;
            return Configure();
        }

        /// <summary>Set the time to do Clear Channel Assessment</summary>
        /// <param name="CCASenseTime">Time to do Clear Channel Assessment (microseconds)</param>
        /// <returns>Status of operation</returns>
        public DeviceStatus SetCCASenseTime(byte CCASenseTime)
        {
            MACConfig.CCASenseTime = CCASenseTime;
            return Configure();
        }

        /// <summary>Set the size of the buffer</summary>
        /// <param name="BufferSize">Size of the buffer</param>
        /// <returns>Operation status</returns>
        public DeviceStatus SetBufferSize(byte BufferSize)
        {
            MACConfig.BufferSize = BufferSize;
            return Configure();
        }

        /// <summary>
        /// Set Radio type
        /// </summary>
        /// <param name="RadioType">Radio type</param>
        /// <returns>DeviceStatus</returns>
        public DeviceStatus SetRadioType(byte RadioType)
        {
            MACConfig.RadioType = RadioType;
            return Configure();
        }

        /// <summary>Get Clear Channel Assessment</summary>
        /// <returns>True iff channel is clear</returns>
        public bool GetCCA()
        {
            return MACConfig.CCA;
        }

        /// <summary>Get number of times to retry sending</summary>
        /// <returns>Number of times to retry sending</returns>
        public byte GetNumberOfRetries()
        {
            return MACConfig.NumberOfRetries;
        }

        /// <summary>Get Clear Channel Assessment Sense Time</summary>
        /// <returns>Time to sense CCA (microseconds)</returns>
        public byte GetCCASenseTime()
        {
            return MACConfig.CCASenseTime;
        }

        /// <summary>Get the Radio type</summary>
        /// <returns>Radio type</returns>
        public byte GetRadioType()
        {
            return MACConfig.RadioType;
        }
        
        /*/// <summary>Get the buffer size</summary>
        /// <returns>The size of the buffer.</returns>
        public byte GetBufferSize()
        {
            return MACConfig.BufferSize;
        }*/

		/// <summary>Get the list of neighbors from the MAC</summary>
		/// <returns>An array with the list of active neighbors</returns>
		public UInt16[] GetNeighborList() {
			if (GetNeighborListInternal(NeighborList) != DeviceStatus.Success) {
				Debug.Print("Get NeighborListInternal fails\n");
				return null;
			}

			return NeighborList;
		}

		/// <summary>Get the list of neighbors from the MAC</summary>
		/// <returns>An array with the list of active neighbors</returns>
		[Obsolete("Deprecated. Use GetNeighborList instead")]
		public UInt16[] GetNeighbourList() {
			if (GetNeighborListInternal(NeighborList) != DeviceStatus.Success) {
				Debug.Print("Get NeighborListInternal fails\n");
				return null;
			}

			return NeighborList;
		}

		/// <summary>Get the details for a neighbor</summary>
	    /// <param name="macAddress">Address of the neighbor</param>
	    /// <returns>Neighbor</returns>
	    public Neighbor GetNeighborStatus(UInt16 macAddress) {
			if (GetNeighborInternal(macAddress, ByteNeighbor) == DeviceStatus.Success) {
				neighbor.MacAddress = (UInt16)(((UInt16)(ByteNeighbor[1] << 8) & 0xFF00) + (UInt16)ByteNeighbor[0]);//MacAddress
				neighbor.ForwardLink.AveRSSI = ByteNeighbor[2]; //ForwardLink
				neighbor.ForwardLink.LinkQuality = ByteNeighbor[3];
				neighbor.ForwardLink.AveDelay = ByteNeighbor[4];
				neighbor.ReverseLink.AveRSSI = ByteNeighbor[5];  //ReverseLink
				neighbor.ReverseLink.LinkQuality = ByteNeighbor[6];
				neighbor.ReverseLink.AveDelay = ByteNeighbor[7];
				neighbor.Status = (NeighborStatus)ByteNeighbor[8];//Status
				neighbor.PacketsReceived = (UInt16)(((ByteNeighbor[10] << 8) & 0xFF00) + ByteNeighbor[9]);//MacAddress
				neighbor.LastHeardTime = (UInt64)((ByteNeighbor[18] << 56) + ByteNeighbor[17] << 48 + ByteNeighbor[16] << 40 + ByteNeighbor[15] << 32 + ByteNeighbor[14] << 24 +
				ByteNeighbor[13] << 16 + ByteNeighbor[12] << 8 + +ByteNeighbor[11]);//LastTimeHeard
				neighbor.ReceiveDutyCycle = ByteNeighbor[19];//ReceiveDutyCycle
				neighbor.FrameLength = (UInt16)(((ByteNeighbor[21] << 8) & 0xFF00) + ByteNeighbor[20]);
				return neighbor;
			}

			return null;
		}

	    /// <summary>Get the details for a neighbor</summary>
	    /// <param name="macAddress">Address of the neighbor</param>
		/// <returns>Neighbor</returns>
	    [Obsolete("Deprecated. Use GetNeighborStatus instead")]
		public Neighbor GetNeighbourStatus(UInt16 macAddress) {
			if (GetNeighborInternal(macAddress, ByteNeighbor) == DeviceStatus.Success) {
				neighbor.MacAddress = (UInt16)(((UInt16)(ByteNeighbor[1] << 8) & 0xFF00) + (UInt16)ByteNeighbor[0]);//MacAddress
				neighbor.ForwardLink.AveRSSI = ByteNeighbor[2]; //ForwardLink
				neighbor.ForwardLink.LinkQuality = ByteNeighbor[3];
				neighbor.ForwardLink.AveDelay = ByteNeighbor[4];
				neighbor.ReverseLink.AveRSSI = ByteNeighbor[5];  //ReverseLink
				neighbor.ReverseLink.LinkQuality = ByteNeighbor[6];
				neighbor.ReverseLink.AveDelay = ByteNeighbor[7];
				neighbor.Status = (NeighborStatus)ByteNeighbor[8];//Status
				neighbor.PacketsReceived = (UInt16)(((ByteNeighbor[10] << 8) & 0xFF00) + ByteNeighbor[9]);//MacAddress
				neighbor.LastHeardTime = (UInt64)((ByteNeighbor[18] << 56) + ByteNeighbor[17] << 48 + ByteNeighbor[16] << 40 + ByteNeighbor[15] << 32 + ByteNeighbor[14] << 24 +
				ByteNeighbor[13] << 16 + ByteNeighbor[12] << 8 + +ByteNeighbor[11]);//LastTimeHeard
				neighbor.ReceiveDutyCycle = ByteNeighbor[19];//ReceiveDutyCycle
				neighbor.FrameLength = (UInt16)(((ByteNeighbor[21] << 8) & 0xFF00) + ByteNeighbor[20]);
				return neighbor;
			}

			return null;
		}

        /// <summary>
        /// Initialize native MAC, radio and interop drivers.
        /// </summary>
        /// <param name="marshalBuffer"></param>
        /// <param name="mactype"></param>
        /// <returns>The driver status after initialization: Success, Fail, Ready, Busy</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus InternalInitialize(byte[] marshalBuffer, byte mactype);  // Changed to private by Bill Leal 2/6/2013 per Mukundan Sridharan.

        /// <summary>Uninitialize radio</summary>
        /// <returns>Status of operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus UnInitialize();

        /// <summary></summary>
        /// <param name="marshalBuffer"></param>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus InternalReConfigure(byte[] marshalBuffer);

        /// <summary></summary>
        /// <param name="nativeBuffer"></param>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus GetNextPacket(byte[] nativeBuffer);

        /*/// <summary>Releases the memory held by a packet. Make this call after assigning the acquired packet to a packet reference</summary>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void ReleasePacket();*/

        /// <summary>Remove a packet from the buffer</summary>
        /// <param name="packet">packet to be removed</param>
        /// <returns>Status of operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus RemovePacket(byte[] packet);

        /// <summary>Get a count of unprocessed packets in the buffer</summary>
        /// <returns>The number of packets in the buffer not yet delivered to the program</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetPendingPacketCount_Receive();

        /// <summary>Get a count of unprocessed packets in the buffer</summary>
        /// <returns>The number of packets in the buffer not yet delivered to the program</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetPendingPacketCount_Send();

        /// <summary>
        /// 
        /// </summary>
        /// <param name="macAddress"></param>
        /// <param name="neighborlist"></param>
        /// <returns></returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus GetNeighborInternal(UInt16 macAddress, byte[] neighborlist);

        /// <summary>
        /// 
        /// </summary>
        /// <param name="neighborlist"></param>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus GetNeighborListInternal(UInt16[] neighborlist);

        /// <summary>Get the type of this MAC instance</summary>
        /// <returns>The ID of the RadioInstance</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetMACType();

        /// <summary>
        /// Set the address of the device.
        /// </summary>
        /// <param name="address">Address of the device</param>
        /// <remarks>This is the address by which the device is known to the rest of the world. A return value of false can occur if another layer locks the address and prevents changes.</remarks>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetRadioAddress(UInt16 address);

        /// <summary>Get the address of the device</summary>
        /// <remarks>This is the address by which the device is known to the rest of the world</remarks>
        /// <returns>Address of the device</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt16 GetRadioAddress();

        /// <summary>Send a packet</summary>
        /// <param name="address">The address of the receiver. Use <code>Addresses.BROADCAST</code> for broadcast</param>
        /// <param name="payload">Byte array containing the payload to be sent</param>
        /// <param name="offset">The first byte in the array to send. Normally 0</param>
        /// <param name="size">The number of bytes to send</param>
        /// <returns>Status of operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus Send(UInt16 address, byte[] payload, UInt16 offset, UInt16 size);

        /// <summary>Send a time stamped packet. Time stamping is done in native send</summary>
        /// <param name="Address">The address of receiver</param>
        /// <param name="packet">Packet to send</param>
        /// <param name="offset">offset if any in the byte array</param>
        /// <param name="size">size of the packet</param>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus SendTimeStamped(UInt16 Address, byte[] packet, UInt16 offset, UInt16 size);

        /// <summary>Send a time stamped packet. Time stamp is specified as a parameter</summary>
        /// <param name="Address">Address of receiver</param>
        /// <param name="packet">Packet to be sent</param>
        /// <param name="offset">offset if any in the byte array</param>
        /// <param name="size">size of the packet</param>
        /// <param name="eventTime">Time to use for timestamp</param>
        /// <returns>Status of operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus SendTimeStamped(UInt16 Address, byte[] packet, UInt16 offset, UInt16 size, UInt32 eventTime);
	
	}
}
