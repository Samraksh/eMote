using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

//using Samraksh.SPOT.Net.Radio;
using System.Runtime.CompilerServices;

namespace Samraksh.SPOT.Net.Radio
{
    /*class Radio_15_4_CB : NativeEventDispatcher
    {
        public Radio_15_4_CB(string strDrvName, ulong drvData, int callbackCount)
            : base(strDrvName, drvData)
        {

        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int GetStatus();
    }*/

    public class Radio_802_15_4 : NativeEventDispatcher, IRadio
    {
        const byte RadioMessageSize = 128;
        static ReceiveCallBack MyReceiveCallback;
        static byte[] ReceiveMessage = new byte[RadioMessageSize];

        public Radio_802_15_4()
            : base("RadioCallback_802_15_4", 1234)
        {           
        }
        
        //Initializes Return the ID of the Radio layer that was initialized
        public DeviceStatus Initialize(RadioConfiguration config, ReceiveCallBack callback)
        {
            MyReceiveCallback = callback;
            NativeEventHandler eventHandler = new NativeEventHandler(ReceiveFunction);
            OnInterrupt += eventHandler;
            return InternalInitialize(config, ReceiveMessage);
        }

        private static void ReceiveFunction(uint data1, uint data2, DateTime time)
        {
            MyReceiveCallback(ReceiveMessage, (UInt16) data1);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern DeviceStatus InternalInitialize(RadioConfiguration config, byte[] receiveMessage);
        
        //Change configuration after initialization
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus Configure(RadioConfiguration config);
       
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus UnInitialize();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern byte GetID();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern UInt16 GetAddress();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool SetAddress(UInt16 Address);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus TurnOn();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern DeviceStatus Sleep(byte level);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus PreLoad(byte[] message, UInt16 size); 

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus SendStrobe();	//Send preloaded message
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus Send(byte[] message, UInt16 size);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern NetOpStatus SendTimeStamped(byte[] message, UInt16 size, UInt32 eventTime);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ClearChannelAssesment();
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern bool ClearChannelAssesment(UInt16 numberOfMicroSecond);
    }
}
