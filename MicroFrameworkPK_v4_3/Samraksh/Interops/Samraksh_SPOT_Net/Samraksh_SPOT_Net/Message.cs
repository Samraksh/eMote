using System;
using Microsoft.SPOT;

namespace Samraksh.SPOT.Net
{
    public class Message
    {
        /// <summary>
        /// Defines the default size of the mac message
        /// </summary>
        const byte MacMessageSize = 128;

        /// <summary>
        /// RSSI Value of the packet
        /// </summary>
        public byte RSSI { get; set; }

        /// <summary>
        /// LQI measured during the packet reception
        /// </summary>
        public byte LQI { get; set; }

        /// <summary>
        /// Source of the packet transmitted
        /// </summary>
        public UInt16 Src { get; set; }

        /// <summary>
        /// Flag to determine if transmission was unicast 
        /// </summary>
        public bool Unicast { get; set; }

        /// <summary>
        /// Received Message
        /// </summary>
        private byte[] ReceiveMessage;

        /// <summary>
        /// Default constructor to create a received message with the default size
        /// </summary>
        public Message()
        {
            ReceiveMessage = new byte[MacMessageSize];
        }


        /// <summary>
        /// Build a MAC Message Object with the constructor 
        /// </summary>
        /// <param name="message">Message payload</param>
        /// <param name="Src">Source of the packet</param>
        /// <param name="Unicast">Was transmission unicast</param>
        /// <param name="RSSI">RSSI</param>
        /// <param name="LQI">LQI</param>
        public Message(byte[] message, UInt16 Src, bool Unicast, byte RSSI, byte LQI)
        {
            //Create a message object of default size
            ReceiveMessage = new byte[MacMessageSize];

            // Copy the message to the receive message buffer the traditional way 
            for (int i = 0; i < message.Length; i++)
            {
                ReceiveMessage[i] = message[i];
            }

            // Copy other parameters to this object 
            this.Src = Src;
            this.Unicast = Unicast;
            this.RSSI = RSSI;
            this.LQI = LQI;
        }

        /// <summary>
        /// Build a MAC Message Object with the constructor 
        /// </summary>
        /// <param name="message">Message payload</param>
        /// <param name="Src">Source of the packet</param>
        /// <param name="Unicast">Was transmission unicast</param>
        /// <param name="RSSI">RSSI</param>
        /// <param name="LQI">LQI</param>
        /// <param name="Size">Size of the payload buffer </param>
        public Message(byte[] message, UInt16 Src, bool Unicast, byte RSSI, byte LQI, UInt16 Size)
        {
            //Create a message object of default size
            ReceiveMessage = new byte[Size];

            // Copy the message to the receive message buffer the traditional way 
            for (int i = 0; i < message.Length; i++)
            {
                ReceiveMessage[i] = message[i];
            }

            // Copy other parameters to this object 
            this.Src = Src;
            this.Unicast = Unicast;
            this.RSSI = RSSI;
            this.LQI = LQI;
        }

        /// <summary>
        /// Configurable size of the received message
        /// </summary>
        /// <param name="Size">Size of the array</param>
        public Message(int Size)
        {
            ReceiveMessage = new byte[Size];
        }

        /// <summary>
        /// Get the message
        /// </summary>
        /// <returns></returns>
        public byte[] GetMessage()
        {
            return ReceiveMessage;
        }

    }
}
