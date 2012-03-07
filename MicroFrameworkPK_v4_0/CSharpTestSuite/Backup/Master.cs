using System;
using Microsoft.SPOT;
using SchedulerTestSuite;

namespace SchedulerTestSuite
{
    class Master
    {
        public static void Main()
        {
           //ThreadPriorityTest test = new ThreadPriorityTest();
           RTOSTimerTest rtosTest = new RTOSTimerTest();
           // AsyncTimerTest test = new AsyncTimerTest();
           rtosTest.Run();
        }
    }
}
