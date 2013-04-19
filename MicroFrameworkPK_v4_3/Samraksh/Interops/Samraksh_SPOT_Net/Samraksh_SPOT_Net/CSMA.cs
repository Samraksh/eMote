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
        const byte MarshalBufferSize = 5;
        const byte MacMessageSize = 128;
        const byte NeighborSize = 22; //Look at IMac.cs to figure out the size of the Neighbor structure.
        static ReceiveCallBack MyReceiveCallback;
        static byte RSSI, LinkQuality;
        static UInt16 Src;
        static bool Unicast;
        static byte[] ReceiveMessage = new byte[MacMessageSize];
        
        static byte[] ByteNeighbor = new byte[NeighborSize];

        static byte[] MarshalBuffer = new byte[MarshalBufferSize];

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
            Src = (UInt16)((data1 >> 16) & 0x0000FFFF);
            RSSI = (byte)(data2 & 0x000000FF);
            LinkQuality = (byte)((data2 >> 8) & 0x000000FF);
            Unicast = ((data2 >> 16) & 0x000000FF) > 0 ? true : false ;
            MyReceiveCallback(ReceiveMessage, (UInt16)data1, Src, Unicast, RSSI, LinkQuality);
        }

        /// <summary>
        /// Set the CCA
        /// </summary>
        /// <param name="CCA"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern DeviceStatus SetCCA(bool CCA);

        /// <summary>
        /// Set Number of retries of the MAC
        /// </summary>
        /// <param name="NumberOfRetries"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern DeviceStatus SetNumberOfRetries(byte NumberOfRetries);

        /// <summary>
        /// Set CCA Sense Time
        /// </summary>
        /// <param name="CCASenseTime"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern DeviceStatus SetCCASenseTime(byte CCASenseTime);

        /// <summary>
        /// Set Buffer Size
        /// </summary>
        /// <param name="BufferSize"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern DeviceStatus SetBufferSize(byte BufferSize);

        /// <summary>
        /// Set Radio ID
        /// </summary>
        /// <param name="RadioID"></param>
        /// <returns>DeviceStatus</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern DeviceStatus SetRadioID(byte RadioID);

        /// <summary>
        /// Get CCA
        /// </summary>
        /// <returns>bool</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetCCA();

        /// <summary>
        /// Get number of retries
        /// </summary>
        /// <returns>byte</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern byte GetNumberOfRetries();

        /// <summary>
        /// Get CCA Sense Time
        /// </summary>
        /// <returns>byte</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern byte GetCCASenseTime();

        /// <summary>
        /// Get Radio ID
        /// </summary>
        /// <returns>byte</returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern byte GetRadioID();

        /// <summary>
        /// ReConfigure the mac by passing a new macconfiguration
        /// </summary>
        /// <param name="config"></param>
        /// <returns>DeviceStatus</returns>
        public static DeviceStatus ReConfigure(MacConfiguration config)
        {
            if (config.CCA == true)
                MarshalBuffer[0] = 1;
            else
                MarshalBuffer[0] = 0;

            MarshalBuffer[1] = config.NumberOfRetries;
            MarshalBuffer[2] = config.CCASenseTime;
            MarshalBuffer[3] = config.BufferSize;
            MarshalBuffer[4] = config.RadioID;

            return ReConfigure(MarshalBuffer);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern DeviceStatus ReConfigure(byte[] buffer);
        
          
        

        /// <summary>
        /// Get the details for a neighbor.
        /// </summary>
        /// <param name="macAddress">Address of the neighbor.</param>
        /// <param name="neighbor">Reference to Neighbor object, in whcich the result will be returned</param>
        /// <returns>Boolen. Success/Failure of operation</returns>
        public bool GetNeighborStatus(UInt16 macAddress,ref Neighbor neighbor)
        {
            if(GetNeighborInternal(macAddress,ByteNeighbor)){
                //Deserilize the object
                /*string str = new string(':',1);
                for (int i = 0; i < ByteNeighbor.Length; i++)
                {
                    str += ByteNeighbor[i].ToString() +':';
                }
                Debug.Print("Byte Nbr: " + str);
                */
                neighbor.MacAddress = (UInt16)(((UInt16)(ByteNeighbor[1] << 8) & 0xFF00) + (UInt16)ByteNeighbor[0]);//MacAddress
                neighbor.ForwardLink.AveRSSI= ByteNeighbor[2]; //ForwardLink
                neighbor.ForwardLink.LinkQuality=ByteNeighbor[3];
                neighbor.ForwardLink.AveDelay=ByteNeighbor[4];
                neighbor.ReverseLink.AveRSSI= ByteNeighbor[5];  //ReverseLink
                neighbor.ReverseLink.LinkQuality=ByteNeighbor[6];
                neighbor.ReverseLink.AveDelay=ByteNeighbor[7];
                neighbor.Status=(NeighborStatus)ByteNeighbor[8];//Status
                neighbor.PacketsReceived = (UInt16)(((ByteNeighbor[10] << 8) & 0xFF00) + ByteNeighbor[9] );//MacAddress
                neighbor.LastHeardTime = (UInt64)((ByteNeighbor[18] << 56) + ByteNeighbor[17] << 48 + ByteNeighbor[16] << 40 + ByteNeighbor[15] << 32 + ByteNeighbor[14] << 24 + 
                ByteNeighbor[13] << 16 + ByteNeighbor[12] << 8 + + ByteNeighbor[11] );//LastTimeHeard
                neighbor.ReceiveDutyCycle = ByteNeighbor[19];//ReceiveDutyCycle
                neighbor.FrameLength = (UInt16)(((ByteNeighbor[21] << 8) & 0xFF00) + ByteNeighbor[20] );
                return true;
            }
            return false;
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
    }
}
