using System;
using Microsoft.SPOT;

namespace Samraksh.SPOT.Net
{
    public class RadioNotConfiguredException : System.Exception
    {
        public RadioNotConfiguredException() { Debug.Print("The Radio has not been configured with a config object or a callback"); }
        public RadioNotConfiguredException(string message) { Debug.Print(message); }
    }
}
