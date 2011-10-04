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

            while(true)
            {
                inputData[0] =(byte) 10;
                tinyosHandler.execute(inputData);
            }

            //Debug.Print("Returning from TinyOS");
        }

    }
}
