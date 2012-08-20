using System;
using System.IO;
using System.Text;
using System.Collections;
using System.Reflection;
using System.Threading;
using System.Diagnostics;

using _DBG = Microsoft.SPOT.Debugger;
using _WP  = Microsoft.SPOT.Debugger.WireProtocol;

namespace Microsoft.SPOT.Tools
{
    class SerialDump
    {
      //  _DBG.Engine m_eng;
        Stream UsbStream;
       
        bool        m_fUsb = true;
        //bool        m_fNewLine = true;
        static Byte[] data = new Byte[64];
        static bool bDone = true;
        AsyncCallback rcb;

        Thread m_sendThread;
        //--//

        SerialDump( string[] args )
        {
            _DBG.PortDefinition pd       = null;
            string              port     = null;
            uint                baudrate = 0;
            
            for(int i=0; i<args.Length; i++)
            {
                string arg = args[i];

                if(String.Compare( arg, "-usb", true ) == 0)
                {
                    m_fUsb = true;

                    continue;
                }

                if (String.Compare(arg, "-serial", true) == 0)
                {
                    m_fUsb = false;

                    continue;
                }

                if(port     == null) { port     =               arg  ; continue; }
                if(baudrate == 0   ) { baudrate = UInt32.Parse( arg ); continue; }
            }

            if(m_fUsb)
            {
                _DBG.PortDefinition[] ports = _DBG.AsyncUsbStream.EnumeratePorts();
                //_DBG.PortDefinition ports = _DBG.PortDefinition_Usb.CreateInstanceForUsb("Micro Framework MXS Reference Board", "");

                if(port == null)
                {
                    if(ports.Length == 0)
                    {
                        System.Console.WriteLine( "No Usb SPOT device is present" );

                        throw new ApplicationException();
                    }
                    else if(ports.Length == 1)
                    {
                        pd = ports[0];
                    }
                    else
                    {
                        System.Console.WriteLine( "More than one USB SPOT device is present" );
                        System.Console.WriteLine( "To dump data from a specific device, choose your device from the list below and execute:" );
                        System.Console.WriteLine( "  serialdump -usb <device> " );
                        System.Console.WriteLine( "" );

                        //
                        // More than one usb device attached; dump list so user can choose
                        //
                        for(int i = 0; i < ports.Length; ++i)
                        {
                            System.Console.WriteLine( "Device " + i + ": " + ports[i].DisplayName );
                        }

                        throw new ApplicationException();
                    }
                }
                else
                {
                    foreach(_DBG.PortDefinition pd2 in ports)
                    {
                        if(port.Equals( pd2.DisplayName ))
                        {
                            pd = pd2;
                            break;
                        }
                    }
                }
            }

            if(pd == null)
            {
                if(port     == null) port     = "COM1";
                if(baudrate == 0   ) baudrate = 115200;

                pd = _DBG.PortDefinition.CreateInstanceForSerial( port, port, baudrate );
            }

            //m_eng = new _DBG.Engine( pd );
            //AsyncFileStream afs = null;

            //afs = new AsyncFileStream(pd.m_port, System.IO.FileShare.ReadWrite);
            if (pd.TryToOpen())
            {
                UsbStream = pd.Open();
                Console.WriteLine("Opening port: \n\r" + pd.DisplayName.ToString());
                Console.WriteLine("properties: \n\r" + pd.Properties + "\n\r port: \n\r" + pd.Port + 
                    "\n\r persist name: \n\r" + pd.PersistName + "\n\r unique id: \n\r" + pd.UniqueId );
            }
            else
            {
                Console.WriteLine("Unable to open USB port");
            }
        }
        
       void HandleInput(string text)
        {
            //Console.WriteLine("####### PRESS RETURN TO EXIT #######");
            //Console.ReadLine();
            byte[] buf = Encoding.ASCII.GetBytes(text);
            try
            {
                UsbStream.Write(buf, 0, buf.Length);
            }
            catch (IOException e)
            {
                Console.WriteLine(
                    "The write operation could not be performed. " +
                    //e.GetBaseException() +
                    e.ToString());
            }
        }

        public void SendThread()
        {
            int Delay = 10;
            for (int i = 0; i < 110; i++)
            {
                //Thread.Sleep(100);
                
                if (i > Delay)
                {
                    String msg="";
                    int k;
                    for (k = 0; k < (i - Delay); k++)
                    {
                        msg += Convert.ToString(k)+"-";
                    }
                    msg += " \n\r";
                    HandleInput(msg);

                    Console.WriteLine("Writing message:" + Convert.ToString(i - Delay));
                }
                else
                {
                    Console.WriteLine("Writing in: " + Convert.ToString(Delay - i));
                }
                Thread.Sleep(1000);
            }      
        } 

        void Run()
        {

            //Create Thread for Writing
            //AsyncCallback rcb = new AsyncCallback(ShowText);    //write callback
            //m_sendThread = new Thread(new ThreadStart(SendThread));
            //m_sendThread.Start();
           

            //Start reading, never quit
            rcb = new AsyncCallback(ShowText); //read callback
            //long Length = UsbStream.Length;
            int count = 0;
            while (true)
            {
                //if (Length == UsbStream.Position)
                    //break;
                if (bDone)
                {
                    bDone = false;
                    UsbStream.BeginRead(data, 0, data.Length, rcb, count);
                    ++count;
                }
                //Thread.Sleep(250);
            }
              
            //Console.WriteLine( "####### PRESS RETURN TO EXIT #######" );
            //Console.ReadLine();

           
        }


        public void ShowText(IAsyncResult result)
        {
            ushort value = 0;
            //String textToSend = Encoding.ASCII.GetString(data, 0, data.Length);
            //HandleInput(textToSend);
            value = data[0];
            value |= (ushort) (data[1] << 8);
            Console.WriteLine(value);
            /*
            for (int x = 0; x < data.Length; ++x)
            {
                
                Console.Write(data[x]);
                data[x] = 0;
            }
             */
            bDone = true;
            Console.Write("\n\r");
        }
      

        //--//

        static void Main( string[] args )
        {
            try
            {
                SerialDump o = new SerialDump( args );

                o.Run();
            }
            catch(ApplicationException)
            {
            }
            catch(Exception e)
            {
                Console.WriteLine( "{0}", e.ToString() );
            }

           
        }
    }
}
