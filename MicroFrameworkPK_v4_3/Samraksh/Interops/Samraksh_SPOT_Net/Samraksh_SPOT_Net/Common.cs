using System;
using Microsoft.SPOT;

namespace Samraksh.SPOT.Net
{
    /// <summary>
    /// Device status
    /// </summary>
    public enum DeviceStatus
    {
        Success,
        Fail,
        Ready,
        Busy,
    };

    /// <summary>
    /// Net operation status
    /// </summary>
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

    /// <summary>
    /// Receive callback delegate
    /// </summary>
    /// <param name="message">The message.</param>
    /// <param name="size">The size of the message.</param>
    public delegate void ReceiveCallBack(byte[] message, UInt16 size);



}
