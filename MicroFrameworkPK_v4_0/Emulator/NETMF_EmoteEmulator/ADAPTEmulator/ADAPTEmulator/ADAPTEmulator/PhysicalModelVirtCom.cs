using System;
using System.Collections.Generic;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace ADAPTEmulator
{
    enum VirtEmulatorInterface
    {
        USART, SPI, I2C, USB,
    } ;

    class PhysicalModelVirtCom
    {
        int serverPort = 47000; //default port
        Thread clientThread;
        Socket clientSocket;
        public bool Connected;
        static TcpListener myListener;
        Logger Log;

        /*public PhysicalModelVirtCom()
        {
            clientThread = new Thread(new ThreadStart(HandleClients));
        }
        public PhysicalModelVirtCom(int port)
        {
            serverPort = port;
            this.PhysicalModelVirtCom();
        }*/

        
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
           // while (true)
            //{
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

            //}
            
        }
        public int SendModelMessage(byte[] mesg)
        {
            return clientSocket.Send(mesg);
        }
    }
}
