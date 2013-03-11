using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace SamrakshEmulator
{
    enum VirtEmulatorInterface
    {
        ADC, USART, SPI, I2C, USB,
    } ;

    class PhysicalModelVirtCom
    {
        int serverPort = 47000; //default port
        Thread clientThread;
        Socket clientSocket;
        public bool Connected;
        static TcpListener myListener;
        Logger Log;

        public delegate void EmulatorCommsCallback(byte[] Message, int length);
        EmulatorCommsCallback emulatorCallback;
        /*public PhysicalModelVirtCom()
        {
            clientThread = new Thread(new ThreadStart(HandleClients));
        }
        public PhysicalModelVirtCom(int port)
        {
            serverPort = port;
            this.PhysicalModelVirtCom();
        }*/

        public PhysicalModelVirtCom(EmulatorCommsCallback callback)
        {
            emulatorCallback = callback;
        }

        ~PhysicalModelVirtCom()
        {
            myListener.Stop();
            //clientSocket.Close();
            //clientThread.Abort();      
        }
        public void StartVirtComServer()
        {
            try
            {
                Log = new Logger();
                
                //IPAddress ipAd = IPAddress.Parse("localhost");
                IPAddress ipAd = IPAddress.Loopback;
                // use local m/c IP address, and 
                // use the same in the client

                /* Initializes the Listener */
                myListener = new TcpListener(ipAd, serverPort);

                /* Start Listeneting at the specified port */
                myListener.Start();

                Log.Debug(Module.PHYCOMM, "The server is running at port: " + serverPort.ToString() + "...");
                Log.Debug(Module.PHYCOMM, "The local End point is  :" + myListener.LocalEndpoint);

                Log.Debug(Module.PHYCOMM, "Starting thread to accept connections.....");
                clientThread = new Thread(new ThreadStart(HandleClients));
                clientThread.Start();
                
            }
            catch (Exception e)
            {
                Log.Debug(Module.PHYCOMM, "Error..... " + e.StackTrace);
            }

        }
        
        void HandleClients()
        {        
            Log.Debug(Module.PHYCOMM, "Waiting for connection...");
            try
            {
                clientSocket = myListener.AcceptSocket();
                Log.Debug(Module.PHYCOMM, "Connection accepted from " + clientSocket.RemoteEndPoint);

                byte[] b = new byte[100];
                int k = clientSocket.Receive(b);
                Log.Debug(Module.PHYCOMM, "Recieved...");
                for (int i = 0; i < k; i++)
                    Console.Write(Convert.ToChar(b[i]));

                ASCIIEncoding asen = new ASCIIEncoding();
                clientSocket.Send(asen.GetBytes("The string was recieved by the server."));
                Log.Debug(Module.PHYCOMM, "Sent Acknowledgement");
                Connected = true;                  
            }
            catch (Exception e)
            {
                Log.Debug(Module.PHYCOMM, "Error..... " + e.StackTrace);
            }
            //Keep listening on clientSocket
            while (true)
            {
                byte[] b = new byte[100];
                int k = clientSocket.Receive(b);
                if (k > 0)
                {
                    //Log.Debug(Module.PHYCOMM, "Got Something from model of length"+ k);
                    emulatorCallback(b, k);
                }
            }  
            
        }

        
        private void SendToADC(byte[] pkt, int length)
        {
            
        }

        public int SendModelMessage(byte[] mesg)
        {
            return clientSocket.Send(mesg);
        }
    }
}
