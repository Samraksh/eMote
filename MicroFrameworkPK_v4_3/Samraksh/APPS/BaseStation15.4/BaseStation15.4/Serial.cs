using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Samraksh.SPOT.Hardware;
using System.IO.Ports;
using CircularBuffer;

namespace BaseStation15._4
{
    public enum RecieveState
    {
        IDLE,
        WAITINGFORHEADER,
        READINGHEADER,
        COMPLETEHEADER,
        READINGPAYLOAD,
        COMPLETEPAYLOAD,
        INITIALIZE,
    }

    public class Serial
    {
        public SerialPort serialPortInstance;
        private static System.Object lockThis = new System.Object();
        private static Serial SerialInstance;
        private CircularBuffer.CircularBuffer rbuffer;
        public Message tempmsg;
        public static Microsoft.SPOT.Hardware.OutputPort gpio2 = new Microsoft.SPOT.Hardware.OutputPort(Samraksh.SPOT.Hardware.EmoteDotNow.Pins.GPIO_J12_PIN2, false);
        public static RecieveState rxState;
        public static int size;
        public const int SIZEOFHEADER = 10;
        public Message partMsg;
        public static int partReadSize;
        

        private Serial(CircularBuffer.CircularBuffer sendBuffer)
        {
            
            serialPortInstance = new SerialPort("COM1", 115200);
            serialPortInstance.DataReceived += ReceiveHandler;
            serialPortInstance.Open();
            partMsg = new Message();
            rbuffer = sendBuffer;
            //serialPortInstance.ReadTimeout = 250;
        }

        public static Serial GetInstance(CircularBuffer.CircularBuffer recvBuffer)
        {
            if (SerialInstance == null)
            {
                lock (lockThis)
                {
                    if (SerialInstance == null)
                        SerialInstance = new Serial(recvBuffer);
                }
            }
            return SerialInstance;

        }

        public bool AdvanceState()
        {
            while (true)
            {
                switch (rxState)
                {
                    case RecieveState.IDLE:
                        return true;

                    case RecieveState.INITIALIZE:
                        rxState = RecieveState.WAITINGFORHEADER;
                        size = SIZEOFHEADER;
                        break;

                    case RecieveState.WAITINGFORHEADER:
                        int sizeOfPacket = serialPortInstance.BytesToRead;
                        serialPortInstance.Read(partMsg.GetMessage(), 0, sizeOfPacket);
                        partReadSize = sizeOfPacket;
                        if (partReadSize >= SIZEOFHEADER)
                            rxState = RecieveState.READINGPAYLOAD;
                        else
                        {
                            rxState = RecieveState.INITIALIZE;
                            partReadSize = 0;
                            partMsg.Clear();
                        }
                        break;

                    case RecieveState.READINGPAYLOAD:
                        sizeOfPacket = serialPortInstance.BytesToRead;
                        serialPortInstance.Read(partMsg.GetMessage(), partReadSize, sizeOfPacket);
                        partReadSize += sizeOfPacket;
                        byte[] tempBuffer = partMsg.GetMessage();
                        if (tempBuffer[partReadSize] == 126)
                        {
                            rxState = RecieveState.COMPLETEPAYLOAD;
                            rbuffer.put(partMsg);

                        }
                        break;

                    case RecieveState.COMPLETEPAYLOAD:
                        rxState = RecieveState.INITIALIZE;
                        partMsg.Clear();
                        partReadSize = 0;
                        break;
                    
                    default:
                        return false;

                }
            }
        }

        public void ReceiveHandler(Object sender, SerialDataReceivedEventArgs e)
        {
            gpio2.Write(true);
            gpio2.Write(false);
            if (serialPortInstance.IsOpen)
            {
                if (serialPortInstance.BytesToRead > 0)
                {
                    AdvanceState();
                    /*
                    int sizeOfPacket = serialPortInstance.BytesToRead;
                    byte[] tempBuffer = new byte[sizeOfPacket];
                    int bytesRead = serialPortInstance.Read(tempBuffer, 0, sizeOfPacket);
                    if (tempmsg.GetFillLevel() == 0)
                    {
                        tempmsg.Length = (int) tempBuffer[0];
                    }
                    if (tempmsg.GetFillLevel() == tempmsg.Length)
                    {
                        rbuffer.put(tempmsg);
                        tempmsg.Clear();
                    }
                       
                    tempmsg.load(tempBuffer, tempmsg.GetFillLevel(), sizeOfPacket);
                    */
                }
            }
        }
        
        public void Send(byte[] msg)
        {

            if (serialPortInstance.IsOpen)
            {         
                serialPortInstance.Write(msg, 0, 128);
                serialPortInstance.Flush();  
            }
            
        }

    }
}
