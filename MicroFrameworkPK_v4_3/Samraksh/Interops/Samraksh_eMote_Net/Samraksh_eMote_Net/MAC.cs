using System;
using Microsoft.SPOT;
using Samraksh.eMote.Net.Mac;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Samraksh.eMote.Net
{

    /// <summary>Kinds of protocol</summary>
    public enum MacID
    {
        CSMA,
        OMAC,
    }

    // Making this private as the user should really never be seeing this 
    /// <summary>Base class for wireless protocols</summary>
    /// <seealso cref="Mac.CSMA" cat="Inherited by">CSMA Class</seealso>
    /// <seealso cref="OMAC" cat="Inherited by">OMAC Class</seealso>
    public class MACBase : IMac
    {
        /// <summary>
        /// Specifies the marshalling buffer size, used by the config to pass data to native code 
        /// </summary>
        const byte MarshalBufferSize = 12;

        const byte MacPacketSize = 126;
        /// <summary>
        /// Specifies the neighbor size
        /// </summary>
        const byte NeighborSize = 22; //Look at IMac.cs to figure out the size of the Neighbor structure.

        const byte MaxNeighbors = 255;

		UInt16[] NeighborList = new UInt16[MaxNeighbors];

		byte[] ByteNeighbor = new byte[NeighborSize];

		byte[] MarshalBuffer = new byte[MarshalBufferSize];

        /// <summary>Configure MAC (obsolete)</summary>
        /// <value>Accesses MacConfig</value>
        [Obsolete("Use MacConfig instead")]
        public static MacConfiguration macconfig {
            get { return MacConfig; }
            set { MacConfig = value; }
        }

        /// <summary>
        /// Mac Config
        /// </summary>
        public static MacConfiguration MacConfig = null;

        private Packet packet;

        private MacID macname;

        private static Neighbor neighbor = new Neighbor();

        static byte[] dataBuffer = new byte[MacPacketSize];
        
        Radio.Radio_802_15_4_Base radioObj;

        /// <summary>
        /// Constructor that takes in a macname parameter and initializes the corresponding mac object
        /// </summary>
        /// <param name="macname">CSMA, OMAC or other MAC</param>
        /// <exception caption="MacNotConfigured Exception" cref="MacNotConfiguredException"></exception>
        /// <exception caption="System Exception" cref="System.SystemException"></exception>
        public MACBase(MacID macname)
        {

            if (MacConfig == null || Callbacks.GetReceiveCallback() == null)
                throw new MacNotConfiguredException();

            if (macname == MacID.CSMA)
            {
                Radio.Radio_802_15_4.CurrUser = Radio.RadioUser.CSMAMAC;
                if (MacConfig.radioConfig.GetRadioName() == Radio.RadioName.RF231RADIO)
                    radioObj = Radio.Radio_802_15_4.GetShallowInstance(Radio.RadioUser.CSMAMAC);
                else if (MacConfig.radioConfig.GetRadioName() == Radio.RadioName.RF231RADIOLR)
                    radioObj = Radio.Radio_802_15_4_LR.GetShallowInstance(Radio.RadioUser.CSMAMAC);
            }
            else if (macname == MacID.OMAC)
            {
                Radio.Radio_802_15_4.CurrUser = Radio.RadioUser.OMAC;
                if (MacConfig.radioConfig.GetRadioName() == Radio.RadioName.RF231RADIO)
                    radioObj = Radio.Radio_802_15_4.GetShallowInstance(Radio.RadioUser.OMAC);
                else if (MacConfig.radioConfig.GetRadioName() == Radio.RadioName.RF231RADIOLR)
                    radioObj = Radio.Radio_802_15_4_LR.GetShallowInstance(Radio.RadioUser.OMAC);
            }

            this.macname = macname;
            //this.neighbor = new Neighbor();

            if (Initialize(MacConfig, macname) != DeviceStatus.Success)
            {
                throw new SystemException("Mac initialization failed\n");
            }

        }

        /// <summary>Releases the memory held by a packet. Make this call after assigning the acquired packet to a packet reference</summary>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void ReleasePacket();


        /// <summary>
        /// Get the next packet from the mac buffer
        /// </summary>
        /// <returns>Next packet</returns>
        public Packet GetNextPacket()
        {
            Array.Clear(dataBuffer, 0, MacPacketSize);

            /*for (UInt16 i = 0; i < MacPacketSize; i++)
                dataBuffer[i] = 0;*/

            if (GetNextPacket(dataBuffer) != DeviceStatus.Success)
                return null;

            if (dataBuffer[0] == 0)
                return null;

            packet = new Packet(dataBuffer);

            return packet;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus GetNextPacket(byte[] nativeBuffer);

        /// <summary>Returns the radio being used by the MAC</summary>
        /// <returns>The instance of the radio</returns>
        public Radio.Radio_802_15_4_Base GetRadio()
        {
            return radioObj;
        }

        /// <summary>
        /// Initialize the mac after marshalling the config
        /// </summary>
        /// <param name="config"></param>
        /// <returns></returns>
        private DeviceStatus Initialize(MacConfiguration config, MacID macname)
        {
            if (config.CCA)
                MarshalBuffer[0] = 1;
            else
                MarshalBuffer[0] = 0;

            MarshalBuffer[1] = config.NumberOfRetries;
            MarshalBuffer[2] = config.CCASenseTime;
            MarshalBuffer[3] = config.BufferSize;
            MarshalBuffer[4] = config.RadioID;
            MarshalBuffer[5] = (byte) (config.NeighborLivenessDelay & 0xff);
            MarshalBuffer[6] = (byte)((config.NeighborLivenessDelay& 0xff00) >> 8);
            MarshalBuffer[7] = (byte)((config.NeighborLivenessDelay& 0xff0000) >> 16);
            MarshalBuffer[8] = (byte)((config.NeighborLivenessDelay& 0xff000000) >> 24);
            // Breaking the object boundary, but shallow instances of the radio can not initialize
            MarshalBuffer[9] = (byte)config.radioConfig.GetTxPower();
            MarshalBuffer[10] = (byte) config.radioConfig.GetChannel();
            MarshalBuffer[11] = (byte)config.radioConfig.GetRadioName();

            return InternalInitialize(MarshalBuffer, (byte) macname);
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus InternalReConfigure(byte[] marshalBuffer, byte macname);


        /// <summary>Reconfigure MAC</summary>
        /// <param name="config">New MAC configuration</param>
        /// <param name="macName">Kind of MAC</param>
        public DeviceStatus ReConfigure(MacConfiguration config, MacID macName)
        {
            MacConfig = config;

            if (radioObj.GetRadioName() != config.radioConfig.GetRadioName())
            {
                if (macName == MacID.CSMA)
                {
                    if (config.radioConfig.GetRadioName() == Radio.RadioName.RF231RADIOLR)
                        radioObj = Radio.Radio_802_15_4_LR.GetShallowInstance(Radio.RadioUser.CSMAMAC);
                    else
                        radioObj = Radio.Radio_802_15_4.GetShallowInstance(Radio.RadioUser.CSMAMAC);
                }
                else if (macName == MacID.OMAC)
                {
                    if (config.radioConfig.GetRadioName() == Radio.RadioName.RF231RADIOLR)
                        radioObj = Radio.Radio_802_15_4_LR.GetShallowInstance(Radio.RadioUser.OMAC);
                    else
                        radioObj = Radio.Radio_802_15_4.GetShallowInstance(Radio.RadioUser.OMAC);
                }
            }
                

            if (config.CCA)
                MarshalBuffer[0] = 1;
            else
                MarshalBuffer[0] = 0;

            MarshalBuffer[1] = config.NumberOfRetries;
            MarshalBuffer[2] = config.CCASenseTime;
            MarshalBuffer[3] = config.BufferSize;
            MarshalBuffer[4] = config.RadioID;
            // Breaking the object boundary, but shallow instances of the radio can not initialize
            MarshalBuffer[5] = (byte)config.radioConfig.GetTxPower();
            MarshalBuffer[6] = (byte) config.radioConfig.GetChannel();
            MarshalBuffer[7] = (byte)config.radioConfig.GetRadioName();

            return InternalReConfigure(MarshalBuffer, (byte)macName);
        }

        /// <summary>Set the liveness delay</summary>
        /// <param name="livelinessDelay"></param>
        /// <returns>Result of setting this parameter</returns>
        [Obsolete("Use SetNeighborLivenessDelay instead")]
        public DeviceStatus SetNeighbourLivelinessDelay(UInt32 livelinessDelay)
        {
            MacConfig.NeighbourLivelinesDelay= livelinessDelay;

            return ReConfigure(MacConfig, this.macname);
        }

        /// <summary>Set the liveness delay</summary>
        /// <param name="livenessDelay"></param>
        /// <returns>Result of setting this parameter</returns>
        public DeviceStatus SetNeighborLivenessDelay(UInt32 livenessDelay) {
            MacConfig.NeighborLivenessDelay = livenessDelay;

            return ReConfigure(MacConfig, this.macname);
        }

        /// <summary>
        /// Get the current liveliness delay parameter
        /// </summary>
        /// <returns></returns>
        [Obsolete("Use GetNeighborLivenessDelay instead")]
        public UInt32 GetNeighbourLivelinessDelay()
        {
            return MacConfig.NeighbourLivelinesDelay;
        }

        /// <summary>
        /// Get the current liveness delay parameter
        /// </summary>
        /// <returns></returns>
        public UInt32 GetNeighborLivenessDelay() {
            return MacConfig.NeighborLivenessDelay;
        }


        /// <summary>Enable or disable Clear Channel Assessment</summary>
        /// <param name="CCA">True to enable, false to disable</param>
        /// <returns>Status of operation</returns>
        public DeviceStatus SetCCA(bool CCA)
        {
            MacConfig.CCA = CCA;

            return ReConfigure(MacConfig, this.macname);

        }

        /// <summary>Set the number of times to try to send a packet</summary>
        /// <param name="NumberOfRetries">Number of times to try to send</param>
        /// <returns>DeviceStatus</returns>
        public DeviceStatus SetNumberOfRetries(byte NumberOfRetries)
        {
            MacConfig.NumberOfRetries = NumberOfRetries;

            return ReConfigure(MacConfig, this.macname);
        }

        /// <summary>Set the time to do Clear Channel Assessment</summary>
        /// <param name="CCASenseTime">Time to do Clear Channel Assessment (microseconds)</param>
        /// <returns>Status of operation</returns>
        public DeviceStatus SetCCASenseTime(byte CCASenseTime)
        {
            MacConfig.CCASenseTime = CCASenseTime;

            return ReConfigure(MacConfig, this.macname);
        }

        /// <summary>Set the size of the buffer</summary>
        /// <param name="BufferSize">Size of the buffer</param>
        /// <returns>Operation status</returns>
        public DeviceStatus SetBufferSize(byte BufferSize)
        {
            MacConfig.BufferSize = BufferSize;

            return ReConfigure(MacConfig, this.macname);
        }

        /// <summary>
        /// Set Radio ID
        /// </summary>
        /// <param name="RadioID">Radio ID</param>
        /// <returns>DeviceStatus</returns>
        public DeviceStatus SetRadioID(byte RadioID)
        {
            MacConfig.RadioID = RadioID;

            return ReConfigure(MacConfig, this.macname);
        }

        /// <summary>Get Clear Channel Assessment</summary>
        /// <returns>True iff channel is clear</returns>
        public bool GetCCA()
        {
            return MacConfig.CCA;
        }

        /// <summary>Get number of times to retry sending</summary>
        /// <returns>Number of times to retry sending</returns>
        public byte GetNumberOfRetries()
        {
            return MacConfig.NumberOfRetries;
        }

        /// <summary>Get Clear Channel Assessment Sense Time</summary>
        /// <returns>Time to sense CCA (microseconds)</returns>
        public byte GetCCASenseTime()
        {
            return MacConfig.CCASenseTime;
        }

        /// <summary>Get the Radio ID</summary>
        /// <returns>Radio ID</returns>
        public byte GetRadioID()
        {
            return MacConfig.RadioID;
        }


        /// <summary>
        /// Initialize native MAC, radio and interop drivers.
        /// </summary>
        /// <param name="macname">MAC configuration.</param>
        /// <param name="marshalBuffer">Byte array for received packets.</param>
        /// <returns>The driver status after initialization: Success, Fail, Ready, Busy</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus InternalInitialize(byte[] marshalBuffer, byte macname);  // Changed to private by Bill Leal 2/6/2013 per Mukundan Sridharan.


        /// <summary>Remove a packet from the buffer</summary>
        /// <param name="msg">The removed packed</param>
        /// <returns>Status of operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus RemovePacket(byte[] pkt);

        /// <summary>Get a count of unprocessed packets in the buffer</summary>
        /// <returns>The number of packets in the buffer not yet delivered to the program</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetPendingPacketCount();

        /// <summary>Get the buffer size</summary>
        /// <returns>The size of the buffer.</returns>
        public byte GetBufferSize()
        {
            return MacConfig.BufferSize;
        }

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

		[MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus GetNeighborListInternal(UInt16[] neighborlist);


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

		[MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus GetNeighborInternal(UInt16 macAddress, byte[] byte_nbr);

        /// <summary>Get the ID of this CSMA instance</summary>
        /// <returns>The ID of the instance</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetID();

        /// <summary>
        /// Set the address of the device.
        /// </summary>
        /// <param name="address">Address of the device</param>
        /// <remarks>This is the address by which the device is known to the rest of the world. A return value of false can occur if another layer locks the address and prevents changes.</remarks>
        /// <returns>Success / failure</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetAddress(UInt16 address);

        /// <summary>Get the address of the device</summary>
        /// <remarks>This is the address by which the device is known to the rest of the world</remarks>
        /// <returns>Address of the device</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt16 GetAddress();

        /// <summary>Send a packet</summary>
        /// <param name="address">The address of the receiver. Use <code>Addresses.BROADCAST</code> for broadcast</param>
        /// <param name="packet">Byte array containing the packet to be sent</param>
        /// <param name="offset">The first byte in the array to send. Normally 0</param>
        /// <param name="size">The number of bytes to send</param>
        /// <returns>Status of operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus Send(UInt16 address, byte[] packet, UInt16 offset, UInt16 size);

        /// <summary>Send a time stamped packet. Time stamping is ddone in the send in native code</summary>
        /// <param name="Address">The address of reciever</param>
        /// <param name="packet">Packet to send</param>
        /// <param name="offset">offset if any in the byte array</param>
        /// <param name="size">size of the packet</param>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus SendTimeStamped(UInt16 Address, byte[] packet, UInt16 offset, UInt16 size);

        /// <summary>Send a time stamped packet. Time stamp is specified in call</summary>
        /// <param name="Address">Address of reciever</param>
        /// <param name="packet">Packet to send</param>
        /// <param name="offset">offset if any in the byte array</param>
        /// <param name="size">size of the packet</param>
        /// <param name="eventTime">Time to use for timestamp</param>
        /// <returns>Status of operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus SendTimeStamped(UInt16 Address, byte[] packet, UInt16 offset, UInt16 size, UInt32 eventTime);


        /// <summary>Uninitialize radio</summary>
        /// <returns>Status of operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus UnInitialize();

		/// <summary>Configure the MAC object. Must be called before a call to get instance</summary>
		/// <param name="config">MAC configuration to use</param>
		/// <param name="receiveCallback">Method to call when packet received</param>
		/// <param name="neighborChangeCallback">Method to call when neighborhood changed</param>
		/// <returns>Status of operation</returns>
		public static DeviceStatus Configure(MacConfiguration config, ReceiveCallBack receiveCallback, NeighborhoodChangeCallBack neighborChangeCallback) {
			if (MacConfig == null) {
				MacConfig = new MacConfiguration(config);
				Callbacks.SetReceiveCallback(receiveCallback);
				Callbacks.SetNeighborChangeCallback(neighborChangeCallback);

				// Configure the radio 
				if (Radio.Radio_802_15_4.Configure(MacConfig.radioConfig) == DeviceStatus.Busy)
					return DeviceStatus.Busy;
			}
			else {
				// Return busy if the mac configuration is busy
				return DeviceStatus.Busy;
			}
			return DeviceStatus.Success;
		}
	}
}
