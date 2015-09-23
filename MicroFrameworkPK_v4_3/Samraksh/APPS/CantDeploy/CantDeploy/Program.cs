using System;
using System.Threading;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;


// Example code that will block deployment.
// Used for testing and trying to fix.
// This one is cheating and a little trivial, but its a start.

namespace CantDeploy
{
    public class Program
    {
        public static void Main()
        {
            // Can only deploy during this pause
            Thread.Sleep(15000);

            Debug.Print("Test. Can you still re-deploy app?");

            // Go to low power mode where TinyCLR is too slow to talk to the PC
            PowerState.ChangePowerLevel(PowerLevel.Low);

            // Done
            Thread.Sleep(Timeout.Infinite);
        }

    }
}
