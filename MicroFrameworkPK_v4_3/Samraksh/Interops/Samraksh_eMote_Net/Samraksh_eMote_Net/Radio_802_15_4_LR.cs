using System;
using Microsoft.SPOT;

namespace Samraksh.eMote.Net.Radio
{
    /// <summary>Long range radio</summary>
    public class Radio_802_15_4_LR : Radio_802_15_4_Base
    {
        private static bool LRRadioInstanceSet = false;
        
        /// <summary>
        /// 
        /// </summary>
        public Radio_802_15_4_LR()
        {
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="user"></param>
        public Radio_802_15_4_LR(RadioUser user)
        {
            if (!LRRadioInstanceSet)
            {
                LRRadioInstanceSet = true;
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
                Debug.Print("Long range radio already initialized");
        }
        
        /// <summary>
        /// 
        /// </summary>
        /// <param name="drvName"></param>
        /// <param name="drvdata"></param>
        public Radio_802_15_4_LR(string drvName, ulong drvdata)
            : base(drvName, drvdata)
        {
        }

    }
}
