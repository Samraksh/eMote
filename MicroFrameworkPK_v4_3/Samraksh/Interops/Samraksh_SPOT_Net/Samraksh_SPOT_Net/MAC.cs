using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net.Mac;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Samraksh.SPOT.Net
{
    // Making this private as the user should really never be seeing this 
    public class MACBase : IMac
    {
        /// <summary>
        /// Specifies the marshalling buffer size, used by the config to pass data to native code 
        /// </summary>
        const byte MarshalBufferSize = 7;

        const byte MacMessageSize = 128;
        /// <summary>
        /// Specifies the neighbour size
        /// </summary>
        const byte NeighborSize = 22; //Look at IMac.cs to figure out the size of the Neighbor structure.

        byte[] ByteNeighbor = new byte[NeighborSize];

        byte[] MarshalBuffer = new byte[MarshalBufferSize];

        public static MacConfiguration macconfig;

        public static ReceiveCallBack receiveCallback;

        private Message message;

        byte[] dataBuffer = new byte[MacMessageSize];
        
        Radio.Radio_802_15_4 radioObj;

        /// <summary>
        /// Constructor to initialize callbacks 
        /// </summary>
        /// <param name="drvName">Name of the callback</param>
        /// <param name="drvData">Driver data</param>
        public MACBase(string macname)
        {

            if (macconfig == null || receiveCallback == null)
                throw new MacNotConfiguredException();

            // Configure the radio 
            Radio.Radio_802_15_4.Configure(macconfig.radioConfig, receiveCallback);

            if (macname == "CSMA")
            {
                radioObj = Radio.Radio_802_15_4.GetShallowInstance(Radio.RadioUser.CSMAMAC);
            }
            else if (macname == "OMAC")
            {
                radioObj = Radio.Radio_802_15_4.GetShallowInstance(Radio.RadioUser.OMAC);
            }

            Initialize(macconfig);

        }

        /// <summary>
        /// Releases the memory held by the packet to Garbage collector, make this call after assigning the acquired packet to a packet reference 
        /// </summary>
        public void ReleasePacketToGC()
        {
            message = null;
        }

        /// <summary>
        /// Get the next packet from the mac buffer
        /// </summary>
        /// <returns>Message Type</returns>
        public Message GetNextPacket()
        {
            if (GetNextPacket(dataBuffer) != DeviceStatus.Success)
                return null;

            message = new Message(dataBuffer);

            return message;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus GetNextPacket(byte[] nativeBuffer);

        /// <summary>
        /// Returns the radio being used by the mac
        /// </summary>
        /// <returns>Radio_802_15_4 object</returns>
        public Radio.Radio_802_15_4 GetRadio()
        {
            return radioObj;
        }

        /// <summary>
        /// Initialize the mac after marshalling the config
        /// </summary>
        /// <param name="config"></param>
        /// <returns></returns>
        private DeviceStatus Initialize(MacConfiguration config)
        {
            if (config.CCA)
                MarshalBuffer[0] = 1;
            else
                MarshalBuffer[0] = 0;

            MarshalBuffer[1] = config.NumberOfRetries;
            MarshalBuffer[2] = config.CCASenseTime;
            MarshalBuffer[3] = config.BufferSize;
            MarshalBuffer[4] = config.RadioID;
            // Breaking the object boundary, but shallow instances of the radio can not initialize
            MarshalBuffer[5] = (byte) config.radioConfig.TxPower;
            MarshalBuffer[6] = (byte) config.radioConfig.Channel;

            return InternalInitialize(MarshalBuffer);
        }

        /// <summary>
        /// Set the CCA
        /// </summary>
        /// <param name="CCA"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetCCA(bool CCA);

        /// <summary>
        /// Set Number of retries of the MAC
        /// </summary>
        /// <param name="NumberOfRetries"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetNumberOfRetries(byte NumberOfRetries);

        /// <summary>
        /// Set CCA Sense Time
        /// </summary>
        /// <param name="CCASenseTime"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetCCASenseTime(byte CCASenseTime);

        /// <summary>
        /// Set Buffer Size
        /// </summary>
        /// <param name="BufferSize"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetBufferSize(byte BufferSize);

        /// <summary>
        /// Set Radio ID
        /// </summary>
        /// <param name="RadioID"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetRadioID(byte RadioID);

        /// <summary>
        /// Get CCA
        /// </summary>
        /// <returns>bool</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool GetCCA();

        /// <summary>
        /// Get number of retries
        /// </summary>
        /// <returns>byte</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetNumberOfRetries();

        /// <summary>
        /// Get CCA Sense Time
        /// </summary>
        /// <returns>byte</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetCCASenseTime();

        /// <summary>
        /// Get Radio ID
        /// </summary>
        /// <returns>byte</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetRadioID();


        /// <summary>
        /// Initialize native MAC, radio and interop drivers.
        /// </summary>
        /// <param name="config">MAC configuration.</param>
        /// <param name="receiveMessage">Byte array for received messages.</param>
        /// <returns>The driver status after initialization: Success, Fail, Ready, Busy</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern DeviceStatus InternalInitialize(byte[] marshalBuffer);  // Changed to private by Bill Leal 2/6/2013 per Mukundan Sridharan.


        /// <summary>
        /// Remove a message packet from the buffer.
        /// </summary>
        /// <param name="msg">The message packet to be removed.</param>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus RemovePacket(byte[] msg);

        /// <summary>
        /// Get a count of unprocessed packets in the buffer.
        /// </summary>
        /// <returns>The number of packets in the buffer not yet delivered to the program.</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetPendingPacketCount();

        /// <summary>
        /// Get the buffer size.
        /// </summary>
        /// <returns>The size of the buffer.</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetBufferSize();

        /// <summary>
        /// Get the details for a neighbor.
        /// </summary>
        /// <param name="macAddress">Address of the neighbor.</param>
        /// <param name="neighbor">Reference to Neighbor object, in whcich the result will be returned</param>
        /// <returns>Boolen. Success/Failure of operation</returns>
        public bool GetNeighborStatus(UInt16 macAddress, ref Neighbor neighbor)
        {
            if (GetNeighborInternal(macAddress, ByteNeighbor))
            {
                //Deserilize the object
                /*string str = new string(':',1);
                for (int i = 0; i < ByteNeighbor.Length; i++)
                {
                    str += ByteNeighbor[i].ToString() +':';
                }
                Debug.Print("Byte Nbr: " + str);
                */
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
                return true;
            }
            return false;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern bool GetNeighborInternal(UInt16 macAddress, byte[] byte_nbr);

        /// <summary>
        /// Get the ID of this CSMA instance.
        /// </summary>
        /// <returns>The ID of the instance.</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetID();

        /// <summary>
        /// Set the address of the device.
        /// </summary>
        /// <param name="Address">The address of the device.</param>
        /// <returns>True iff address successfully set.</returns>
        /// <remarks>This is the address by which the device is known to the rest of the world. A return value of false can occur if another layer locks the address and prevents changes.</remarks>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetAddress(UInt16 Address);

        /// <summary>
        /// Get the address of the device.
        /// </summary>
        /// <returns>Address of the device.</returns>
        /// <remarks>This is the address by which the device is known to the rest of the world.</remarks>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt16 GetAddress();

        /// <summary>
        /// Send a message.
        /// </summary>
        /// <param name="Address">
        /// The address of the receiver. Use 
        ///     <code>(UInt16)Addresses.BROADCAST</code>
        ///     for broadcast (65535).
        ///     </param>
        /// <param name="message">Byte array containing the message to be sent.</param>
        /// <param name="offset">The first byte in the array to send. Normally 0 for first byte.</param>
        /// <param name="size">The number of bytes to send.</param>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus Send(UInt16 Address, byte[] message, UInt16 offset, UInt16 size);

        /// <summary>
        /// Uninitialize the radio.
        /// </summary>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus UnInitialize();

        /// <summary>
        /// Configure the mac object, should be called before a call to get instance 
        /// </summary>
        /// <param name="config"></param>
        /// <param name="callback"></param>
        /// <returns></returns>
        public static DeviceStatus Configure(MacConfiguration config, ReceiveCallBack callback)
        {
            if (macconfig == null)
            {
                macconfig = new MacConfiguration();
                macconfig = config;
                receiveCallback = callback;
            }
            else
            {
                // Return busy if the mac configuration is busy
                return DeviceStatus.Busy;
            }

            return DeviceStatus.Success;
        }

    }
}
