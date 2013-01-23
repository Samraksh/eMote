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

    public class CSMA: NativeEventDispatcher, IMac
    {
        const byte MacMessageSize = 128;
        static ReceiveCallBack MyReceiveCallback;
        static byte[] ReceiveMessage = new byte[MacMessageSize];

        public CSMA()
            :base ("CSMACallback",4321)
        {

        }       

        //Initializes Return the ID of the Radio layer that was initialized
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

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern DeviceStatus InternalInitialize(MacConfiguration config, byte[] receiveMessage);       

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus Configure(MacConfiguration config);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus UnInitialize();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus Send(UInt16 Address, byte[] message, UInt16 offset, UInt16 size);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt16 GetAddress();
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetAddress(UInt16 Address);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetID();

        //Neighbor functions

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NeighborTable GetNeighborTable();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Neighbor GetNeighbors(UInt16 macAddress);

        //Buffer functions
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetBufferSize();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetPendingPacketCount();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus RemovePacket(byte[] msg);
    }
}
