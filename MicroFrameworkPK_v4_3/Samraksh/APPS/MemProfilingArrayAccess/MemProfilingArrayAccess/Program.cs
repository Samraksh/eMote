using System;
using System.Runtime.CompilerServices;
using Microsoft.SPOT;

namespace MemProfilingArrayAccess
{
    public class CopyTime
    {
        static byte[] b1 = new byte[1000];
        byte[] b2 = new byte[1000];
        static System.DateTime dt1;
        static System.TimeSpan ts0;
        static System.TimeSpan ts1;
        static System.TimeSpan ts2;
        static System.TimeSpan ts3;
        Random r = new Random();

        public CopyTime() {
            RandBuffer();
        }

        public void RandBuffer() {
            r.NextBytes(b1);
        }
        public bool CheckBuffer() {
            for (UInt16 i = 0; i < 1000; i++) {
                if (b1[i] != b2[i]) {
                    return false;
                }
            }
            return true;
        }

        public void CopyArray() {
            b1.CopyTo(b2, 0);
        }

        public void LoadArray() {
            ushort x;
            for (int itr = 0; itr < 1000; ++itr) {
                x = b1[itr];
            }
        }

        public void LoopOverhead() {
            ushort x;  //Match LoadArray().
            for (int itr = 0; itr < 1000; ++itr) {
            }
        }

        public static void Main() {

            CopyTime ct = new CopyTime();
            if (ct.CheckBuffer()) { Debug.Print("WARNING: The buffers before copy are the same"); }
            dt1 = System.DateTime.Now;
            for (UInt16 i = 0; i < 1000; i++) {
                ct.CopyArray();
            }
            ts0 = System.DateTime.Now - dt1;
            if (!ct.CheckBuffer()) { Debug.Print("ERROR: The buffers after copy are not the same"); }

            ct.RandBuffer();
            dt1 = System.DateTime.Now;
            for (UInt16 i = 0; i < 1000; i++) {
                ct.LoadArray();
            }
            ts1 = System.DateTime.Now - dt1;

            dt1 = System.DateTime.Now;
            for (UInt16 i = 0; i < 1000; i++) {
                ct.LoopOverhead();
            }
            ts2 = System.DateTime.Now - dt1;

            Debug.Print("Profile for 1000*1000 UInt16");
            Debug.Print("ArrayCopy:         " + ts0.Minutes + "m:  " + ts0.Seconds + "sec:  " + ts0.Milliseconds + " milliseconds.");
            Debug.Print("Loop+Array Access: " + ts1.Minutes + "m: " + ts1.Seconds + "sec: " + ts1.Milliseconds + " milliseconds.");
            Debug.Print("Loop Overhead:     " + ts2.Minutes + "m: " + ts2.Seconds + "sec: " + ts2.Milliseconds + " milliseconds.");
            ts3 = ts1.Subtract(ts2);
            Debug.Print("Array access:      " + ts3.Minutes + "m: " + ts3.Seconds + "sec: " + ts3.Milliseconds + " milliseconds.");
            Debug.Print("Array access is " + (ts3.Ticks / ts0.Ticks) + " times slower than block copy.");
        }

    }
}
