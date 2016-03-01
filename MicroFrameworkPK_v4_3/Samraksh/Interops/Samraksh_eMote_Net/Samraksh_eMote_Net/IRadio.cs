using System;

namespace Samraksh.eMote.Net.Radio
{
    /// <summary>
    /// Kind of user
    /// </summary>
    public enum RadioUser
    {
        /// <summary>
        /// Radio Object is used by a C# application
        /// </summary>
        CSharp,
        /// <summary>
        /// Radio is being used by the CSMA Mac object
        /// </summary>
        CSMAMAC,
        /// <summary>
        /// Radio is being used by the OMAC Mac object
        /// </summary>
        OMAC,
        /// <summary>
        /// Radio is idle
        /// </summary>
        IDLE,
    };


    /// <summary>
    /// Radio configuration
    /// </summary>
    public class RadioConfiguration
    {
       /// <summary>
        /// Transmission power of the radio
        /// </summary>
        private TxPowerValue TxPower;
        /// <summary>
        /// Channel the radio will transmit on
        /// </summary>
        private Channels Channel;

        private RadioName Name;

        private ReceiveCallBack OnReceiveCallback;
        private NeighborhoodChangeCallBack OnNeighborChangeCallback;

        /// <summary>
        /// Radio configuration constructor
        /// </summary>
        public RadioConfiguration()
        {
            TxPower = TxPowerValue.Power_3dBm;
            Channel = Channels.Channel_26;
            Name = RadioName.RF231RADIO;
        }

        /// <summary>
        /// Radio configuration constructor
        /// </summary>
        /// <param name="config">Radio configuration</param>
        public RadioConfiguration(RadioConfiguration config)
        {
            this.TxPower = config.TxPower;
            this.Channel = config.Channel;
            this.Name = config.Name;
            this.OnReceiveCallback = config.OnReceiveCallback;
            this.OnNeighborChangeCallback = config.OnNeighborChangeCallback;
        }

        /// <summary>
        /// Radio configuration constructor
        /// </summary>
        /// <param name="power">Transmission power</param>
        /// <param name="channel">Channel</param>
        /// <param name="name">name</param>
        public RadioConfiguration(TxPowerValue power, Channels channel, RadioName name)
        {
            this.Channel = channel;
            this.TxPower = power;
            this.Name = name;
        }

        /// <summary>
        /// Set the channel 
        /// </summary>
        /// <param name="channel">Channel</param>
        public void SetChannel(Channels channel)
        {
            this.Channel = channel;
        }

        /// <summary>
        /// Get current channel
        /// </summary>
        /// <returns>Channel</returns>
        public Channels GetChannel()
        {
            return Channel;
        }

        /// <summary>
        /// Set the transmission power of the radio
        /// </summary>
        /// <param name="power">Transmission power value</param>
        public void SetTxPower(TxPowerValue power)
        {
            this.TxPower = power;
        }

        /// <summary>
        /// Get current transmission power
        /// </summary>
        /// <returns>Transmission power</returns>
        public TxPowerValue GetTxPower()
        {
            return TxPower;
        }

        /// <summary>
        /// Set radio name 
        /// </summary>
        /// <param name="name">Radio name</param>
        public void SetRadioName(RadioName name)
        {
            this.Name = name;
        }

