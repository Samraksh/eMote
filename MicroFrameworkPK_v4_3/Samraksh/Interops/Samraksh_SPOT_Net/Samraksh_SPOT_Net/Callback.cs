using System;
using Microsoft.SPOT;

namespace Samraksh.SPOT.Net
{

    public enum CallbackTypes
    {
        Received,
        NeighbourChanged,
    }

    /// <summary>
    /// Callback class, should only be used internally by the mac and not by any user programs
    /// </summary>
    public static class Callbacks
    {
        private static ReceiveCallBack _receiveCallBack = null;
        private static NeighbourhoodChangeCallBack _neighbourChangeCallBack = null;

        public static void SetReceiveCallback(ReceiveCallBack callback)
        {
            _receiveCallBack = callback;
        }

        public static void SetNeighbourChangeCallback(NeighbourhoodChangeCallBack callback)
        {
            _neighbourChangeCallBack = callback;
        }

        public static ReceiveCallBack GetReceiveCallback()
        {
            return _receiveCallBack;
        }

        public static NeighbourhoodChangeCallBack GetNeighbourChangeCallback()
        {
            return _neighbourChangeCallBack;
        }

        public static void ReceiveFunction(uint data1, uint data2, DateTime time)
        {
            if ((data1 == (uint)CallbackTypes.Received) && (_receiveCallBack != null))
                _receiveCallBack((UInt16)data2);
            else if ((data1 == (uint)CallbackTypes.NeighbourChanged) && (_neighbourChangeCallBack != null))
                _neighbourChangeCallBack((UInt16)data2);

        }

    }
}
