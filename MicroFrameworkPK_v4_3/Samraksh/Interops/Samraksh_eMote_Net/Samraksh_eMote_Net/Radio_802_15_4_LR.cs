using System;
using Microsoft.SPOT;

namespace Samraksh.eMote.Net.Radio
{
    /// <summary>Long range radio</summary>
    public class Radio_802_15_4_LR : Radio_802_15_4_Base
    {
        private static Radio_802_15_4_LR LRRadioInstance;
        private static object syncObject = new Object();

        private Radio_802_15_4_LR()
        {
        }
        
        private Radio_802_15_4_LR(string drvName, ulong drvdata)
            : base(drvName, drvdata)
        {
        }

        /// <summary>Get RadioInstance</summary>
        /// <returns>Instance</returns>
        /// <exception caption="RadioNotConfigured Exception" cref="O:Samraksh.eMote.Net.RadioNotConfiguredException._ctor"></exception>
        /// <exception caption="System Exception" cref="System.SystemException"></exception>
        public static Radio_802_15_4_LR GetInstance()
        {
            if (RadioBaseConfig == null)
            {
                throw new Samraksh.eMote.Net.RadioNotConfiguredException();
            }

            if (RadioBaseConfig.RadioType != RadioType.RF231RADIOLR)
            {
                throw new SystemException("Mismatch between radio object initialized and configuration passed\n");
            }

            if (LRRadioInstance == null)
            {
                lock (syncObject)
                {
                    if (LRRadioInstance == null)
                        LRRadioInstance = new Radio_802_15_4_LR();
                }
            }

            //if (RadioInstance != null)
                //RadioInstance.RadioName = RadioName.RF231RADIOLR;

            return LRRadioInstance;
        }



        /// <summary>Get radio instance</summary>
        /// <param name="user">Radio user</param>
        /// <exception caption="RadioNotConfigured Exception" cref="O:Samraksh.eMote.Net.RadioNotConfiguredException._ctor"></exception>
        /// <exception caption="System Exception" cref="System.SystemException"></exception>
        public static Radio_802_15_4_LR GetInstance(RadioUser user)
        {
            if (RadioBaseConfig == null)
            {
                throw new Samraksh.eMote.Net.RadioNotConfiguredException();
            }

            if (RadioBaseConfig.RadioType != RadioType.RF231RADIOLR)
            {
                throw new SystemException("Mismatch between radio object initialized and configuration passed");
            }

            if (LRRadioInstance == null)
            {
                lock (syncObject)
                {
                    if (LRRadioInstance == null)
                    {
                        if (user == RadioUser.CSMAMAC)
                            LRRadioInstance = new Radio_802_15_4_LR("CSMACallback", 4321);
                        else if (user == RadioUser.OMAC)
                            LRRadioInstance = new Radio_802_15_4_LR("OMACCallback", 4322);
                        else if (user == RadioUser.CSharp)
                            LRRadioInstance = new Radio_802_15_4_LR();
                    }
                }

                //if (RadioInstance != null)
                    //instance.RadioName = RadioName.RF231RADIOLR;
            }

            return LRRadioInstance;
        }
    }
}
