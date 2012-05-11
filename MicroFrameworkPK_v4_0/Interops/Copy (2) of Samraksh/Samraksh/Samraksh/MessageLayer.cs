using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Runtime.CompilerServices;

namespace Samraksh
{
    

    public class Time
    {
        private int m_time = 0;

        public int time
        {
            get
            {
                return m_time;
            }
            set
            {
                m_time = value;
            }
        }
    }

    public class SendDoneEvent : NativeEventDispatcher
    {
      public SendDoneEvent( string strDrvName, ulong drvData, int callbackCount )
          : base( strDrvName, drvData )
      {
          
      }

      public void OnNativeEvent( uint data1, uint data2, DateTime time )
      {
          
      }
    }

    public class ReceiveDoneEvent : NativeEventDispatcher
    {
        public ReceiveDoneEvent(string strDrvName, ulong drvData, int callbackCount)
          : base( strDrvName, drvData )
      {
          
      }

      public void OnNativeEvent( uint data1, uint data2, DateTime time )
      {
          
      }
       
    }

    public class MessageLayer
    {
        
        public delegate void RadioCallBack(Object state);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int Init();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int Send(byte Id, UInt16 address, byte[] payload, UInt16 len);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int Received(byte[] payload);

        

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static public int ConfigureReceiver(byte Id,RadioCallBack callBack);

    }
}
