using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Runtime.CompilerServices;


namespace Samraksh
{
    public static class TimeSync
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public void Init();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public void setBeaconRate(int BeaconRate);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public void sendTimeSyncMessage();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int getLocalTime();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int getGlobalTime();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public float getSkew();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public void local2Global();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public Int16 getRootId();


    }
    public class TestCallback : NativeEventDispatcher
    {
        int m_CallbackReceived;
        int m_CallbackRequired;


        // Pass the name to the base so it connects to driver 
        public TestCallback(string strDrvName, ulong drvData, int callbackCount)
            : base(strDrvName, drvData)
        {
            m_CallbackReceived = 0;
            m_CallbackRequired = callbackCount;
        }

        public void OnNativeEvent(uint data1, uint data2, DateTime time)
        {
            
        }

        
    }
}
