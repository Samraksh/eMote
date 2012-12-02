using System;
using Microsoft.SPOT;
using System.Threading;

namespace HelloWorld
{
    public class Program
    {
        public static void Main()
        {
            while (true)
            {
                Debug.Print("Hello World");
                Thread.Sleep(1000);
            }
        }

    }
}
