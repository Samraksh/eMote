using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

using Samraksh.SPOT.Net.Radio;
using System.Runtime.CompilerServices;


namespace Samraksh.SPOT.Net.Mac
{

    /*class CSMA 
    {
        const byte MacMessageSize = 128;
        static CSMA_CB ReceiveCB;
        static ReceiveCallBack MyReceiveCallback;
        static byte[] ReceiveMessage = new byte[MacMessageSize];   

        public CSMA()
        {
            ReceiveCB = new CSMAInternal("CSMACallback", 4231, 0);   
        }

       
        
        
        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int GetStatus();

    }*/

    /// <summary>
    /// CSMA protocol configuration
    /// </summary>
    /// <remarks>By default, uses Radio #1 on the device. For the eMote.NOW 1.0, that is the 802.15.4 radio.</remarks>
    public class CSMA: NativeEventDispatcher, IMac
    {
        const byte MacMessageSize = 128;
        static ReceiveCallBack MyReceiveCallback;
        static byte[] ReceiveMessage = new byte[MacMessageSize];

        /// <summary>
        /// Constructor for CSMA class
        /// </summary>
        public CSMA()
            :base ("CSMACallback",4321)
        {

        }       

        /// <summary>
        /// Initialize CSMA.
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
            return InternalInitialize(config, ReceiveMessage);
        }

        private static void ReceiveFunction(uint data1, uint data2, DateTime time)
        {
            MyReceiveCallback(ReceiveMessage, (UInt16)data1);
        }

        /// <summary>
        /// Initialize native MAC, radio and interop drivers.
        /// </summary>
        /// <param name="config">MAC configuration.</param>
        /// <param name="receiveMessage">Byte array for received messages.</param>
        /// <returns>The driver status after initialization: Success, Fail, Ready, Busy</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern DeviceStatus InternalInitialize(MacConfiguration config, byte[] receiveMessage);  // Changed to private by Bill Leal 2/6/2013 per Mukundan Sridharan.

        /// <summary>
        /// Set MAC configuration for CSMA.
        /// </summary>
        /// <param name="config">MAC configuration.</param>
        /// <returns>The radio status after initialization: Success, Fail, Ready, Busy</returns>
        /// <remarks>Used to change the MAC configuration after initialization.</remarks>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus Configure(MacConfiguration config);

        /// <summary>
        /// Uninitialize the radio.
        /// </summary>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus UnInitialize();

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
        /// Get the address of the device.
        /// </summary>
        /// <returns>Address of the device.</returns>
        /// <remarks>This is the address by which the device is known to the rest of the world.</remarks>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt16 GetAddress();
        
        /// <summary>
        /// Set the address of the device.
        /// </summary>
        /// <param name="Address">The address of the device.</param>
        /// <returns>True iff address successfully set.</returns>
        /// <remarks>This is the address by which the device is known to the rest of the world. A return value of false can occur if another layer locks the address and prevents changes.</remarks>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetAddress(UInt16 Address);

        /// <summary>
        /// Get the ID of this CSMA instance.
        /// </summary>
        /// <returns>The ID of the instance.</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetID();

        //Neighbor functions

        /// <summary>
        /// Get the device neighbors.
        /// </summary>
        /// <returns>Neighbors of the device.</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NeighborTable GetNeighborTable();

        /// <summary>
        /// Get the details for a neighbor.
        /// </summary>
        /// <param name="macAddress">Address of the neighbor.</param>
        /// <returns>The details for the neighbor.</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Neighbor GetNeighbors(UInt16 macAddress);

        //Buffer functions
        
        /// <summary>
        /// Get the buffer size.
        /// </summary>
        /// <returns>The size of the buffer.</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetBufferSize();

        /// <summary>
        /// Get a count of unprocessed packets in the buffer.
        /// </summary>
        /// <returns>The number of packets in the buffer not yet delivered to the program.</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetPendingPacketCount();

        /// <summary>
        /// Remove a message packet from the buffer.
        /// </summary>
        /// <param name="msg">The message packet to be removed.</param>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus RemovePacket(byte[] msg);
    }
}
