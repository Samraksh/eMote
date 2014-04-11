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

        /// <summary>
        /// Set the receive callback to be called on arrival of a packet
        /// </summary>
        /// <param name="callback">Receivecallback function pointer</param>
        public static void SetReceiveCallback(ReceiveCallBack callback)
        {
            _receiveCallBack = callback;
        }

        /// <summary>
        /// Set the neighbour change callback to be called when there is a change in neighbour table
        /// </summary>
        /// <param name="callback">Neighbourcallback function pointer</param>
        public static void SetNeighbourChangeCallback(NeighbourhoodChangeCallBack callback)
        {
            _neighbourChangeCallBack = callback;
        }

        /// <summary>
        /// Returns the recieve callback function
        /// </summary>
        /// <returns></returns>
        public static ReceiveCallBack GetReceiveCallback()
        {
            return _receiveCallBack;
        }

        /// <summary>
        /// Returns the neighbour change callback function registered
        /// </summary>
        /// <returns></returns>
        public static NeighbourhoodChangeCallBack GetNeighbourChangeCallback()
        {
            return _neighbourChangeCallBack;
        }

        /// <summary>
        /// First level callback from native code
        /// </summary>
        /// <param name="data1"></param>
        /// <param name="data2"></param>
        /// <param name="time"></param>
        public static void ReceiveFunction(uint data1, uint data2, DateTime time)
        {

            if (_receiveCallBack == null)
                throw new CallbackNotConfiguredException();

            if (_neighbourChangeCallBack == null)
                Debug.Print("The Neighbour Callback has not been configured \n");

            if ((data1 == (uint)CallbackTypes.Received) && (_receiveCallBack != null))
                _receiveCallBack((UInt16)data2);
            else if ((data1 == (uint)CallbackTypes.NeighbourChanged) && (_neighbourChangeCallBack != null))
                _neighbourChangeCallBack((UInt16)data2);

        }

    }
}
