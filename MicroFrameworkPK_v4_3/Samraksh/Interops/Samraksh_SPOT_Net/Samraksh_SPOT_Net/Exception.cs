using System;
using Microsoft.SPOT;

namespace Samraksh.SPOT.Net
{
    /// <summary>
    /// Exception thrown when the radio is configured incorrectly
    /// </summary>
    public class RadioConfigurationMismatchException : System.Exception
    {
        public RadioConfigurationMismatchException() { Debug.Print("Mismatch between radio object initialized and configuration passed\n"); }
    }


    public class RadioNotConfiguredException : System.Exception
    {
        public RadioNotConfiguredException() { Debug.Print("The Radio has not been configured with a config object or a callback\n"); }
        public RadioNotConfiguredException(string message) { Debug.Print(message); }
    }

    public class RadioBusyException : System.Exception
    {
        public RadioBusyException() { Debug.Print("The Radio has a user already\n"); }
    }

    public class MacNotConfiguredException : System.Exception
    {
        public MacNotConfiguredException() { Debug.Print("You are attempting to acquire an instance of the mac without configuring it\n"); }
    }

    public class CallbackNotConfiguredException : System.Exception
    {
        public CallbackNotConfiguredException() { Debug.Print("The Receive Callback has not been configured \n"); }
    }
}
