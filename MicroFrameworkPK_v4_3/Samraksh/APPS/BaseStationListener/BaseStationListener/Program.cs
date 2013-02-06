using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

using _DBG = Microsoft.SPOT.Debugger;
using _WP = Microsoft.SPOT.Debugger.WireProtocol;

namespace BaseStationListener
{
    public class Listener
    {
        Stream stream;
        AsyncCallback callback;
        static Byte[] data = new Byte[128];
        bool bDone;

        public Listener(string com, string port)
        {
            _DBG.PortDefinition pd = null;
            string lport = port;
            uint baudrate = 115200;

            if (com == "serial")
            {
                pd = _DBG.PortDefinition.CreateInstanceForSerial(lport, lport, baudrate);
            }

            if (pd.TryToOpen())
            {
                stream = pd.Open();

                Console.WriteLine("Opening port: " + lport);

            }
            else
            {
                Console.WriteLine("Failed to open port for communication");
            }

        }

        public void ShowText(IAsyncResult result)
        {
            for (int x = 0; x < data.Length; ++x)
            {
                Console.Write(data[x]);
                data[x] = 0;
            }

            Console.Write("\n");

            bDone = true;
        }

        public void Run()
        {
            callback = new AsyncCallback(ShowText);
            int count = 0;
            while (true)
            {
                bDone = false;
                stream.BeginRead(data, 0, data.Length, callback, null);
                ++count;
            }

        }
        


        static void Main(string[] args)
        {
            Listener ls = new Listener("serial","COM1");
            ls.Run();
        }
    }
}
