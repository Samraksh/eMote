using Microsoft.SPOT;

// ReSharper disable once CheckNamespace
namespace Samraksh.eMote.Net.Radio
{
    /// <summary>Generic radio object</summary>
    // ReSharper disable once InconsistentNaming
    public class Radio_802_15_4_RF231 : Radio_802_15_4_Base
    {
        private static bool _genericRadioInstanceSet;

        /// <summary>Constructor</summary>
        public Radio_802_15_4_RF231()
        {
        }

        /// <summary>Constructor</summary>
        /// <param name="user"></param>
        public Radio_802_15_4_RF231(RadioUser user)
        {
            if (!_genericRadioInstanceSet)
            {
                _genericRadioInstanceSet = true;
                if (user == RadioUser.CSMA)
                {
                    CurrUser = RadioUser.CSMA;
                }
                else if (user == RadioUser.OMAC)
                {
                    CurrUser = RadioUser.OMAC;
                }
                else if (user == RadioUser.CSharp)
                {
                    CurrUser = RadioUser.CSharp;
                }
            }
            else
                Debug.Print("Generic radio already initialized");
        }

        /// <summary>
        /// 
        /// </summary>
        ~Radio_802_15_4_RF231()
        {
            _genericRadioInstanceSet = false;
            CurrUser = RadioUser.Idle;
            Debug.Print("Generic radio uninitialized");
        }

    }
}
