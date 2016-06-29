namespace Samraksh.eMote.Net
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
        /// <summary>
        /// Timeout 
        /// </summary>
        Timeout,
        /// <summary>
        /// There is a bug in the code
        /// </summary>
        Bug
    }

    /// <summary>
    /// Net operation result status
    /// </summary>
    public enum NetOpStatus
    {
        /// <summary>
        /// Radio initialization failed
        /// </summary>
		// ReSharper disable once InconsistentNaming
		E_RadioInit,
        /// <summary>
        /// Radio synchronization failed
        /// </summary>
		// ReSharper disable once InconsistentNaming
		E_RadioSync,
        /// <summary>
        /// Radio configuration failed
        /// </summary>
		// ReSharper disable once InconsistentNaming
		E_RadioConfig,
        /// <summary>
        /// MAC layer initialization failed
        /// </summary>
		// ReSharper disable once InconsistentNaming
		E_MACInit,
        /// <summary>
        /// MAC configuration failed
        /// </summary>
		// ReSharper disable once InconsistentNaming
		E_MACConfig,
        /// <summary>
        /// MAC layer send failed
        /// </summary>
		// ReSharper disable once InconsistentNaming
		E_MACSendError,
        /// <summary>
        /// MAC layer bufferis full
        /// </summary>
		// ReSharper disable once InconsistentNaming
		E_MACBufferFull,
        /// <summary>
        /// Success
        /// </summary>
		// ReSharper disable once InconsistentNaming
		S_Success
    };

    /// <summary>
    /// Kinds of callbacks
    /// </summary>
    public enum CallbackType
    {
        /// <summary>Packet received</summary>
        Received,
        /// <summary>Neighborhood has changed</summary>
        NeighborChanged,
    }

    /*/// <summary>
    /// 
    /// </summary>
    /// <param name="macBase"></param>
    //public delegate void ReceiveCallBackGlobal(MACBase macBase);

    /// <summary>
    /// ReceiveCallback delegate 
    /// </summary>
    /// <param name="macBase">Type of MAC object</param>
    //public delegate void ReceiveCallBack(MACBase macBase);

	/// <summary>
	/// Neighborhood callback delegate
	/// </summary>
    /// <param name="macBase">Type of MAC object</param>
	/// <param name="countOfChangedNeighbors">Count of neighbors that have changed</param>
    public delegate void NeighborhoodChangeCallBack(MACBase macBase, UInt16 countOfChangedNeighbors);*/
}
