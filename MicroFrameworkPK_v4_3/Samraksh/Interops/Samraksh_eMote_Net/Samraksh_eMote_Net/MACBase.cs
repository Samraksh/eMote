using System;
using System.Collections;
using Samraksh.eMote.Net.MAC;
using Samraksh.eMote.Net.Radio;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Samraksh.eMote.Net
{
	/// <summary>Kinds of protocol</summary>
	public enum MACType
	{
        /// <summary>
        /// 
        /// </summary>
        NONE = 0,
		/// <summary>
		/// 
		/// </summary>
		CSMA = 1,
		/// <summary>
		/// 
		/// </summary>
		OMAC = 2,
	}

	/// <summary>
	/// Class to register multiple callbacks for CSHARP payload types.
	/// </summary>
	public class MACPipe : IMAC
	{
		/// <summary>The MACBase instance this pipe subscribes to</summary>
		public readonly MACBase MACBase;

		/// <summary>
		/// The subscribed payload type for the this pipe
		/// </summary>
		public readonly PayloadType PayloadType;

		/// <summary>Raised when a packet has been received</summary>
		public event MACBase.IMACReceiveEventHandler OnReceive;

		/// <summary>Raised when neighborhood changes</summary>
        public event MACBase.IMACNeighborChangeEventHandler OnNeighborChange
		{
			add { MACBase.OnNeighborChange += value; }
			remove { MACBase.OnNeighborChange -= value; }
		}

        /// <summary>Raised when we get an ACK for a transmitted packet</summary>
        public event MACBase.IMACTransmitACKEventHandler OnSendStatus
        {
            add { MACBase.OnSendStatus += value; }
            remove { MACBase.OnSendStatus -= value; }
        }

		/// <summary>
		/// Register callback functions for CSHARP payload types. 
		/// This is used to register multiple callbacks for different payload types
		/// and for different MAC objects.
		/// </summary>
		/// <param name="macBase"></param>
		/// <param name="payloadType"></param>
		public MACPipe(MACBase macBase, PayloadType payloadType)
		{
			MACBase = macBase;
			PayloadType = payloadType;
            /* This might be needed later. 
             *      
             * if (OnReceive == null)
            {
                throw new ArgumentNullException("OnReceive event cannot be null");
            }*/
			MACBase.RegisterOnReceiveGlobal(this);
		}

        internal void MACPipeCallback(PayloadType payloadTypeTemp, DateTime dateTime, Packet receivedPacket)
		{
            if (OnReceive != null)
            {
                OnReceive(this, dateTime, receivedPacket);
            }
            /* No need to remove a packet as we download the packet first (which removes it from the receive queue)
             *      and then decide if it needs to be passed onto the callback function.
             * //If OnReceive event is not registered, just drop the packet.
            else
            {
                MACBase.RemovePacket();
            }*/
		}

		///// <summary>
		///// 
		///// </summary>
		///// <returns></returns>
		//public DeviceStatus UnInitialize()
		//{
		//	return this.MACBase.UnInitialize();
		//}

		/// <summary>Send packet</summary>
		/// <param name="address">Address of recipient</param>
		/// <param name="payload">Payload (in byte array) to send</param>
		/// <param name="offset">Offset into array</param>
		/// <param name="size">Size of payload</param>
		/// <returns>Result status</returns>
		public NetOpStatus Send(ushort address, byte[] payload, ushort offset, ushort size)
		{
			return MACBase.Send(address, PayloadType, payload, offset, size);
		}

		/// <summary>Send packet with time value</summary>
		/// <param name="address">Address of recipient</param>
		/// <param name="payload">Payload (in byte array) to send</param>
		/// <param name="offset">Offset into array</param>
		/// <param name="size">Size of packet</param>
		/// <param name="eventTime">Time value to add to packet</param>
		/// <returns>Result status</returns>
		public NetOpStatus Send(ushort address, byte[] payload, ushort offset, ushort size, DateTime eventTime)
		{
			return MACBase.Send(address, PayloadType, payload, offset, size, eventTime);
		}

		/*/// <summary>Send packet with payload type</summary>
		/// <param name="address">Address of recipient</param>
		/// <param name="payloadType">Payload type of packet</param>
		/// <param name="payload">Payload (in byte array) to send</param>
		/// <param name="offset">Offset into array</param>
		/// <param name="size">Size of payload</param>
		/// <returns>Result status</returns>
		public NetOpStatus Send(ushort address, PayloadType payloadType, byte[] payload, ushort offset, ushort size)
		{
			return MACBase.Send(address, payloadType, payload, offset, size);
		}

		/// <summary>Send packet with payload type and time value</summary>
		/// <param name="address">Address of recipient</param>
		/// <param name="payloadType">Payload type of packet</param>
		/// <param name="payload">Payload (in byte array) to send</param>
		/// <param name="offset">Offset into array</param>
		/// <param name="size">Size of payload</param>
		/// <param name="eventTime">Time value to add to packet</param>
		/// <returns>Result status</returns>
		public NetOpStatus Send(ushort address, PayloadType payloadType, byte[] payload, ushort offset, ushort size, DateTime eventTime)
		{
			return MACBase.Send(address, payloadType, payload, offset, size, eventTime);
		}*/

		/// <summary>Size of send buffer</summary>
		public byte BufferSize
		{
			get { return MACBase.BufferSize; }
			set { MACBase.BufferSize = value; }
		}

		/// <summary>True iff MAC CCA (clear channel assessment) is enabled</summary>
		public bool CCA
		{
			get { return MACBase.CCA; }
			set { MACBase.CCA = value; }
		}

		/// <summary>Amount of time (in milliseconds) to assess whether channel is clear (CCA)</summary>
		public byte CCASenseTime
		{
			get { return MACBase.CCASenseTime; }
			set { MACBase.CCASenseTime = value; }
		}

		/// <summary>Delay (in milliseconds) before a Neighbor is deemed dead</summary>
		public uint NeighborLivenessDelay
		{
			get { return MACBase.NeighborLivenessDelay; }
			set { MACBase.NeighborLivenessDelay = value; }
		}

		/// <summary>Number of times to try sending before MAC gives up</summary>
		public byte NumberOfRetries
		{
			get { return MACBase.NumberOfRetries; }
			set { MACBase.NumberOfRetries = value; }
		}

		/// <summary>The radio object the MAC is using</summary>
		public Radio_802_15_4_Base MACRadioObj { get { return MACBase.MACRadioObj; } }

		/// <summary>The type of MAC (OMAC, CSMA)</summary>
		public MACType MACType { get { return MACBase.MACType; } }

		/// <summary>
		///		Get the list of neighbors from the MAC
		/// </summary>
		/// <param name="neighborListArray">
		///		Array is filled with the addresses of active neighbors, padded with zeroes at the end.
		/// </param>
		/// <returns>
		///		Result status
		/// </returns>
		public DeviceStatus NeighborList(ushort[] neighborListArray)
		{
			return MACBase.NeighborList(neighborListArray);
		}

        #region commented code
        
        /*/// <summary>
		/// Get the next packet from the MAC buffer
		/// </summary>
		/// <returns>Next packet if any, else null</returns>
        private bool NextPacket(Packet receivedPacket)
		{
            return MACBase.NextPacket(receivedPacket);
		}*/

		/*
		///// 
		///// </summary>
		///// <returns></returns>
		//public UInt16 GetRadioAddress()
		//{
		//	return this.MACBase.GetRadioAddress();
		//}

		///// <summary>
		///// 
		///// </summary>
		///// <param name="address"></param>
		///// <returns></returns>
		//public DeviceStatus SetRadioAddress(UInt16 address)
		//{
		//	return this.MACBase.SetRadioAddress(address);
		//}*/

		/*
		///// 
		///// </summary>
		///// <returns></returns>
		//public byte GetMACType()
		//{
		//	return this.MACBase.GetMACType();
		//}*/
		#endregion

		/// <summary>
		/// 
		/// </summary>
		/// <param name="macAddress"></param>
		/// <returns></returns>
		public Neighbor NeighborStatus(ushort macAddress)
		{
			return MACBase.NeighborStatus(macAddress);
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		public byte PendingSendPacketCount()
		{
			return MACBase.PendingSendPacketCount();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		public byte PendingReceivePacketCount()
		{
			return MACBase.PendingReceivePacketCount();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		public DeviceStatus RemovePacket()
		{
			return MACBase.RemovePacket();
		}

        /// <summary>
        /// Uninitialize MACPipe instance
        /// </summary>
        public void Dispose()
        {
            MACBase.UnRegisterOnReceiveGlobal(this);
        }

	}



	/// <summary>Base class for wireless protocols</summary>
	/// <seealso cref="MAC.CSMA" cat="Inherited by">CSMA Class</seealso>
	/// <seealso cref="MAC.OMAC" cat="Inherited by">OMAC Class</seealso>
	public class MACBase : NativeEventDispatcher, IMAC
	{

		#region Private variables & properties	-------------------------------------------------------------------------------
		/// <summary>
		/// Specifies the marshalling buffer size, used by the config to pass data to native code 
		/// </summary>
		private const byte MarshalBufferSize = 9;
		private const byte MACPacketSize = 126;
		/// <summary>Specifies the Neighbor size</summary>
		private const byte NeighborSize = 22; //Look at IMAC.cs to figure out the size of the Neighbor structure.

		/// <summary>Specifies maximum neighbors that can be held in Neighbor table</summary>
		private const int NeighborListSize = 12;

		private readonly ushort[] _neighborListTemp = new ushort[NeighborListSize];
		private readonly byte[] _byteNeighbor = new byte[NeighborSize];
		private readonly byte[] _marshalBuffer = new byte[MarshalBufferSize];
		private static readonly byte[] DataBuffer = new byte[MACPacketSize];

		// ReSharper disable once InconsistentNaming
		private static readonly Neighbor _neighbor = new Neighbor();
		//private static bool _initializeCounter;
		private static Hashtable _macPipeHashtable = new Hashtable();
		private static bool _csmaInstanceSet;
		private static bool _omacInstanceSet;
        
		#endregion Private variables & properties



		#region Public variables & properties	---------------------------------------------------------------------------------

		// Removed by Bill ... use NeighborListArray instead
		//public int NeighborListSize
		//{
		//	get
		//	{
		//		return NeighborListSize;
		//	}
		//}


		/// <summary>The type of MAC (OMAC, CSMA)</summary>
		public MACType MACType { get; private set; }

		/// <summary>The radio object the MAC is using</summary>
		public Radio_802_15_4_Base MACRadioObj { get; private set; }

		// Changed first argument to IMAC instead of MACBase -- Bill
		/// <summary>Event handler for classes implementing IMAC's on receive event</summary>
		/// <param name="macInstance">
		///		When used by MACBase classes (CSMA, OMAC), will be MACBase instance.
		///		When used by MACPipe class, will be MACPipe class.
		/// </param>
		/// <param name="time"></param>
        /// <param name="receivedPacket">Packet object to be filled in with received packet details</param>
		public delegate void IMACReceiveEventHandler(IMAC macInstance, DateTime time, Packet receivedPacket);

        /// <summary>Event handler for classes implementing IMAC's neighbor change event</summary>
        /// <param name="macInstance">
        ///		When used by MACBase classes (CSMA, OMAC), will be MACBase instance.
        ///		When used by MACPipe class, will be MACPipe class.
        /// </param>
        /// <param name="time"></param>
        public delegate void IMACNeighborChangeEventHandler(IMAC macInstance, DateTime time);
        //public delegate void IMACNeighborChangeEventHandler(IMAC macInstance, DateTime time, uint currentNeighborNumber);

		/// <summary>Raised when any packet is received (promiscuous mode)</summary>
        public event IMACReceiveEventHandler OnReceiveAll;

		/// <summary>Raised when a packet has been received</summary>
        public event IMACReceiveEventHandler OnReceive;

		/// <summary>Raised when neighborhood changes</summary>
        public event IMACNeighborChangeEventHandler OnNeighborChange;

        /// <summary>Event handler for classes implementing IMAC's transmit ACK event</summary>
        /// <param name="macInstance">
        ///		When used by MACBase classes (CSMA, OMAC), will be MACBase instance.
        ///		When used by MACPipe class, will be MACPipe class.
        /// </param>
        /// <param name="time"></param>
        public delegate void IMACTransmitACKEventHandler(IMAC macInstance, DateTime time, SendPacketStatus ACKStatus, uint transmitDestination);

        /// <summary>Raised when a transmited packet is ACKed</summary>
        public event IMACTransmitACKEventHandler OnSendStatus;

		/// <summary>True iff MAC CCA (clear channel assessment) is enabled</summary>
		public bool CCA
		{
			get
			{
				return _cca;
			}
			set
			{
				_cca = value;
				Configure();
			}
		}
		private bool _cca;

		/// <summary>Number of times to try sending before MAC gives up</summary>
		public byte NumberOfRetries
		{
			get
			{
				return _numberOfRetries;
			}
			set
			{
				_numberOfRetries = value;
				Configure();
			}
		}
		private byte _numberOfRetries;

		/// <summary>Amount of time (in milliseconds) to assess whether channel is clear (CCA)</summary>
		public byte CCASenseTime
		{
			get
			{
				return _ccaSenseTime;
			}
			set
			{
				_ccaSenseTime = value;
				Configure();
			}
		}
		private byte _ccaSenseTime;

		/// <summary>Size of send buffer</summary>
		public byte BufferSize
		{
			get
			{
				return _bufferSize;
			}
			set
			{
				_bufferSize = value;
				Configure();
			}
		}
		private byte _bufferSize;

		/// <summary>Delay (in milliseconds) before a Neighbor is deemed dead</summary>
		public uint NeighborLivenessDelay
		{
			get
			{
				return _neighborLivenessDelay;
			}
			set
			{
				_neighborLivenessDelay = value;
				Configure();
			}
		}
		private uint _neighborLivenessDelay;

		#endregion Public variables & properties

		/// <summary>Constructor to create MAC object</summary>
		/// <param name="macType">CSMA, OMAC or other MAC</param>
		/// <param name="cca"></param>
		/// <param name="radioConfiguration"></param>
		/// <param name="numberOfRetries"></param>
		/// <param name="ccaSenseTime"></param>
		/// <param name="bufferSize"></param>
		/// <param name="neighborLivenessDelay"></param>
		/// <exception caption="MacNotConfigured Exception" cref="MACNotConfiguredException"></exception>
		/// <exception caption="System Exception" cref="System.SystemException"></exception>
		public MACBase(MACType macType, IRadioConfiguration radioConfiguration, uint neighborLivenessDelay = 650, byte numberOfRetries = 0, byte bufferSize = 8, bool cca = true,
									byte ccaSenseTime = 120)
			: base("MACCallback", 1234)
		{
			if (radioConfiguration == null)
				throw new RadioNotConfiguredException("Radio not configured");

			//if(MacConfig == null)
			//throw new MACNotConfiguredException("MAC not configured");

			//Initialize MAC object properties
			_cca = cca;
			_numberOfRetries = numberOfRetries;
			_ccaSenseTime = ccaSenseTime;
			_bufferSize = bufferSize;
			_neighborLivenessDelay = neighborLivenessDelay;

			//Initialize MAC and radio config 
			//MACRadioConfig.Channel = channel;
			//MACRadioConfig.RadioType = radioName;
			//MACRadioConfig.TxPower = txPower;

			//Enable interrupt handler for receive callback function 
			OnInterrupt += ReceiveFunction;
			//InitializeMacConfig(CCA, _numberOfRetries, CCASenseTime, _bufferSize, _neighborLivenessDelay, MACRadioConfig);

			//if (MACConfig.MACRadioConfig == null)
			//throw new RadioNotConfiguredException("Radio not configured");

			if (macType == MACType.CSMA)
			{
				if (!_csmaInstanceSet)
				{
					_csmaInstanceSet = true;
					if (radioConfiguration.RadioName == RadioName.RF231)
                        MACRadioObj = new Radio_802_15_4_RF231(RadioUser.CSMA);

					else if (radioConfiguration.RadioName == RadioName.RF231LR)
						MACRadioObj = new Radio_802_15_4_RF231_LR(RadioUser.CSMA);

					else if (radioConfiguration.RadioName == RadioName.SI4468)
						MACRadioObj = new Radio_802_15_4_SI4468(RadioUser.CSMA);

					else
						throw new UnknownRadioTypeException("Unknown radio type");
				}
				else
					throw new Exception("CSMA already configured");
			}
			else if (macType == MACType.OMAC)
			{
				if (!_omacInstanceSet)
				{
					_omacInstanceSet = true;
					if (radioConfiguration.RadioName == RadioName.RF231)
						MACRadioObj = new Radio_802_15_4_RF231(RadioUser.OMAC);

					else if (radioConfiguration.RadioName == RadioName.RF231LR)
						MACRadioObj = new Radio_802_15_4_RF231_LR(RadioUser.OMAC);

					else if (radioConfiguration.RadioName == RadioName.SI4468)
						MACRadioObj = new Radio_802_15_4_SI4468(RadioUser.OMAC);

					else
						throw new UnknownRadioTypeException("Unknown radio type");
				}
				else
					throw new Exception("OMAC already configured");
			}
			else
				throw new MACTypeMismatchException("Unrecognized MAC type");

			//Configure MAC
			MACType = macType;
			var status = Initialize(CCA, NumberOfRetries, CCASenseTime, BufferSize, NeighborLivenessDelay, MACType, radioConfiguration.RadioName);

			//Configure radio (radio has to be configured only after MAC is done, as MAC initializes radio)
			MACRadioObj.RadioName = radioConfiguration.RadioName;
			MACRadioObj.Channel = radioConfiguration.Channel;
			MACRadioObj.TxPower = radioConfiguration.TxPower;

			if (status != DeviceStatus.Success)
			{
				throw new MACNotConfiguredException("MAC initialization failed. One reason for failure could be that a USB cable is attached to the DotNow.\n");
			}
		}

		/// <summary>Native initialization of mac</summary>
		/// <param name="cca"></param>
		/// <param name="numberOfRetries"></param>
		/// <param name="ccaSenseTime"></param>
		/// <param name="bufferSize"></param>
		/// <param name="neighborLivenessDelay"></param>
		/// <param name="mactype"></param>
		/// <param name="radioName"></param>
		/// <returns></returns>
		private DeviceStatus Initialize(bool cca, byte numberOfRetries, byte ccaSenseTime, byte bufferSize, uint neighborLivenessDelay, MACType mactype, RadioName radioName)
		{
			if (cca)
				_marshalBuffer[0] = 1;
			else
				_marshalBuffer[0] = 0;

			_marshalBuffer[1] = numberOfRetries;
			_marshalBuffer[2] = ccaSenseTime;
			_marshalBuffer[3] = bufferSize;
			//_marshalBuffer[4] = (byte)config.RadioType;
			_marshalBuffer[4] = (byte)(neighborLivenessDelay & 0xff);
			_marshalBuffer[5] = (byte)((neighborLivenessDelay & 0xff00) >> 8);
			_marshalBuffer[6] = (byte)((neighborLivenessDelay & 0xff0000) >> 16);
			_marshalBuffer[7] = (byte)((neighborLivenessDelay & 0xff000000) >> 24);
			// Breaking the object boundary, but shallow instances of the radio can not initialize
			/*_marshalBuffer[9] = (byte)radioConfiguration.TxPower;
			_marshalBuffer[10] = (byte)radioConfiguration.Channel;*/
			_marshalBuffer[8] = (byte)radioName;

			return InternalInitialize(_marshalBuffer, (byte)mactype);
		}

		/// <summary>Reconfigure MAC</summary>
		// ReSharper disable once UnusedMethodReturnValue.Local
		private DeviceStatus Configure()
		{
			/*if (MACConfig == null)
			{
				throw new MACNotConfiguredException("MAC not configured");
			}*/

			if (MACType != MACType.CSMA && MACType != MACType.OMAC)
			{
				throw new MACNotConfiguredException("MAC type not known");
			}

			if (CCA)
				_marshalBuffer[0] = 1;
			else
				_marshalBuffer[0] = 0;

			_marshalBuffer[1] = NumberOfRetries;
			_marshalBuffer[2] = CCASenseTime;
			_marshalBuffer[3] = BufferSize;
			//_marshalBuffer[4] = (byte)MACConfig.RadioType;
			_marshalBuffer[4] = (byte)(NeighborLivenessDelay & 0xff);
			_marshalBuffer[5] = (byte)((NeighborLivenessDelay & 0xff00) >> 8);
			_marshalBuffer[6] = (byte)((NeighborLivenessDelay & 0xff0000) >> 16);
			_marshalBuffer[7] = (byte)((NeighborLivenessDelay & 0xff000000) >> 24);

			return InternalReConfigure(_marshalBuffer);
		}

		/// <summary>
		/// Invoked when payload type is data.
		/// </summary>
		/// <param name="data1"></param>
		/// <param name="data2"></param>
		/// <param name="time"></param>
		private void ReceiveFunction(uint data1, uint data2, DateTime time)
		{
			/*if (MACConfig.MACRadioConfig.OnReceiveCallback == null)
				throw new CallbackNotConfiguredException();*/

			/*if (MACConfig.MACRadioConfig.OnNeighborChangeCallback == null)
				throw new CallbackNotConfiguredException();*/

            switch (data1)
			{
                case (uint)CallbackType.Received:
                {
                    //Download the packet and then decide if it needs to be 
                    //  passed onto the corresponding callback function.
                    Packet receivedPacket = null;
                    receivedPacket = NextPacket();
                    if (receivedPacket == null)
                        return;

                    // OnReceiveAll is raised for every payload type
                    if (OnReceiveAll != null)
                    {
                        OnReceiveAll(this, time, receivedPacket);
                    }

                    // OnReceive is raised for MFM_Data payload type
                    if (data2 == (uint)PayloadType.MFM_Data)
                    {
                        if (OnReceive != null)
                        {
                            OnReceive(this, time, receivedPacket);
                        }
                    }

                    // Otherwise raise the OnReceive for a registered MACPipe, if any
                    else
                    {
                        var keyCollection = _macPipeHashtable.Keys;
                        //Get a count of total registered payload types
                        //int eventCounter = keyCollection.Count;
                        foreach (PayloadType payloadTypeKey in keyCollection)
                        {
                            if (data2 == (uint)payloadTypeKey)
                            {
                                var macPipe = (MACPipe)_macPipeHashtable[payloadTypeKey];
                                //If a registered payload type is received, subtract count
                                //eventCounter--;
                                macPipe.MACPipeCallback(payloadTypeKey, time, receivedPacket);
                                break;
                            }
                            else
                            {
                                continue;
                            }
                        }
                        /* No need to remove a packet as we download the packet first (which removes it from the receive queue)
                         *      and then decide if it needs to be passed onto the callback function.
                         *      
                         * //If a packet is received for which the payload type has not been registered, drop the packet.
                        if (eventCounter == keyCollection.Count)
                        {
                            RemovePacket();
                        }*/
                    }
                    break;
                }
                case (uint)CallbackType.NeighborChanged:
                {
                    if (OnNeighborChange != null)
                    {
                        //OnNeighborChange(this, time, data2);
                        OnNeighborChange(this, time);
                    }
                    break;
                }
                case (uint)CallbackType.SendInitiated:
                {
                    if (OnSendStatus != null)
                    {
                        OnSendStatus(this, time, SendPacketStatus.SendInitiated, data2);
                    }
                    break;
                }
                case (uint)CallbackType.SendACKed:
                {
                    if (OnSendStatus != null)
                    {
                        OnSendStatus(this, time, SendPacketStatus.SendACKed, data2);
                    }
                    break;
                }
                case (uint)CallbackType.SendNACKed:
                {
                    if (OnSendStatus != null)
                    {
                        OnSendStatus(this, time, SendPacketStatus.SendNACKed, data2);
                    }
                    break;
                }
                case (uint)CallbackType.SendFailed:
                {
                    if (OnSendStatus != null)
                    {
                        OnSendStatus(this, time, SendPacketStatus.SendFailed, data2);
                    }
                    break;
                }
                default:
                {
                    throw new MACNotConfiguredException("(internal error) Unrecognized CallBackType " + data1);
                }
			}
		}

		/// <summary>
		/// Registers callbacks for payload types.
		/// </summary>
		/// <param name="macPipe"></param>
		internal static void RegisterOnReceiveGlobal(MACPipe macPipe)
		{
            if (!_macPipeHashtable.Contains(macPipe.PayloadType))
			{
				_macPipeHashtable[macPipe.PayloadType] = macPipe;
			}
			else
			{
				throw new Exception("Callback for payloadtype already registered");
			}
		}

        /// <summary>
        /// Unregisters callbacks for payload types.
        /// </summary>
        /// <param name="macPipe"></param>
        internal static void UnRegisterOnReceiveGlobal(MACPipe macPipe)
        {
            if (_macPipeHashtable.Contains(macPipe.PayloadType))
            {
                _macPipeHashtable.Remove(macPipe.PayloadType);
            }
            else
            {
                throw new Exception("Callback for payloadtype not registered");
            }
        }

        /// <summary>
        /// Get the next packet from the MAC buffer
        /// </summary>
        /// <returns>True if success, false otherwise</returns>
        internal Packet NextPacket()
		{
			for (ushort i = 0; i < MACPacketSize; i++)
				DataBuffer[i] = 0;

            if (PendingReceivePacketCount() == 0)
                return null;

			if (GetNextPacket(DataBuffer) != DeviceStatus.Success)
                return null;

			if (DataBuffer[0] == 0)
                return null;

            Packet receivedPacket = new Packet(DataBuffer);

            return receivedPacket;
		}

		#region commented code
		/*/// <summary>Set the liveness delay</summary>
		///// <param name="livenessDelay"></param>
		///// <returns>Result of setting this parameter</returns>
		//public DeviceStatus SetNeighborLivenessDelay(UInt32 livenessDelay) {
		//	MACConfig.NeighborLivenessDelay = livenessDelay;
		//	return Configure();
		//}

		///// <summary>Enable or disable Clear Channel Assessment</summary>
		///// <param name="CCA">True to enable, false to disable</param>
		///// <returns>Status of operation</returns>
		//public DeviceStatus SetCCA(bool CCA)
		//{
		//	MACConfig.CCA = CCA;
		//	return Configure();
		//}

		///// <summary>Set the count of send retries</summary>
		///// <param name="NumberOfRetries">Send retry count</param>
		///// <returns>DeviceStatus</returns>
		//public DeviceStatus SetNumberOfRetries(byte NumberOfRetries)
		//{
		//	MACConfig.NumberOfRetries = NumberOfRetries;
		//	return Configure();
		//}

		///// <summary>Set the time to do Clear Channel Assessment</summary>
		///// <param name="CCASenseTime">Time to do Clear Channel Assessment (microseconds)</param>
		///// <returns>Status of operation</returns>
		//public DeviceStatus SetCCASenseTime(byte CCASenseTime)
		//{
		//	MACConfig.CCASenseTime = CCASenseTime;
		//	return Configure();
		//}

		///// <summary>Set the size of the buffer</summary>
		///// <param name="BufferSize">Size of the buffer</param>
		///// <returns>Operation status</returns>
		//public DeviceStatus SetBufferSize(byte BufferSize)
		//{
		//	MACConfig.BufferSize = BufferSize;
		//	return Configure();
		//}

		///// <summary>Set Radio type</summary>
		///// <param name="RadioType">Radio type</param>
		///// <returns>DeviceStatus</returns>
		//public DeviceStatus SetRadioType(RadioType RadioType)
		//{
		//	MACConfig.MACRadioConfig.RadioType = RadioType;
		//	return SetRadioType((byte)RadioType);
		//}

		///// <summary></summary>
		///// <param name="txPowerValue"></param>
		///// <returns></returns>
		//public DeviceStatus SetTxPower(TxPowerValue txPowerValue)
		//{
		//	MACConfig.MACRadioConfig.TxPower = txPowerValue;
		//	return SetTxPower((byte)MACConfig.MACRadioConfig.RadioType, (byte)txPowerValue);
		//}

		///// <summary></summary>
		///// <param name="channel"></param>
		///// <returns></returns>
		//public DeviceStatus SetChannel(Channel channel)
		//{
		//	MACConfig.MACRadioConfig.Channel = channel;
		//	return SetChannel((byte)MACConfig.MACRadioConfig.RadioType, (byte)channel);
		//}*/

		/*/// <summary></summary>
		///// <param name="radioName"></param>
		///// <returns></returns>
		//[MethodImpl(MethodImplOptions.InternalCall)]
		//private extern DeviceStatus SetRadioType(byte radioName);

		///// <summary>Set the transmit power of the radio</summary>
		///// <param name="radioName">Radio name</param>
		///// <param name="TxPower">Transmission power to use</param>
		///// <returns>Status of operation</returns>
		//[MethodImpl(MethodImplOptions.InternalCall)]
		//private extern DeviceStatus SetTxPower(byte radioName, int TxPower);

		///// <summary>Set the radio channel</summary>
		///// <param name="radioName">Radio name</param>
		///// <param name="Channel">Channel to use</param>
		///// <returns>Status of operation</returns>
		//[MethodImpl(MethodImplOptions.InternalCall)]
		//private extern DeviceStatus SetChannel(byte radioName, int Channel);*/
		#endregion commented code

		// Added by Bill. 

		/// <summary>
		/// Create a NeighborList array for the user
		/// </summary>
		/// <remarks>
		/// The NeighborList array must be exactly NeighborListSize. 
		/// To relieve the user of having to know the size, this method can be used to create one.
		/// </remarks>
		/// <returns></returns>
		public static ushort[] NeighborListArray()
		{
			return new ushort[NeighborListSize];
		}

		/// <summary>
		///		Get the list of neighbors from the MAC
		/// </summary>
		/// <param name="neighborListArray">
		///		Array is filled with the addresses of active neighbors, padded with zeroes at the end.
		/// </param>
		/// <returns>
		///		Result status
		/// </returns>
		public DeviceStatus NeighborList(ushort[] neighborListArray)
		{
			if (neighborListArray.Length == 0)
			{
				throw new Exception("Array size is 0");
			}

			if (neighborListArray.Length > NeighborListSize)
			{
				throw new Exception("Array size cannot be greater than " + NeighborListSize);
			}

			var status = NeighborList();

			Array.Copy(_neighborListTemp, neighborListArray, _neighborListTemp.Length);

			return status;
		}

		private DeviceStatus NeighborList()
		{
			if (GetNeighborListInternal(_neighborListTemp) != DeviceStatus.Success)
			{
				//throw new Exception("Could not get list of neighbors");
				return DeviceStatus.Fail;
			}

			return DeviceStatus.Success;
		}

		/// <summary>Get the details for a Neighbor</summary>
		/// <param name="macAddress">Address of the Neighbor</param>
		/// <returns>Neighbor</returns>
		public Neighbor NeighborStatus(ushort macAddress)
		{
			if (GetNeighborInternal(macAddress, _byteNeighbor) == DeviceStatus.Success)
			{
				_neighbor.MACAddress = (ushort)(((ushort)(_byteNeighbor[1] << 8) & 0xFF00) + _byteNeighbor[0]);//MacAddress
				_neighbor.SendLink.AverageRSSI = _byteNeighbor[2]; //ForwardLink
                _neighbor.SendLink.LinkQuality = _byteNeighbor[3];
                _neighbor.SendLink.AverageDelay = _byteNeighbor[4];
                _neighbor.ReceiveLink.AverageRSSI = _byteNeighbor[5];  //ReverseLink
                _neighbor.ReceiveLink.LinkQuality = _byteNeighbor[6];
                _neighbor.ReceiveLink.AverageDelay = _byteNeighbor[7];
				_neighbor.NeighborStatus = (NeighborStatus)_byteNeighbor[8];//Status of neighbor
                _neighbor.CountOfPacketsReceived = (ushort)(((_byteNeighbor[10] << 8) & 0xFF00) + _byteNeighbor[9]);
				_neighbor.LastHeardTime = (ulong)((_byteNeighbor[18] << 56) + _byteNeighbor[17] << 48 + _byteNeighbor[16] << 40 + _byteNeighbor[15] << 32 + _byteNeighbor[14] << 24 +
				_byteNeighbor[13] << 16 + _byteNeighbor[12] << 8 + +_byteNeighbor[11]);//LastTimeHeard
				_neighbor.ReceiveDutyCycle = _byteNeighbor[19];//ReceiveDutyCycle
				_neighbor.FrameLength = (ushort)(((_byteNeighbor[21] << 8) & 0xFF00) + _byteNeighbor[20]);
				return _neighbor;
			}

			return null;
		}

		#region Public Externs	---------------------------------------------------------------------------------------
		/// <summary>Remove a _packet from the buffer</summary>
		/// <returns>Status of operation</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern DeviceStatus RemovePacket();

		/// <summary>Get a count of unprocessed packets in the buffer</summary>
		/// <returns>The number of packets in the buffer not yet delivered to the program</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern byte PendingReceivePacketCount();

		/// <summary>Get a count of unprocessed packets in the buffer</summary>
		/// <returns>The number of packets in the buffer not yet delivered to the program</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern byte PendingSendPacketCount();
		#endregion Public Externs


		#region Private Externs ------------------------------------------------------------------------------------------
		/// <summary>Uninitialize radio</summary>
		/// <returns>Status of operation</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern DeviceStatus UnInitialize();

		/// <summary>Initialize native MAC, radio and interop drivers</summary>
		/// <param name="marshalBuffer"></param>
		/// <param name="mactype"></param>
		/// <returns>The driver status after initialization: Success, Fail, Ready, Busy</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern DeviceStatus InternalInitialize(byte[] marshalBuffer, byte mactype);  // Changed to private by Bill Leal 2/6/2013 per Mukundan Sridharan.

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

		/// <summary></summary>
		/// <param name="macAddress"></param>
		/// <param name="neighborlist"></param>
		/// <returns></returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern DeviceStatus GetNeighborInternal(ushort macAddress, byte[] neighborlist);

		/// <summary></summary>
		/// <param name="neighborlist"></param>
		/// <returns></returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern DeviceStatus GetNeighborListInternal(ushort[] neighborlist);

		/*/// <summary>Get the type of this MAC instance</summary>
		/// <returns></returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern byte GetMACType();*/

		/*/// <summary>Set the address of the device</summary>
		/// <param name="address">Address of the device</param>
		/// <remarks>This is the address by which the device is known to the rest of the world. A return value of false can occur if another layer locks the address and prevents changes.</remarks>
		/// <returns>Success / failure</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern DeviceStatus SetRadioAddress(UInt16 address);

		/// <summary>Get the address of the device</summary>
		/// <remarks>This is the address by which the device is known to the rest of the world</remarks>
		/// <returns>Address of the device</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern UInt16 GetRadioAddress();*/
		#endregion Private Externs


		/// <summary>Send packet</summary>
		/// <param name="address">Address of recipient</param>
		/// <param name="payload">Payload (in byte array) to send</param>
		/// <param name="offset">Offset into array</param>
		/// <param name="size">Size of payload</param>
		/// <returns>Result status</returns>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern NetOpStatus Send(ushort address, byte[] payload, ushort offset, ushort size);

		/// <summary>Send a _packet</summary>
		/// <param name="address">The address of the receiver. Use <code>Addresses.BROADCAST</code> for broadcast</param>
		/// <param name="payload">Byte array containing the payload to be sent</param>
		/// <param name="offset">The first byte in the array to send. Normally 0</param>
		/// <param name="size">The number of bytes to send</param>
		/// <param name="eventTime">Time at which event occurred</param>
		/// <returns>Status of operation</returns>
		public NetOpStatus Send(ushort address, byte[] payload, ushort offset, ushort size, DateTime eventTime)
		{
			return Send(address, payload, offset, size, (uint)(eventTime.Ticks & uint.MaxValue));
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern NetOpStatus Send(ushort address, byte[] payload, ushort offset, ushort size, uint eventTime);

		/// <summary>Send a _packet</summary>
		/// <param name="address">The address of the receiver. Use <code>Addresses.BROADCAST</code> for broadcast</param>
		/// <param name="payloadType">PayloadType to be sent</param>
		/// <param name="payload">Byte array containing the payload to be sent</param>
		/// <param name="offset">The first byte in the array to send. Normally 0</param>
		/// <param name="size">The number of bytes to send</param>
		/// <returns>Status of operation</returns>
		public NetOpStatus Send(ushort address, PayloadType payloadType, byte[] payload, ushort offset, ushort size)
		{
			return Send(address, (byte)payloadType, payload, offset, size);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern NetOpStatus Send(ushort address, byte payloadType, byte[] payload, ushort offset, ushort size);

		/// <summary>Send a time stamped _packet. Time stamp is specified as a parameter</summary>
		/// <param name="address">Address of receiver</param>
		/// <param name="payloadType">PayloadType to be sent</param>
		/// <param name="payload">payload to be sent</param>
		/// <param name="offset">offset if any in the byte array</param>
		/// <param name="size">size of the _packet</param>
		/// <param name="eventTime">Time to use for timestamp</param>
		/// <returns>Status of operation</returns>
		public NetOpStatus Send(ushort address, PayloadType payloadType, byte[] payload, ushort offset, ushort size, DateTime eventTime)
		{
			return Send(address, (byte)payloadType, payload, offset, size, (uint)(eventTime.Ticks & uint.MaxValue));
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		private extern NetOpStatus Send(ushort address, byte payloadType, byte[] payload, ushort offset, ushort size, uint eventTime);

		/// <summary>Uninitialize MAC instance</summary>
		public override void Dispose()
		{
            DeviceStatus status = DeviceStatus.Fail;
            //Radio is uninitialized when MAC is uninitialized
            //Uninitialize radio
            /*status = MACRadioObj.UnInitialize((byte)MACRadioObj.RadioName);
            if (status != DeviceStatus.Success)
            {
                throw new Exception("Could not uninitialize radio");
            }*/

            //Uninitialize radio properties
            switch (MACRadioObj.RadioName)
            {
                case RadioName.RF231:
                    ((Radio_802_15_4_RF231)MACRadioObj).Radio_802_15_4_RF231_UnInitialize();
                    break;
                case RadioName.RF231LR:
                    ((Radio_802_15_4_RF231_LR)MACRadioObj).Radio_802_15_4_RF231_LR_UnInitialize();
                    break;
                case RadioName.SI4468:
                    ((Radio_802_15_4_SI4468)MACRadioObj).Radio_802_15_4_SI4468_UnInitialize();
                    break;
                default:
                    throw new RadioNotConfiguredException("Radio name unknown");
            }
            //Uninitialize MAC
            status = UnInitialize();
            if (status != DeviceStatus.Success)
            {
                string macTypeStr = "";
                if (MACType == MACType.CSMA)
                    macTypeStr = "CSMA";
                else if(MACType == MACType.OMAC)
                    macTypeStr = "OMAC";

                throw new Exception("Could not uninitialize " + macTypeStr);
            }
			switch (MACType)
			{
				case MACType.CSMA:
					_csmaInstanceSet = false;
					break;
				case MACType.OMAC:
					_omacInstanceSet = false;
					break;
				default:
					throw new ArgumentOutOfRangeException(MACType.ToString(), "Invalid MAC type");
			}
			base.Dispose();
		}
	}
}
