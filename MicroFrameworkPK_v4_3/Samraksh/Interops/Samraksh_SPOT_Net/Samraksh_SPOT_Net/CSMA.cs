using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;
using Samraksh.SPOT.Net.Mac;

namespace Samraksh.SPOT.Net.Mac
{
    public class CSMA : MACBase
    {
        private static CSMA instance;
        private static object syncObject = new Object();

        private CSMA() : base(MacID.CSMA)
        {
        }

        // There can only be one csma mac object in the system
        /// <summary>
        /// Returns the instance of the CSMA object
        /// </summary>
        public static CSMA Instance
        {
            get
            {
                if (instance == null)
                {
                    lock (syncObject)
                    {
                        if (instance == null)
                            instance = new CSMA();
                    }
                }

                return instance;
            }
        }

<<<<<<< HEAD




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
        /*
        /// <summary>
        /// Get the device neighbors.
        /// </summary>
        /// <param name="table">Reference to NeighborTable object, in which the result will be returned</param>
        /// <returns>Boolen. Success/Failure of operation</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool GetNeighborTable(NeighborTable table);
        */
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern bool GetNeighborInternal(UInt16 macAddress, byte[] byte_nbr);

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

        /// <summary>
        /// Set the transmit power of the 802.15.4 radio.
        /// </summary>
        /// <param name="TxPower"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetTxPower(int TxPower);

        /// <summary>
        /// Set the channel of the 802.15.4 radio.
        /// </summary>
        /// <param name="Channel"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SetChannel(int Channel);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern int GetChannel(int RadioID);

        /// <summary>
        /// Get the current active channel from the radio
        /// </summary>
        /// <returns>Channel</returns>
        public Channels GetChannel()
        {
            return (Channels)GetChannel(1);
        }

        // Get the TxPower
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern int GetTxPower(int RadioID);

        /// <summary>
        /// Get the current tx power of the radio
        /// </summary>
        /// <returns>TxPowerValue</returns>
        public TxPowerValue GetTxPower()
        {
            return (TxPowerValue) GetTxPower(1);
        }


        /// <summary>
        /// Turn on the radio.
        /// </summary>
        /// <returns>The status after the method call: Success, Fail, Ready, Busy</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus TurnOnRadio();

        /// <summary>
        /// Go to sleep.
        /// </summary>
        /// <param name="level">The sleep level.</param>
        /// <returns>The status after the method call: Success, Fail, Ready, Busy</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus SleepRadio(byte level);
=======
>>>>>>> netdll
    }
}
