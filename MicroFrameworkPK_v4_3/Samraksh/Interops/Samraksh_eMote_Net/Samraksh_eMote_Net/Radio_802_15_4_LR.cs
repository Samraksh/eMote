using System;
using Microsoft.SPOT;

namespace Samraksh.eMote.Net.Radio
{
    /// <summary>
    /// Class represents the long range radio object, maps to the long range instance in native
    /// </summary>
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

        public static Radio_802_15_4_LR GetInstance()
        {
            if (config == null)
            {
                throw new Samraksh.eMote.Net.RadioNotConfiguredException();
            }

            if (config.GetRadioName() != RadioName.RF231RADIOLR)
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



        public static Radio_802_15_4_LR GetShallowInstance(RadioUser user)
        {

            if (config == null)
            {
                throw new Samraksh.eMote.Net.RadioNotConfiguredException();
            }

            if (config.GetRadioName() != RadioName.RF231RADIOLR)
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
