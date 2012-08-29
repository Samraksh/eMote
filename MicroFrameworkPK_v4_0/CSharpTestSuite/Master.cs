using System;
using Microsoft.SPOT;
using CSharpTestSuite;
using CSharpTestSuite.SchedulerTestSuite;

namespace CSharpTestSuite
{
    class Master
    {
        public static void Main()
        {
        	//TimerTest timerTest = new TimerTest();
            //timerTest.Run();
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
