using System;
using Microsoft.SPOT;

namespace Samraksh.eMote.Net.Radio
{
    /// <summary>Long range radio</summary>
    public class Radio_802_15_4_LR : Radio_802_15_4_Base
    {

        private static Radio_802_15_4_LR instance;
        private static object syncObject = new Object();


        private Radio_802_15_4_LR()
        {

        }

                
        private Radio_802_15_4_LR(string drvName, ulong drvdata)
            : base(drvName, drvdata)
        {
        }

        /// <summary>Get instance</summary>
        /// <returns>Instance</returns>
        /// <exception caption="RadioNotConfigured Exception" cref="O:Samraksh.eMote.Net.RadioNotConfiguredException._ctor"></exception>
        /// <exception caption="System Exception" cref="System.SystemException"></exception>
        public static Radio_802_15_4_LR GetInstance()
        {
            if (Config == null)
            {
                throw new Samraksh.eMote.Net.RadioNotConfiguredException();
            }

            if (Config.GetRadioName() != RadioName.RF231RADIOLR)
            {
                throw new SystemException("Mismatch between radio object initialized and configuration passed\n");
            }

            if (instance == null)
            {
                lock (syncObject)
                {
                    if (instance == null)
                        instance = new Radio_802_15_4_LR();
                }
            }

            if (instance != null)
                instance.SetRadioName(RadioName.RF231RADIOLR);

            return instance;
        }



        /// <summary>Get shallow instance</summary>
        /// <param name="user">Shallow instance</param>
        /// <exception caption="RadioNotConfigured Exception" cref="O:Samraksh.eMote.Net.RadioNotConfiguredException._ctor"></exception>
        /// <exception caption="System Exception" cref="System.SystemException"></exception>
        public static Radio_802_15_4_LR GetShallowInstance(RadioUser user)
        {

            if (Config == null)
            {
                throw new Samraksh.eMote.Net.RadioNotConfiguredException();
            }

            if (Config.GetRadioName() != RadioName.RF231RADIOLR)
            {
                throw new SystemException("Mismatch between radio object initialized and configuration passed");
            }

            if (instance == null)
            {
                lock (syncObject)
                {
                    if (instance == null)
                    {
                        if (user == RadioUser.CSMAMAC)
                            instance = new Radio_802_15_4_LR("CSMACallback", 4321);
                        else if (user == RadioUser.OMAC)
                            instance = new Radio_802_15_4_LR("OMACCallback", 4322);
                        else if (user == RadioUser.CSharp)
                            instance = new Radio_802_15_4_LR();
                    }
                }

                if (instance != null)
                    instance.SetRadioName(RadioName.RF231RADIOLR);
            }

            return instance;
        }
    }


    
}
