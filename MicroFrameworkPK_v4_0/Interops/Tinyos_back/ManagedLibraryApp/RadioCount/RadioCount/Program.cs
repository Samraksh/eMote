using System;
using Microsoft.SPOT;
using TinyOS;

namespace RadioCount
{
    public class Program
    {
        public static void Main()
        {
            TinyosInterface tinyosHandler = new TinyosInterface();

            Byte[] inputData = new Byte[1];

            Debug.Print("Calling Tinyos");

            for (int i = 0; i < 10; i++)
            {
                inputData[0] =(byte) i;
                tinyosHandler.execute(inputData);
            }

            Debug.Print("Returning from TinyOS");
        }

    }
}
