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

        const string VERSION = "Benchmark App v0.2 started. Running 7 tests.";

        const int TIMEBASE = 8000000; // in all power modes, 8 MHz is the timebase
        const int TIMEBASE_MS = TIMEBASE / 1000; // to get answer in milli-seconds
        const int ERASE_WINDOW = 14000; // ms
        const int SLEEP_BEFORE_POWERSTATE = 100;

        const string HIGH_POWER = "High Power";
        const string MID_POWER = "Mid Power";
        const string LOW_POWER = "Low Power";
        const string ERROR_POWER = "Unknown Power State, error";

        const string TEST_SEP = "";

        const int TEST_001_SIZE = 10000;
        const int TEST_002_SIZE = 120;
        const int TEST_003_SIZE = 1000;
        const int TEST_006_SIZE = 400;
        const int TEST_006_SEED = 5980;
        const int TEST_007_SIZE = 2500;

        private static uint PRE_TEST(PowerLevel pl)
        {
            PowerState.ChangePowerLevel(pl);
            Debug.GC(true);
            GC.WaitForPendingFinalizers();
            return Debug.GC(true);
        }

        private static void POST_TEST(string desc01, PowerLevel pl, uint freeMem, uint usedMem, int size, long diffms)
        {
            string desc02 = "\t" + power_to_string(pl);
            string desc03 = "\t" + freeMem + " bytes free memory pre-test";
            string desc04 = "\tsize = " + size;
            string memory_used = "\tused memory (bytes): " + usedMem;
            string result = "\tresult: " + diffms + " ms";
            Debug.Print(TEST_SEP);
            Debug.Print(desc01);
            Debug.Print(desc02);
            Debug.Print(desc03);
            Debug.Print(desc04);
            Debug.Print(memory_used);
            Debug.Print(result);
        }

        private static string power_to_string(PowerLevel pl)
        {
            if (pl.Equals(PowerLevel.High))
                return HIGH_POWER;
            if (pl.Equals(PowerLevel.Medium))
                return MID_POWER;
            if (pl.Equals(PowerLevel.Low))
                return LOW_POWER;

            return ERROR_POWER; // Should not be possible.
        }

        private static void TEST_001(PowerLevel pl = PowerLevel.High, int size = TEST_001_SIZE)
        {
            uint freeMem = PRE_TEST(pl);
            long start = DateTime.Now.Ticks;
            // START TEST CODE

            for (int i = 0; i < size; i++) { }
            for (int i = 0; i < size; i++) { }
            for (int i = 0; i < size; i++) { }
            for (int i = 0; i < size; i++) { }

            // END TEST CODE
            long diff = DateTime.Now.Ticks - start;
            long diffms = diff / TIMEBASE_MS;
            uint usedMem = Debug.GC(false);
            usedMem = (usedMem <= freeMem) ? freeMem - usedMem : 0;

            // RESULT STRINGS
            string desc = "Test 001: Busy wait loops";
            POST_TEST(desc, pl, freeMem, usedMem, size, diffms);
        }

        private static void TEST_002(PowerLevel pl = PowerLevel.High, int size = TEST_002_SIZE)
        {
            uint freeMem = PRE_TEST(pl);
            long start = DateTime.Now.Ticks;
            // START TEST CODE

            char c = 'a';
            ArrayList al = new ArrayList();
            for (int i = 0; i < size; i++)
            {
                string ss = c.ToString();
                string s = i + ss;
                al.Add(s);
                c++;
            }
            // Remove every 3rd item
            for (int i = 0; i < size; i += 3)
            {
                al.Remove(i);
            }

            // END TEST CODE
            long diff = DateTime.Now.Ticks - start;
            long diffms = diff / TIMEBASE_MS;
            uint usedMem = Debug.GC(false);
            usedMem = (usedMem <= freeMem) ? freeMem - usedMem : 0;

            // RESULT STRINGS
            string desc = "Test 002: ArrayList and strings";
            POST_TEST(desc, pl, freeMem, usedMem, size, diffms);
        }

        private static void TEST_003(PowerLevel pl = PowerLevel.High, int size = TEST_003_SIZE)
        {
            uint freeMem = PRE_TEST(pl);
            long start = DateTime.Now.Ticks;
            // START TEST CODE

            StringBuilder s = new StringBuilder();

            for (int i = 0; i < size; i++)
            {
                s.Append(i);
            }

            // END TEST CODE
            long diff = DateTime.Now.Ticks - start;
            long diffms = diff / TIMEBASE_MS;
            uint usedMem = Debug.GC(false);
            usedMem = (usedMem <= freeMem) ? freeMem - usedMem : 0;

            // RESULT STRINGS
            string desc = "Test 003: Giant String (StringBuilder)";
            POST_TEST(desc, pl, freeMem, usedMem, size, diffms);
        }

        private static void TEST_004(PowerLevel pl = PowerLevel.High, int size = 0)
        {
            uint freeMem = PRE_TEST(pl);
            long start = DateTime.Now.Ticks;
            // START TEST CODE



            // END TEST CODE
            long diff = DateTime.Now.Ticks - start;
            long diffms = diff / TIMEBASE_MS;
            uint usedMem = Debug.GC(false);
            usedMem = (usedMem <= freeMem) ? freeMem - usedMem : 0;

            // RESULT STRINGS
            string desc = "Test 004: NYI: RegEx";
            POST_TEST(desc, pl, freeMem, usedMem, size, diffms);
        }

        private static void TEST_005(PowerLevel pl = PowerLevel.High, int size = 0)
        {
            uint freeMem = PRE_TEST(pl);
            long start = DateTime.Now.Ticks;
            // START TEST CODE



            // END TEST CODE
            long diff = DateTime.Now.Ticks - start;
            long diffms = diff / TIMEBASE_MS;
            uint usedMem = Debug.GC(false);
            usedMem = (usedMem <= freeMem) ? freeMem - usedMem : 0;

            // RESULT STRINGS
            string desc = "Test 005: NYI: Heap Workout";
            POST_TEST(desc, pl, freeMem, usedMem, size, diffms);
        }

        private static void TEST_006(PowerLevel pl = PowerLevel.High, int size = TEST_006_SIZE)
        {
            uint freeMem = PRE_TEST(pl);
            long start = DateTime.Now.Ticks;
            // START TEST CODE

            ArrayList al = new ArrayList();
            Random rand = new Random(TEST_006_SEED);
            al.Add(rand.Next());

            // Setup the array
            for (int i = 1; i < size; i++)
            {
                al.Add((rand.Next() + (int)al[i - 1]) % 1037575);
            }

            // END TEST CODE
            long diff = DateTime.Now.Ticks - start;
            long diffms = diff / TIMEBASE_MS;
            uint usedMem = Debug.GC(false);
            usedMem = (usedMem <= freeMem) ? freeMem - usedMem : 0;

            // RESULT STRINGS
            string desc = "Test 006: Random() and ArrayList()";
            POST_TEST(desc, pl, freeMem, usedMem, size, diffms);
        }

        public class TEST_007
        {
            private PowerLevel pl;
            private int size;
            private AutoResetEvent ev;

            public TEST_007(PowerLevel pl, int size)
            {
                this.pl = pl;
                this.size = size;
                ev = new AutoResetEvent(false);
            }

            public void doTest()
            {
                uint freeMem = PRE_TEST(pl);
                long start = DateTime.Now.Ticks;

                // START TEST CODE
                OutputPort pinout = new OutputPort(Cpu.Pin.GPIO_Pin0, false);
                InterruptPort pinint = new InterruptPort(Cpu.Pin.GPIO_Pin1, false, Port.ResistorMode.PullDown, Port.InterruptMode.InterruptEdgeHigh);
                pinint.OnInterrupt += gpioInt;
                bool isWired = false;

                // Make sure the GPIO is attached.

                {
                    pinout.Write(true);
                    ev.WaitOne(100, true);
                    isWired = pinint.Read();
                    pinout.Write(false);
                }

                if (isWired)
                {
                    for (int i = 1; i < size; i++)
                    {
                        pinout.Write(true);
                        ev.WaitOne();
                        pinout.Write(false);
                    }
                }
                // END TEST CODE

                long diff = DateTime.Now.Ticks - start;
                long diffms = diff / TIMEBASE_MS;
                uint usedMem = Debug.GC(false);
                usedMem = (usedMem <= freeMem) ? freeMem - usedMem : 0;

                // RESULT STRINGS
                string desc = "Test 007: GPIO + interrupts";
                if (!isWired) { desc = desc + "\r\n\tTest Invalid, pin 0+1 not wired"; diffms = 0; }
                POST_TEST(desc, pl, freeMem, usedMem, size, diffms);
            }

            private void gpioInt(uint data1, uint data2, DateTime time)
            {
                ev.Set();
            }

        }

        private static void TEST_007_HELPER(PowerLevel pl = PowerLevel.High, int size = TEST_007_SIZE)
        {
            TEST_007 test = new TEST_007(pl, size);
            test.doTest();
        }

        public static void Main()
        {
            Thread.Sleep(ERASE_WINDOW);
            //Debug.EnableGCMessages(true);
            Debug.Print(VERSION);

            TEST_001(PowerLevel.High);
            TEST_001(PowerLevel.Medium);
            TEST_001(PowerLevel.Low);

            TEST_002(PowerLevel.High);
            TEST_002(PowerLevel.Medium);
            TEST_002(PowerLevel.Low);

            TEST_003(PowerLevel.High);
            TEST_003(PowerLevel.Medium);
            TEST_003(PowerLevel.Low);

            //TEST_004();
            //TEST_005();

            TEST_006(PowerLevel.High);
            TEST_006(PowerLevel.Medium);
            TEST_006(PowerLevel.Low);

            TEST_007_HELPER(PowerLevel.High);
            TEST_007_HELPER(PowerLevel.Medium);
            TEST_007_HELPER(PowerLevel.Low);

            PowerState.ChangePowerLevel(PowerLevel.High);
            Debug.Print(TEST_SEP);
            Debug.Print("All tests completed");
            Thread.Sleep(Timeout.Infinite);
        }
    }
}
