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
        public byte RSSI;

        /// <summary>
        /// LQI measured during the packet reception
        /// </summary>
        public byte LQI;

        /// <summary>
        /// Source of the packet transmitted
        /// </summary>
        public UInt16 Src;

        /// <summary>
        /// Flag to determine if transmission was unicast 
        /// </summary>
        public bool Unicast;

        /// <summary>
        /// Received Message
        /// </summary>
        private byte[] ReceiveMessage;

        public UInt16 Size;

        /// <summary>
        /// Default constructor to create a received message with the default size
        /// </summary>
        public Message()
        {
            ReceiveMessage = new byte[MacMessageSize];
        }

        /// <summary>
        /// Constructor that takes a byte array as argument and marshals the Size,Payload, RSSI, LQI, Src, Unicast information out of it, in that order
        /// </summary>
        /// <param name="msg">The data array input</param>
        public Message(byte[] msg)
        {
            UInt16 i = 0;
            UInt16 length = (UInt16) msg[0];
            length |= (UInt16) (msg[1] << 8);

            Size = length;

            ReceiveMessage = new byte[MacMessageSize];

            for (i = 0; i < length; i++)
            {
                ReceiveMessage[i] = msg[i + 2];
            }

            RSSI = msg[i++ + 2];
            LQI = msg[i++ + 2];

            Src = msg[i++ + 2];
            Src |= (UInt16) (msg[i++ + 2] << 8);

            if (msg[i++ + 2] == 1)
                Unicast = true;
            else
                Unicast = false;

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
