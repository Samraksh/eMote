using System;
using Microsoft.SPOT;

namespace Samraksh.eMote.Net
{
    /// <summary>Message object. Passed to native</summary>
    public class Message
    {
        /// <summary>
        /// Defines the default size of the mac message
        /// </summary>
        const byte MacMessageSize = 128;

        /// <summary>Received Signal Strength of message</summary>
        /// <value>RSSI value</value>
        public byte RSSI;

        /// <summary>Link Quality Indication measured during the packet reception</summary>
        /// <value>LQI measured</value>
        public byte LQI;

        /// <summary>
        /// Source of the packet transmitted
        /// </summary>
        /// <value>Source of the packet</value>
        public UInt16 Src;

        /// <summary>Was message sent unicast?</summary>
        /// <value>True if message was sent unicast, else broadcast</value>
        public bool Unicast;

        /// <summary>
        /// Received Message
        /// </summary>
        private byte[] ReceiveMessage;

        /// <summary>Size of the message payload</summary>
        /// <value>Size of message payload</value>
        public UInt16 Size;

        /// <summary>The time at which the packet was sent out</summary>
        /// <value>Time the packet was sent out (microseconds)</value>
        public long SenderEventTimeStamp;

        /// <summary>The time at which the packet was sent out</summary>
        /// <value>Time the packet was sent out (microseconds)</value>
        [Obsolete("Use SenderEventTimestamp instead")]
        public long senderEventTimeStamp {
            get { return SenderEventTimeStamp; }
            set { SenderEventTimeStamp = value; }
        }


        private bool timeStamped;

        /// <summary>Check if message is timestamped</summary>
        /// <returns>True iff message is timestamped</returns>
        public bool IsSenderTimeStamped()
        {
            return timeStamped;
        }

        /// <summary>Create a message with the default size</summary>
        public Message()
        {
            ReceiveMessage = new byte[MacMessageSize];
        }

        /// <summary>Create a message with Size, Payload, RSSI, LQI, Src and Unicast information specified in message array</summary>
        /// <param name="msg">Message. Size, Payload, RSSI, LQI, Src and Unicast information specified in the first 6 bytes. Rest is payload</param>
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

            // Determines whether the message is unicast or not 
            if (msg[i++ + 2] == 1)
                Unicast = true;
            else
                Unicast = false;

            // Check if the message is timestamped from the sender 
            if (msg[i++ + 2] == 1)
                timeStamped = true;
            else
                timeStamped = false;

            // Elaborate conversion plan because nothing else works 
            UInt32 lsbItem = msg[i++ + 2];
            lsbItem |= ((UInt32)msg[i++ + 2] << 8);
            lsbItem |= ((UInt32)msg[i++ + 2] << 16);
            lsbItem |= ((UInt32)msg[i++ + 2] << 24);

            UInt32 msbItem = msg[i++ + 2];
            msbItem |= ((UInt32)msg[i++ + 2] << 8);
            msbItem |= ((UInt32)msg[i++ + 2] << 16);
            msbItem |= ((UInt32)msg[i++ + 2] << 24);

            
            long tempTimeStamp = ((long)msbItem << 32) | lsbItem;

            SenderEventTimeStamp = tempTimeStamp;

        }

        /// <summary>Create a message with specified parameters</summary>
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

        /// <summary>Create a message with specified parameters</summary>
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

        /// <summary>Create a message with specified parameters</summary>
        /// <param name="message"></param>
        /// <param name="Src"></param>
        /// <param name="Unicast"></param>
        /// <param name="RSSI"></param>
        /// <param name="LQI"></param>
        /// <param name="Size"></param>
        public Message(byte[] message, UInt16 Src, bool Unicast, byte RSSI, byte LQI, UInt16 Size, bool timeStamped)
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
            this.timeStamped = timeStamped;
        }

        /// <summary>Configure size of messages</summary>
        /// <param name="Size">Size of messages</param>
        public Message(int Size)
        {
            ReceiveMessage = new byte[Size];
        }

        /// <summary>Get the next message</summary>
        /// <returns>The message, as a byte array</returns>
        public byte[] GetMessage()
        {
            return ReceiveMessage;
        }

    }
}
