using System;
using Microsoft.SPOT;
using Samraksh.SPOT.Net;

namespace Samraksh.SPOT.Net.Radio
{
    public enum RadioUser
    {
        CSharp,
        CSMAMAC,
        OMAC,
    };


    /// <summary>
    /// Radio configuration
    /// </summary>
    public class RadioConfiguration
    {
        public int TxPower;
        public int Channel;

        public RadioConfiguration()
        {
            TxPower = (int) TxPowerValue.Power_3dBm;
            Channel = (int)Channels.Channel_26;
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
