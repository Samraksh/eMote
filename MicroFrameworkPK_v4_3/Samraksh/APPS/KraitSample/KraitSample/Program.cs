using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace KraitSample
{
    public class Program
    {
        private static OutputPort testPort1 = new OutputPort((Cpu.Pin) 51, true);
        private static OutputPort testPort2 = new OutputPort((Cpu.Pin) 58, true);

        public static void Main()
        {
            while (true)
            {
                testPort1.Write(true);
                for (int i = 0; i < 10000; i++) ;
                testPort1.Write(false);

                testPort2.Write(true);
                for (int i = 0; i < 10000; i++) ;
                testPort2.Write(false);

            }
        }

    }
}
