using System;
using Microsoft.SPOT;

namespace Samraksh.eMote.Net
{
    /// <summary>
    /// Exception thrown when the radio is configured incorrectly
    /// </summary>
    public class RadioConfigurationMismatchException : System.Exception
    {
        /// <summary>
        /// 
        /// </summary>
        public RadioConfigurationMismatchException() { Debug.Print("Mismatch between initialized radio object and configuration passed\n"); }
    }

    /// <summary>
    /// 
    /// </summary>
    public class RadioNotConfiguredException : System.Exception
    {
        /// <summary>
        /// 
        /// </summary>
        public RadioNotConfiguredException() { Debug.Print("The radio has not been configured with a config object or a callback\n"); }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="message"></param>
        public RadioNotConfiguredException(string message) : base(message) { 
            //Debug.Print(message); 
        }
    }

    /// <summary>
    /// 
    /// </summary>
    public class RadioBusyException : System.Exception
    {
        /// <summary>
        /// 
        /// </summary>
        public RadioBusyException() { Debug.Print("The radio already has a user\n"); }
    }

    /// <summary>
    /// 
    /// </summary>
    public class MacNotConfiguredException : System.Exception
    {
        /// <summary>
        /// 
        /// </summary>
        public MacNotConfiguredException() { Debug.Print("You are attempting to acquire an instance of the mac without configuring it\n"); }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="message"></param>
        public MacNotConfiguredException(string message) : base(message) { }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="message"></param>
        /// <param name="innerException"></param>
        public MacNotConfiguredException(string message, Exception innerException) : base(message, innerException) { }
    }

    /// <summary>
    /// 
    /// </summary>
    public class CallbackNotConfiguredException : System.Exception
    {
        /// <summary>
        /// 
        /// </summary>
        public CallbackNotConfiguredException() { Debug.Print("Receive callback has not been configured \n"); }
    }
}
