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
    /// Receive callback delegate
    /// </summary>
    /// <param name="message">The message.</param>
    /// <param name="size">The size of the message.</param>
    /// <param name="Source">The source of thec current message</param>
    /// <param name="Unicast">Boolen: True if message is addressed to the node</param>
    /// <param name="RSSI">Received Signal Strength. Range and units of values depends on specific radio.</param>
    /// <param name="linkQuality">A indicator of the link quality calculated by radio layer. Range of values depends on specific radio.</param>
    public delegate void ReceiveCallBack(byte[] message, UInt16 size, UInt16 Source, bool Unicast, byte RSSI, byte linkQuality);



}