        /// <summary>
        /// Get radio name 
        /// </summary>
        /// <returns>Radio name</returns>
        public RadioName GetRadioName()
        {
            return this.Name;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="OnReceiveCallBack"></param>
        public void SetReceiveCallBack(ReceiveCallBack onReceiveCallBack)
        {
            this.OnReceiveCallback = onReceiveCallBack;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public ReceiveCallBack GetReceiveCallBack()
        {
            return this.OnReceiveCallback;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="onNeighborChangeCallBack"></param>
        public void SetNeighborChangeCallBack(NeighborhoodChangeCallBack onNeighborChangeCallBack)
        {
            this.OnNeighborChangeCallback = onNeighborChangeCallBack;
        }

        /// <summary>
        /// 
        /// </summary>
        public NeighborhoodChangeCallBack GetNeighborChangeCallBack()
        {
            return this.OnNeighborChangeCallback;
        }

    };


    /// <summary>
    /// Radio interface
    /// </summary>
    public interface IRadio
    {
        //DeviceStatus Initialize(RadioConfiguration config, ReceiveCallBack callback); //Initializes Return the ID of the Radio layer that was initialized
        //DeviceStatus Configure(RadioConfiguration config, ReceiveCallBack callback);  //Change configuration after initialization

        /// <summary>
        /// Unitialize radio
        /// </summary>
        /// <returns>Success of operation</returns>
        DeviceStatus UnInitialize();

        /// <summary>
        /// Reconfigure radio
        /// </summary>
        /// <param name="config">New radio configuration</param>
        /// <returns>Success of operation</returns>
        DeviceStatus ReConfigure(RadioConfiguration config);

        /*/// <summary>
        /// Get radio ID
        /// </summary>
        /// <returns>Radio ID</returns>
        byte GetID();*/

        /// <summary>
        /// Get radio address
        /// </summary>
        /// <returns>Radio address</returns>
        UInt16 GetAddress(byte radioID);

        /// <summary>
        /// Set radio address
        /// </summary>
        /// <param name="radioID">Radio ID</param>
        /// <param name="address">Radio address</param>
        /// <returns></returns>
        bool SetAddress(byte radioID, UInt16 address);

        /// <summary>
        /// Turn radio on
        /// </summary>
        /// <param name="radioID">Radio ID</param>
        /// <returns>Success of operation</returns>
        DeviceStatus TurnOnRx(byte radioID);

        /// <summary>
        /// Put radio to sleep
        /// </summary>
        /// <param name="radioID">Radio ID</param>
        /// <param name="level">Sleep level</param>
        /// <returns>Success of operation</returns>
        DeviceStatus Sleep(byte radioID, byte level);

        /// <summary>
        /// Preload radio
        /// </summary>
        /// <param name="packet">Packet to preload</param>
        /// <param name="size">Size of packet</param>
        /// <returns>Success of operation</returns>
        NetOpStatus PreLoad(byte[] packet, UInt16 size);

        /// <summary>
        /// Send preloaded message
        /// </summary>
        /// <returns>Success of operation</returns>
        NetOpStatus SendStrobe();

        /// <summary>
        /// Send message on radio
        /// </summary>
        /// <param name="radioID">Radio ID</param>
        /// <param name="packet">Packet to send</param>
        /// <param name="size">Size of packet</param>
        /// <returns>Success of operation</returns>
        NetOpStatus Send(byte radioID, byte[] packet, UInt16 size);

        /// <summary>
        /// Sent time-stamped message
        /// </summary>
        /// <param name="radioID">Radio ID</param>
        /// <param name="packet">Packet buffer to send</param>
        /// <param name="size">Size of packet</param>
        /// <param name="eventTime">Time stamp of message</param>
        /// <returns>Send status</returns>
        NetOpStatus SendTimeStamped(byte radioID, byte[] packet, UInt16 size, UInt32 eventTime);

        /// <summary>
        /// Check if channel is clear
        /// </summary>
        /// <param name="radioID">Radio ID</param>
        /// <returns>True iff clear</returns>
        bool ClearChannelAssesment(byte radioID);

        /// <summary>
        /// Check if channel has been clear for the specified interval of time
        /// </summary>
        /// <param name="radioID">Radio ID</param>
        /// <param name="numberOfMicroSecond">Interval (microseconds)</param>
        /// <returns>True iff clear</returns>
        bool ClearChannelAssesment(byte radioID, UInt16 numberOfMicroSecond);

        //UInt32 GetSNR();
        //UInt32 GetRSSI();

    }
}
