using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;

namespace MemProfiling
{
    public class Program
    {
        static OutputPort Output = new OutputPort((Cpu.Pin)30, true);
        static OutputPort Error = new OutputPort((Cpu.Pin)31, true);

        public static void Main()
        {
            byte k = 0;
            while (true)
            {
                Output.Write(true);

                for (byte i = 0; i < 255; i++)
                {
                    k = i;
                }

                Output.Write(false);
            }
        }

    }
}
