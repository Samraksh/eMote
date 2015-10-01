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

        const string HIGH_POWER = "High Power";
        const string MID_POWER = "Mid Power";
        const string LOW_POWER = "Low Power";
        const string ERROR_POWER = "Unknown Power State, error";

        const string TEST_SEP = "\r\n";

        const int TEST_001_SIZE = 10000;
        const int TEST_002_SIZE = 120;
        const int TEST_003_SIZE = 1000;
        const int TEST_006_SIZE = 400;
        const int TEST_006_SEED = 5980;

        private static uint PRE_TEST(PowerLevel pl)
        {
            PowerState.ChangePowerLevel(pl);
            Debug.GC(true);
            GC.WaitForPendingFinalizers();
            return Debug.GC(true);
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
            string desc01 = "Test 001: 4 Busy wait loops";
            string desc02 = "\t" + power_to_string(pl);
            string desc03 = "\t"+freeMem+" bytes free memory pre-test";
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
            string desc01 = "Test 002: ArrayList and strings";
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
            string desc01 = "Test 003: Giant String (StringBuilder)";
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
            string desc01 = "Test 004: NYI: RegEx";
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
            string desc01 = "Test 005: NYI: Heap Workout";
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
            string desc01 = "Test 006: Random() and ArrayList()";
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

        public static void Main()
        {
            Thread.Sleep(ERASE_WINDOW);
            //Debug.EnableGCMessages(true);
            Debug.Print("Benchmark App v0.1 started");

            TEST_001(PowerLevel.High);
            TEST_001(PowerLevel.Medium);
            TEST_001(PowerLevel.Low);

            TEST_002(PowerLevel.High);
            TEST_002(PowerLevel.Medium);
            TEST_002(PowerLevel.Low);

            TEST_003(PowerLevel.High);
            TEST_003(PowerLevel.Medium);
            TEST_003(PowerLevel.Low);

            TEST_006(PowerLevel.High);
            TEST_006(PowerLevel.Medium);
            TEST_006(PowerLevel.Low);

            TEST_004();
            TEST_005();

            PowerState.ChangePowerLevel(PowerLevel.High);
            Debug.Print(TEST_SEP);
            Debug.Print("All tests completed");
            Thread.Sleep(Timeout.Infinite);
        }
    }
}
