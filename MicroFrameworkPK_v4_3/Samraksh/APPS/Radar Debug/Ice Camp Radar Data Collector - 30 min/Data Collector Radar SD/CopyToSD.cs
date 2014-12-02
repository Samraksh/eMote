using Microsoft.SPOT;
using Samraksh.AppNote.Utility;
using Samraksh.eMote.DotNow;
using Samraksh.eMote.NonVolatileMemory;
using System.IO.Ports;

namespace Samraksh.AppNote.DataCollector.Radar {
    public partial class Program {
        private const int BlockSizeBytes = 1024;
        private const byte EOF = 0xF0;  // A ushort of F0F0 (2 bytes of EOF) is larger than a 12-bit sample (max 0FFF)
        /// <summary>
        /// Copy DataStore data to the SD card
        /// </summary>
        /// <returns>True iff the copy succeeded</returns>
        private static bool WrtieToSerial() {
            // Set up the serial buffer
            //var serialBuffer = new byte[BlockSizeBytes];

            try {
                // Set up the serial port
                var serialPort = new SerialPort("COM1")
                {
                    BaudRate = 115200,
                    Parity = Parity.None,
                    StopBits = StopBits.One,
                    DataBits = 8,
                    Handshake = Handshake.None
                };
                serialPort.Open();

                

                //Debug.Print("Starting copy to SD");
                //EnhancedLcd.Display(LCDMessages.TransferingToSD);

                // Reuse the ADC I buffer
                var buffer = new ushort[ADCBufferSize * 3];
                // Byte buffer to hold the data to write to the SD card
                //var sdBytes = new byte[ADCBufferSize * sizeof(ushort)];
                var sdBytes = new byte[ADCBufferSize * 6];

                // Initialize the LargeDataReference
                //  This causes reads to start from the beginning
                if ((_retVal = LargeDataRef.InitializeRead()) != DataStoreReturnStatus.Success) {
                    Debug.Print("Error LargeDataReference.InitializeRead. Return value: " + _retVal);
                    return false;
                }

                //Debug.Print("   Sending data");

                int sendCnt = 0;
                while (true) {
                    sendCnt++;
                    LargeDataReference.ReturnStatus retValL;
                    // Try to read the next buffer's worth of data
                    //  If success, keep on
                    if ((retValL = LargeDataRef.ReadNext(buffer)) != LargeDataReference.ReturnStatus.Success) {
                        // If we've reached the end of the data, we're done
                        if (retValL == LargeDataReference.ReturnStatus.NoMoreData) {
                            break;
                        }
                        // Otherwise, something went wrong
                        Debug.Print("Error LargeDataReference.ReadNext: " + retValL);
                        return false;
                    }
                    
                        if (sendCnt > 5399)
                        {
                            break;
                        }
                    

                    // Convert the samples to a byte array
                    for (var i = 0; i < buffer.Length; i++) {
                        // Convert a sample to a pair of bytes
                        //  Note the "i << 1". This is a fast way to multiply by 2.
                        BitConverter.GetBytes(sdBytes, i << 1, buffer[i]);
                    }
                    // Write to the SD card. If not successful, return failure
                    /*if (!SD.Write(sdBytes, 0, (ushort) sdBytes.Length)) {
                        return false;
                    }*/
                    SerialWriteAndFlush(serialPort, sdBytes);
                    // For good measure, write a buffer's worth of EOF
                    /*for (var j = 0; j < serialBuffer.Length; j++)
                    {
                        serialBuffer[j] = EOF;
                    }
                    SerialWriteAndFlush(serialPort, serialBuffer);*/
                    // Print first and last values in the buffer
                    /*if (_debuggerIsAttached) {
                        Debug.Print(bufferCnt++ + " I: " + buffer[0] + ", Q: " + buffer[1]
                                    + " / I: " + buffer[buffer.Length - 2] + ", Q: " + buffer[buffer.Length - 1]);
                    }*/
                }

                // Write EOF and quit
                for (var i = 0; i < sdBytes.Length; i++) {
                    sdBytes[i] = Eof;
                }
                //return SD.Write(sdBytes, 0, (ushort)sdBytes.Length);
                return true;
            }
            finally {
                Debug.Print("");
            }
        }
        private static void SerialWriteAndFlush(SerialPort serialPort, byte[] serialBuffer)
        {
#if Testing
            Debug.Print("Buffer " + _bufferCntr);
                for (var i = 0; i < serialBuffer.Length; i += 2) {
                    var ushortVal = BitConverter.ToInt16(serialBuffer, i);
                    Debug.Print("  " + i + " " + ushortVal);
                }
                Debug.Print("");
            }
#else
            serialPort.Write(serialBuffer, 0, serialBuffer.Length);
            serialPort.Flush();
#endif
        }

        /// <summary>
        /// Show buffer write status
        /// </summary>
        /// <param name="alert">True iff alert</param>
        private static void AlertSerialWrite(bool alert)
        {
            // Note that LCD char numbers are right to left; 0 is right-most
            //EnhancedLCD.WriteN(1, (alert ? LCD.CHAR_1 : LCD.CHAR_NULL));
        }
    }
}
