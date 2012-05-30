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

        UInt16 packetCount;

        bool sendComplete = false;
        bool        m_fUsb = true;
        //bool        m_fNewLine = true;
        static Byte[] data = new Byte[130];
        static Byte[] processedData = new Byte[64];
        BinaryWriter b = new BinaryWriter(File.Open("C:\\Main\\Work\\Projects\\Images\\Rose.jpg", FileMode.Append));
        //System.IO.StreamWriter file = new System.IO.StreamWriter(@"C:\Main\Work\Projects\Images\error.txt",true);
        System.IO.StreamWriter inpStats = new System.IO.StreamWriter(@"C:\Main\Work\Projects\Images\inputStats.txt", true);
        System.IO.StreamWriter outStats = new System.IO.StreamWriter(@"C:\Main\Work\Projects\Images\OutputStats.txt", true);

        const UInt16 packetLength = 128;

        static bool bDone = true;
        AsyncCallback rcb;

        int recRevecieved = 0;
        int numberOfRecords = 0;

        Thread m_sendThread;
        Thread m_startSendingThread;
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

        void HandleInput(byte[] buf)
        {
            String prnt = "";
            for (UInt16 i = 0; i < buf.Length; i++)
            {
                prnt += buf[i];
                prnt += " | ";
            }

            Console.WriteLine(prnt+ "\n");

            try
            {
                UsbStream.Write(buf, 0, buf.Length);
            }
            catch (Exception e)
            {
                Console.WriteLine("The Write Operation could not be performed. " + e.ToString());
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

       public byte[] flushBuffer(byte[] buffer)
       {
           for (int i = 0; i < buffer.Length; i++)
           {
               buffer[i] = 0;
           }
           return buffer;
       }

       // Designed to send binary picture like files with a packet type and packet number attached to the beginning, the objective is investigate the loss
       // of packets
       public void SendThread()
        {
            int Delay = 10;
            byte[] msg = new byte[134];
            String line;
            int pos = 0;
            UInt16 packetNumber = 0;
            byte packetType = 0;

            int bufferCounter = 6;

            Console.WriteLine("Commencing Picture Send \n");

            // Sending the start listening packet
            msg[0] = 0x01;
            msg[1] = 0x00;

            msg[2] = (byte)((packetLength & 0xff));
            msg[3] = (byte)((packetLength >> 8) & 0xff);

            msg[4] = (byte)((packetNumber & 0xff));
            msg[5] = (byte)((packetNumber >> 8) & 0xff);

            for (int i = 6; i < 134; i++)
                msg[i] = 0;

            packetNumber++;

            // Send three start listening packets to ensure receipt of packet
            HandleInput(msg);
            HandleInput(msg);
            HandleInput(msg);

            msg = flushBuffer(msg);

            Thread.Sleep(10);

            using (BinaryReader imgRead = new BinaryReader(File.Open("C:\\Main\\Work\\Projects\\Images\\desaturated-rose-small.jpg", FileMode.Open)))
            {
                while (pos < (int) imgRead.BaseStream.Length)
                {

                    byte x = imgRead.ReadByte();

                    msg[bufferCounter++] = x;

                    if (bufferCounter == 134)
                    {
                        // Packet Type is transmitting
                        Console.WriteLine("Transmitting a packet\n");

                        msg[0] = 0x03;
                        msg[1] = 0x00;

                        msg[2] = (byte)((packetLength & 0xff));
                        msg[3] = (byte)((packetLength >> 8) & 0xff);

                        msg[4] = (byte)((packetNumber & 0xff));
                        msg[5] = (byte)((packetNumber >> 8) & 0xff);

//                        inpStats.Write(Convert.ToString(System.Diagnostics.Stopwatch.GetTimestamp()) + "," + Convert.ToString(packetNumber) + "," + Convert.ToString(x) + "\n");
                        HandleInput(msg);
                        msg = flushBuffer(msg);
                        bufferCounter = 6;

                        numberOfRecords++;
                    }

                    pos += sizeof(byte);
                    Thread.Sleep(25);
                    //inpStats.Write(Convert.ToString(DateTime.Now.ToString("yyyyMMddHHmmssffffff")) + " | "  + Convert.ToString(packetNumber) + " | " + Convert.ToString(x) + "\n");
                    packetNumber++;
                }

                // send last packet
                msg[0] = 0x03;
                msg[1] = 0x00;

                msg[2] = (byte)(((bufferCounter - 1) & 0xff));
                msg[3] = (byte)(((bufferCounter - 1) >> 8) & 0xff);

                msg[4] = (byte)((packetNumber & 0xff));
                msg[5] = (byte)((packetNumber >> 8) & 0xff);

                HandleInput(msg);
                msg = flushBuffer(msg);
                bufferCounter = 6;

            }

            // Sending stop listening packet
            msg[0] = 0x02;
            msg[1] = 0x00;

            msg[2] = (byte)((packetLength & 0xff));
            msg[3] = (byte)((packetLength >> 8) & 0xff);

            msg[4] = (byte)((packetNumber & 0xff));
            msg[5] = (byte)((packetNumber >> 8) & 0xff);

            for (int i = 6; i < 134; i++)
                msg[i] = 0;


            packetNumber++;

            // Send three stop listening packets to ensure receipt of packet
            HandleInput(msg);
            HandleInput(msg);
            HandleInput(msg);

            Console.WriteLine(bufferCounter);
            /*
            // Sending start sending packet
            msg[0] = 0xf0;
            msg[1] = (byte)((packetNumber >> 8) & 0xff);
            msg[2] = (byte)((packetNumber & 0xff));
            msg[3] = 0;
            msg[4] = 0xff;

            packetNumber++;

            // Send three stop listening packets to ensure receipt of packet
            HandleInput(msg);
            HandleInput(msg);
            HandleInput(msg);
            */
            sendComplete = true;

            //SendDataThread();

            inpStats.Close();
            Thread.Sleep(10);
            outStats.Close();
        }

       void SendDataThread()
       {
           byte[] msg = new byte[134];
           // Sending the start listening packet
           msg[0] = 0x04;
           msg[1] = (byte) (36 & 0xff);

           msg[2] = (byte)((73 & 0xff));
           msg[3] = (byte)((73 >> 8) & 0xff);

           msg[4] = (byte)((0 & 0xff));
           msg[5] = (byte)((0 >> 8) & 0xff);

           for (int i = 6; i < 134; i++)
               msg[i] = 0;

           //packetNumber++;

           // Send three start listening packets to ensure receipt of packet
           HandleInput(msg);

          // Thread.Sleep(10);

           //HandleInput(msg);

          // Thread.Sleep(10);

          // HandleInput(msg);

          // Thread.Sleep(10);

       }

       
        void Run()
        {

            //Create Thread for Writing
            //AsyncCallback rcb = new AsyncCallback(ShowText);    //write callback
            m_sendThread = new Thread(new ThreadStart(SendThread));
            m_startSendingThread = new Thread(new ThreadStart(SendDataThread));
            //m_sendThread.Start();
            m_startSendingThread.Start();
           

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

        
        public void ShowText_debug(IAsyncResult result)
        {
            //String textToSend = Encoding.ASCII.GetString(data, 0, data.Length);
            //HandleInput(textToSend);
            for (int x = 0; x < data.Length; ++x)
            {
                if (data[x] == 0)
                    break;
                Console.Write((char)data[x]);
                data[x] = 0;
            }
            bDone = true;
            Console.Write("\n\r");
        }
         

        public void ShowText(IAsyncResult result)
        {
            Console.WriteLine("Recieved a packet ");
            UInt16 packetlost;
            byte packet;
            UInt16 packetNumber;
            UInt16 diff;
            byte header1;
            byte header2;
            String errInfo = "";

            Console.WriteLine("Got Packet\n");

            Console.WriteLine(Convert.ToString(recRevecieved++));

            String recvdMessage = "";

            for (int i = 0; i < data.Length; i++)
            {
                recvdMessage += Convert.ToString(data[i]) + " | ";
            }
            Console.WriteLine(recvdMessage + "\n");

           //if (sendComplete == false)
           //   return;

            header1 = (byte)data[0];
            header2 = (byte)data[1];
            if ((header1 == 0xff) && (header2 == 0x00))
            {
                packetCount++;
                //packet = data[3];
                //packetNumber = (UInt16) (data[1] << 8);
                //packetNumber = (UInt16) (data[2]);
                //outStats.Write(Convert.ToString(System.Diagnostics.Stopwatch.Frequency) + "\n");
                //outStats.Write(Convert.ToString(System.Diagnostics.Stopwatch.GetTimestamp()) + "," + Convert.ToString(packetNumber) + "," + Convert.ToString(packet) + "\n");
                using (BinaryWriter b = new BinaryWriter(File.Open("C:\\Main\\Work\\Projects\\Images\\FileSystemDemo\\Recieved.jpg", FileMode.Append)))
                {
                    if (packetCount < 38)
                    {
                        for (int i = 2; i < data.Length; i++)
                            b.Write(data[i]);
                    }
                    else
                    {
                        for (int i = 2; i < 73; i++)
                            b.Write(data[i]);
                    }
                }
                for (int i = 0; i < data.Length; i++)
                {
                    data[i] = 0;
                }
                
                //b.Write(data[4]);
            }
                /*
            else
            {
                for (int x = 0; x < data.Length; ++x)
                {
                    if (data[x] == 0)
                        break;
                    Console.Write((char)data[x]);
                    data[x] = 0;
                }
            }
                 */
            bDone = true;
            Console.Write("\n\r");
        }

        //--//

        static void Main(string[] args)
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
