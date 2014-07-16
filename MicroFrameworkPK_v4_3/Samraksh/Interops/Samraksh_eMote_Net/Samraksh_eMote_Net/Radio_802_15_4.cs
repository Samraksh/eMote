using System;
using Microsoft.SPOT;

namespace Samraksh.eMote.Net.Radio
{
    /// <summary>
    /// Class represents the generic radio object 
    /// </summary>
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

        public static Radio_802_15_4 GetInstance()
        {

            if (config == null)
            {
                throw new Samraksh.eMote.Net.RadioNotConfiguredException();
            }

            if (config.GetRadioName() != RadioName.RF231RADIO)
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



        public static Radio_802_15_4 GetShallowInstance(RadioUser user)
        {
            if (config == null)
            {
                throw new Samraksh.eMote.Net.RadioNotConfiguredException();
            }

            if (config.GetRadioName() != RadioName.RF231RADIO)
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
