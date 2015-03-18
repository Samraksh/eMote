using System;
using Microsoft.SPOT;

namespace Samraksh.eMote.Net.Radio
{
    /// <summary>Generic radio object</summary>
    public class Radio_802_15_4 : Radio_802_15_4_Base
    {
        private static Radio_802_15_4 instance;
        private static object syncObject = new Object();

        private Radio_802_15_4()
        {
        }

        private Radio_802_15_4(string drvName, ulong drvdata)
            : base(drvName, drvdata)
        {
        }

        /// <summary>Get the radio instance</summary>
        /// <returns>Radio instance</returns>
        /// <exception caption="RadioNotConfigured Exception" cref="O:Samraksh.eMote.Net.RadioNotConfiguredException._ctor"></exception>
        /// <exception caption="System Exception" cref="System.SystemException"></exception>
        public static Radio_802_15_4 GetInstance()
        {

            if (Config == null)
            {
                throw new Samraksh.eMote.Net.RadioNotConfiguredException();
            }

            if (Config.GetRadioName() != RadioName.RF231RADIO)
            {
                throw new SystemException("Mismatch between radio object initialized and configuration passed\n");
            }

            if (instance == null)
            {
                lock (syncObject)
                {
                    if (instance == null)
                        instance = new Radio_802_15_4();
                }

                if (instance != null)
                    instance.SetRadioName(RadioName.RF231RADIO);

            }

            return instance;
        }



        /// <summary>Get the shallow radio instance</summary>
        /// <param name="user">Shallow radio instance</param>
        /// <exception caption="RadioNotConfigured Exception" cref="O:Samraksh.eMote.Net.RadioNotConfiguredException._ctor"></exception>
        /// <exception caption="System Exception" cref="System.SystemException"></exception>
        public static Radio_802_15_4 GetShallowInstance(RadioUser user)
        {
            if (Config == null)
            {
                throw new Samraksh.eMote.Net.RadioNotConfiguredException();
            }

            if (Config.GetRadioName() != RadioName.RF231RADIO)
            {
                throw new SystemException("Mismatch between radio object initialized and configuration passed\n");
            }

            if (instance == null)
            {
                lock (syncObject)
                {
                    if (instance == null)
                    {
                        if (user == RadioUser.CSMAMAC)
                            instance = new Radio_802_15_4("CSMACallback", 4321);
                        else if (user == RadioUser.OMAC)
                            instance = new Radio_802_15_4("OMACCallback", 4322);
                        else if (user == RadioUser.CSharp)
                            instance = new Radio_802_15_4();
                    }
                }

                if (instance != null)
                    instance.SetRadioName(RadioName.RF231RADIO);
            }

            return instance;
        }
    }
}
