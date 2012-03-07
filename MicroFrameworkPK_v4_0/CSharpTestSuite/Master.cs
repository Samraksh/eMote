using System;
using Microsoft.SPOT;
using CSharpTestSuite;

namespace CSharpTestSuite
{
    class Master
    {
        public static void Main()
        {
           //ThreadPriorityTest threadTest = new ThreadPriorityTest();
           //threadTest.Run();
           RTOSTimerTest rtosTest = new RTOSTimerTest();
           rtosTest.Run();
           //GarbageCollectorTest gcTest = new GarbageCollectorTest();
           //gcTest.Run();
           //USBSpeedTest ut = new USBSpeedTest();
           //ut.Run();
        }
    }
}
