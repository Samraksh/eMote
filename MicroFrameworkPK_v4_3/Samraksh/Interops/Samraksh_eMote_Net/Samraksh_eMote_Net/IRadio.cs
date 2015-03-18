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

        private RadioName name;

        /// <summary>
        /// Radio configuration constructor
        /// </summary>
        public RadioConfiguration()
        {
            TxPower = TxPowerValue.Power_3dBm;
            Channel = Channels.Channel_26;
            name = RadioName.RF231RADIO;
        }

        /// <summary>
        /// Radio configuration constructor
        /// </summary>
        /// <param name="config">Radio configuration</param>
        public RadioConfiguration(RadioConfiguration config)
        {
            this.TxPower = config.TxPower;
            this.Channel = config.Channel;
            this.name = config.name;
        }

        /// <summary>
        /// Radio configuration constructor
        /// </summary>
        /// <param name="power">Transmission power</param>
        /// <param name="channel">Channel</param>
        public RadioConfiguration(TxPowerValue power, Channels channel, RadioName name)
        {
            this.Channel = channel;
            this.TxPower = power;
            this.name = name;
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
            this.name = name;
        }

        /// <summary>
        /// Get radio name 
        /// </summary>
        /// <returns>Radio name</returns>
        public RadioName GetRadioName()
        {
            return this.name;
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

        /// <summary>
        /// Get radio ID
        /// </summary>
        /// <returns>Radio ID</returns>
        byte GetID();

        /// <summary>
        /// Get radio address
        /// </summary>
        /// <returns>Radio address</returns>
        UInt16 GetAddress();

        /// <summary>
        /// Set radio address
        /// </summary>
        /// <param name="address">Radio address</param>
        /// <returns></returns>
        bool SetAddress(UInt16 address);

        /// <summary>
        /// Turn radio on
        /// </summary>
        /// <returns>Success of operation</returns>
        DeviceStatus TurnOn();

        /// <summary>
        /// Put radio to sleep
        /// </summary>
        /// <param name="level">Sleep level</param>
        /// <returns>Success of operation</returns>
        DeviceStatus Sleep(byte level);

        /// <summary>
        /// Preload radio
        /// </summary>
        /// <param name="message">Message to preload</param>
        /// <param name="size">Size of message</param>
        /// <returns>Success of operation</returns>
        NetOpStatus PreLoad(byte[] message, UInt16 size);

        /// <summary>
        /// Send preloaded message
        /// </summary>
        /// <returns>Success of operation</returns>
        NetOpStatus SendStrobe();

        /// <summary>
        /// Send message on radio
        /// </summary>
        /// <param name="message">Message to send</param>
        /// <param name="size">Size of message</param>
        /// <returns>Success of operation</returns>
        NetOpStatus Send(byte[] message, UInt16 size);

        /// <summary>
        /// Sent time-stamped message
        /// </summary>
        /// <param name="message">Message to send</param>
        /// <param name="size">Size of message</param>
        /// <param name="eventTime">Time stamp of message</param>
        /// <returns>Success of message</returns>
        NetOpStatus SendTimeStamped(byte[] message, UInt16 size, UInt32 eventTime);

        /// <summary>
        /// Check if channel is clear
        /// </summary>
        /// <returns>True iff clear</returns>
        bool ClearChannelAssesment();

        /// <summary>
        /// Check if channel has been clear for the specified interval of time
        /// </summary>
        /// <param name="numberOfMicroSecond">Interval (microseconds)</param>
        /// <returns>True iff clear</returns>
        bool ClearChannelAssesment(UInt16 numberOfMicroSecond);

        //UInt32 GetSNR();
        //UInt32 GetRSSI();

    }
}
