using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net.Mac;
using System.Runtime.CompilerServices;
using Microsoft.SPOT.Hardware;

namespace Samraksh.SPOT.Net
{
    // Making this private as the user should really never be seeing this 
    public class MACBase : NativeEventDispatcher, IMac
    {
        /// <summary>
        /// Specifies the marshalling buffer size, used by the config to pass data to native code 
        /// </summary>
        const byte MarshalBufferSize = 5;


        /// <summary>
        /// Specifies the neighbour size
        /// </summary>
        const byte NeighborSize = 22; //Look at IMac.cs to figure out the size of the Neighbor structure.

        Message message;

        byte[] ByteNeighbor = new byte[NeighborSize];

        byte[] MarshalBuffer = new byte[MarshalBufferSize];

        /// <summary>
        /// Callback for the MAC object 
        /// </summary>
        ReceiveCallBack MyReceiveCallback;

        /// <summary>
        /// Constructor to initialize callbacks 
        /// </summary>
        /// <param name="drvName">Name of the callback</param>
        /// <param name="drvData">Driver data</param>
        public MACBase(string drvName ,ulong drvData) : base(drvName, drvData) 
        {
        }


        private void ReceiveFunction(uint data1, uint data2, DateTime time)
        {
            /*
            Src = (UInt16)((data1 >> 16) & 0x0000FFFF);
            RSSI = (byte)(data2 & 0x000000FF);
            LinkQuality = (byte)((data2 >> 8) & 0x000000FF);
            Unicast = ((data2 >> 16) & 0x000000FF) > 0 ? true : false;
             */
            //MyReceiveCallback(ReceiveMessage, (UInt16)data1, Src, Unicast, RSSI, LinkQuality);
        }

        /// <summary>
        /// Initialize MAC
        /// </summary>
        /// <param name="config">MAC configuration.</param>
        /// <param name="callback">Callback method for received messages.</param>
        /// <returns>The radio status after initialization: Success, Fail, Ready, Busy</returns>
        /// <remarks>By default, uses Radio #1 on the device. For the eMote.NOW 1.0, that is the 802.15.4 radio.</remarks>
        public DeviceStatus Initialize(MacConfiguration config, ReceiveCallBack callback)
        {
            MyReceiveCallback = callback;
            NativeEventHandler eventHandler = new NativeEventHandler(ReceiveFunction);
            OnInterrupt += eventHandler;
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
        /// Set MAC configuration for CSMA.
        /// </summary>
        /// <param name="config">MAC configuration.</param>
        /// <returns>The radio status after initialization: Success, Fail, Ready, Busy</returns>
        /// <remarks>Used to change the MAC configuration after initialization.</remarks>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus Configure(MacConfiguration config);


    }
}
