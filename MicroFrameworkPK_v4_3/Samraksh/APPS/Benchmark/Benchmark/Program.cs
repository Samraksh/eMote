using System;
//using System.Diagnostics;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using System.Threading;
using System.Collections;
using System.Text;
//using Samraksh.eMote.DotNow;

namespace Benchmark
{
    public class Program
    {

        const int TIMEBASE = 8000000; // in all power modes, 8 MHz is the timebase
        const int TIMEBASE_MS = TIMEBASE / 1000; // to get answer in milli-seconds
        const int ERASE_WINDOW = 14000; // ms
        const int SLEEP_BEFORE_POWERSTATE = 100;

        const int TEST_001_ITER = 10000;
        const int TEST_002_ITER = 120;
        const int TEST_003_SIZE = 1000;

        public static void Main()
        {
            Thread.Sleep(ERASE_WINDOW);
            UInt32 freeMem;
            //Debug.EnableGCMessages(true);
            Debug.Print("Benchmark App v0.1 started");
            Debug.Print("Test 001: 4 Busy wait loops");
            {
                Debug.GC(true);
                GC.WaitForPendingFinalizers();
                freeMem = Debug.GC(true);
                PowerState.ChangePowerLevel(PowerLevel.High);
                long start = DateTime.Now.Ticks;
                for (int i = 0; i < TEST_001_ITER; i++) { }
                for (int i = 0; i < TEST_001_ITER; i++) { }
                for (int i = 0; i < TEST_001_ITER; i++) { }
                for (int i = 0; i < TEST_001_ITER; i++) { }
                long diff = DateTime.Now.Ticks - start;
                long diffms = diff / TIMEBASE_MS;
                string result = "\tresult " + diffms + " ms, freeMem = " + freeMem;
                Debug.Print(result);
            }

            Debug.Print("Test 001a: 4 Busy wait loops, low power");
            {
                Debug.GC(true);
                GC.WaitForPendingFinalizers();
                freeMem = Debug.GC(true);
                Thread.Sleep(SLEEP_BEFORE_POWERSTATE);
                PowerState.ChangePowerLevel(PowerLevel.Low);

                long start = DateTime.Now.Ticks;
                for (int i = 0; i < TEST_001_ITER; i++) { }
                for (int i = 0; i < TEST_001_ITER; i++) { }
                for (int i = 0; i < TEST_001_ITER; i++) { }
                for (int i = 0; i < TEST_001_ITER; i++) { }
                long diff = DateTime.Now.Ticks - start;
                long diffms = diff / TIMEBASE_MS;
                string result = "\tresult " + diffms + " ms, freeMem = " + freeMem;
                Debug.Print(result);
                Thread.Sleep(SLEEP_BEFORE_POWERSTATE);
                PowerState.ChangePowerLevel(PowerLevel.High);
            }

            Debug.Print("Test 002: ArrayList");
            {
                Debug.GC(true);
                GC.WaitForPendingFinalizers();
                freeMem = Debug.GC(true);
                long start = DateTime.Now.Ticks;

                char c = 'a';
                ArrayList al = new ArrayList();
                for (int i = 0; i < TEST_002_ITER; i++)
                {
                    string ss = c.ToString();
                    string s = i + ss;
                    al.Add(s);
                    c++;
                }
                // Remove every 3rd item
                for (int i = 0; i < TEST_002_ITER; i += 3)
                {
                    al.Remove(i);
                }

                long diff = DateTime.Now.Ticks - start;
                long diffms = diff / TIMEBASE_MS;
                string result = "\tresult " + diffms + " ms, freeMem = " + freeMem;
                Debug.Print(result);
            }

            Debug.Print("Test 002a: ArrayList, low power");
            {
                Debug.GC(true);
                GC.WaitForPendingFinalizers();
                freeMem = Debug.GC(true);
                Thread.Sleep(SLEEP_BEFORE_POWERSTATE);
                PowerState.ChangePowerLevel(PowerLevel.Low);
                long start = DateTime.Now.Ticks;

                char c = 'a';
                ArrayList al = new ArrayList();
                for (int i = 0; i < TEST_002_ITER; i++)
                {
                    string ss = c.ToString();
                    string s = i + ss;
                    al.Add(s);
                    c++;
                }
                // Remove every 3rd item
                for (int i = 0; i < TEST_002_ITER; i += 3)
                {
                    al.Remove(i);
                }

                long diff = DateTime.Now.Ticks - start;
                Thread.Sleep(SLEEP_BEFORE_POWERSTATE);
                PowerState.ChangePowerLevel(PowerLevel.High);
                long diffms = diff / TIMEBASE_MS;
                string result = "\tresult " + diffms + " ms, freeMem = " + freeMem;
                Debug.Print(result);
            }

            Debug.Print("Test 003: Giant String");
            {
                Debug.GC(true);
                GC.WaitForPendingFinalizers();
                freeMem = Debug.GC(true);

                long start = DateTime.Now.Ticks;

                StringBuilder s = new StringBuilder();

                for (int i = 0; i < TEST_003_SIZE; i++)
                {
                    s.Append(i);
                }

                long diff = DateTime.Now.Ticks - start;
                long diffms = diff / TIMEBASE_MS;
                string result = "\tresult " + diffms + " ms, freeMem = " + freeMem;
                Debug.Print(result);
            }

            Debug.Print("Test 004: RegEx");
            {
                Debug.GC(true);
                GC.WaitForPendingFinalizers();
                freeMem = Debug.GC(true);

                long start = DateTime.Now.Ticks;

                long diff = DateTime.Now.Ticks - start;
                long diffms = diff / TIMEBASE_MS;
                //string result = "\tresult " + diffms + " ms, freeMem = " + freeMem;
                string result = "\tNot Yet Implemented... freeMem = " + freeMem;
                Debug.Print(result);
            }

            Debug.Print("Test 005: Heap Workout");
            {
                Debug.GC(true);
                GC.WaitForPendingFinalizers();
                freeMem = Debug.GC(true);

                long start = DateTime.Now.Ticks;

                long diff = DateTime.Now.Ticks - start;
                long diffms = diff / TIMEBASE_MS;
                //string result = "\tresult " + diffms + " ms, freeMem = " + freeMem;
                string result = "\tNot Yet Implemented... freeMem = " + freeMem;
                Debug.Print(result);
            }


            Debug.Print("All tests completed");
            Thread.Sleep(Timeout.Infinite);
        }
    }
}
