using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;

namespace Samraksh.SPOT.Net.Radio
{
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

        /// <summary>
        /// Create a radio configuration object with default power (3dBm) and channel (26) 
        /// </summary>
        public RadioConfiguration()
        {
            TxPower = TxPowerValue.Power_3dBm;
            Channel = Channels.Channel_26;
        }

        /// <summary>
        /// Constructor takes RadioConfiguration as a parameter
        /// </summary>
        /// <param name="config">RadioConfiguration</param>
        public RadioConfiguration(RadioConfiguration config)
        {
            this.TxPower = config.TxPower;
            this.Channel = config.Channel;
        }

        /// <summary>
        /// Constructor that takes power and channel as input
        /// </summary>
        /// <param name="power">Transmission power of the radio</param>
        /// <param name="channel">Channel of the radio</param>
        public RadioConfiguration(TxPowerValue power, Channels channel)
        {
            this.Channel = channel;
            this.TxPower = power;
        }

        /// <summary>
        /// Set the channel 
        /// </summary>
        /// <param name="channel">Takes Channels enum as input</param>
        public void SetChannel(Channels channel)
        {
            this.Channel = channel;
        }

        /// <summary>
        /// Get the current channel
        /// </summary>
        /// <returns>Channels enum</returns>
        public Channels GetChannel()
        {
            return Channel;
        }

        /// <summary>
        /// Set the transmission power of the radio
        /// </summary>
        /// <param name="power">Takes TxPowerValue enum as input</param>
        public void SetTxPower(TxPowerValue power)
        {
            this.TxPower = power;
        }

        /// <summary>
        /// Return the current transmission power
        /// </summary>
        /// <returns></returns>
        public TxPowerValue GetTxPower()
        {
            return TxPower;
        }

    };


    /// <summary>
    /// Radio interface
    /// </summary>
    public interface IRadio
    {
        //DeviceStatus Initialize(RadioConfiguration config, ReceiveCallBack callback); //Initializes Return the ID of the Radio layer that was initialized
        //DeviceStatus Configure(RadioConfiguration config, ReceiveCallBack callback);  //Change configuration after initialization
        DeviceStatus UnInitialize();
        DeviceStatus ReConfigure(RadioConfiguration config);
        byte GetID();
        UInt16 GetAddress();
        bool SetAddress(UInt16 Address);
        DeviceStatus TurnOn();
        DeviceStatus Sleep(byte level);
        NetOpStatus PreLoad(byte[] message, UInt16 size);
        NetOpStatus SendStrobe();	//Send preloaded message
        NetOpStatus Send(byte[] message, UInt16 size);
        NetOpStatus SendTimeStamped(byte[] message, UInt16 size, UInt32 eventTime);
        bool ClearChannelAssesment();
        bool ClearChannelAssesment(UInt16 numberOfMicroSecond);
        //UInt32 GetSNR();
        //UInt32 GetRSSI();

    }
}
