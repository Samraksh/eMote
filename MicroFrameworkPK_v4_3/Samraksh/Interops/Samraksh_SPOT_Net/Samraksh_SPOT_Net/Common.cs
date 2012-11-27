using System;
using Microsoft.SPOT;

namespace Samraksh.SPOT.Net
{
    public enum DeviceStatus
    {
        Success,
        Fail,
        Ready,
        Busy,
    };

    public enum NetOpStatus
    {
        E_RadioInit,
        E_RadioSync,
        E_RadioConfig,
        E_MacInit,
        E_MacConfig,
        E_MacSendError,
        E_MacBufferFull,
        S_Success
    };

    public delegate void ReceiveCallBack(byte[] message, UInt16 size);



}
