using System;
using Microsoft.SPOT;

namespace Samraksh.SPOT.Net
{
    /// <summary>
    /// Device status
    /// </summary>
    public enum DeviceStatus
    {
        /// <summary>
        /// Success
        /// </summary>
        Success,
        /// <summary>
        /// Fail
        /// </summary>
        Fail,
        /// <summary>
        /// Ready
        /// </summary>
        Ready,
        /// <summary>
        /// Busy 
        /// </summary>
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
    /// ReceiveCallback delegate 
    /// </summary>
    /// <param name="NumberOfPacketsReceived">Number of packets in the receive buffer</param>
    public delegate void ReceiveCallBack(int NumberOfPacketsReceived);



}
