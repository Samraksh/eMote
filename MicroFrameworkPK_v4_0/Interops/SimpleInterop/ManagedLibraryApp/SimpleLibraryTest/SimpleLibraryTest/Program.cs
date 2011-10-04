using System;
using Microsoft.SPOT;
using SimpleInterop;


namespace SimpleLibraryTest
{
    public class Program
    {
        public static void Main()
        {
            SimpleInterop.Library si = new SimpleInterop.Library();

            si.init();

            while (true)
            {
                si.execute();
            }
            
        }

    }
}
