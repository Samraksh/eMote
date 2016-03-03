using System;
using Microsoft.SPOT;

namespace Samraksh.eMote.Net.Radio
{
    /// <summary>Generic radio object</summary>
    public class Radio_802_15_4 : Radio_802_15_4_Base
    {
        private static bool GenericRadioInstanceSet = false;

        /// <summary>
        /// 
        /// </summary>
        public Radio_802_15_4()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="user"></param>
        public Radio_802_15_4(RadioUser user)
        {
            if (!GenericRadioInstanceSet)
            {
                GenericRadioInstanceSet = true;
                if (user == RadioUser.CSMAMAC)
                {
                    Radio_802_15_4_Base.CurrUser = RadioUser.CSMAMAC;
                }
                else if (user == RadioUser.OMAC)
                {
                    Radio_802_15_4_Base.CurrUser = RadioUser.OMAC;
                }
                else if (user == RadioUser.CSharp)
                {
                    Radio_802_15_4_Base.CurrUser = RadioUser.CSharp;
                }
            }
            else
                Debug.Print("Generic radio already initialized");
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="drvName"></param>
        /// <param name="drvdata"></param>
        public Radio_802_15_4(string drvName, ulong drvdata)
            : base(drvName, drvdata)
        {
        }

    }
}
