using System;
using Microsoft.SPOT;

namespace Samraksh.eMote.Net
{
    /// <summary>
    /// Exception thrown when the radio is configured incorrectly
    /// </summary>
    public class RadioConfigurationMismatchException : System.Exception
    {
        public RadioConfigurationMismatchException() { Debug.Print("Mismatch between initialized radio object and configuration passed\n"); }
    }


    public class RadioNotConfiguredException : System.Exception
    {
        public RadioNotConfiguredException() { Debug.Print("The radio has not been configured with a config object or a callback\n"); }
        public RadioNotConfiguredException(string message) : base(message) { 
            //Debug.Print(message); 
        }
    }

    public class RadioBusyException : System.Exception
    {
        public RadioBusyException() { Debug.Print("The radio already has a user\n"); }
    }

    public class MacNotConfiguredException : System.Exception
    {
        public MacNotConfiguredException() { Debug.Print("You are attempting to acquire an instance of the mac without configuring it\n"); }
        public MacNotConfiguredException(string message) : base(message) { }
        public MacNotConfiguredException(string message, Exception innerException) : base(message, innerException) { }
    }

    public class CallbackNotConfiguredException : System.Exception
    {
        public CallbackNotConfiguredException() { Debug.Print("Receive callback has not been configured \n"); }
    }
}
