using System;
using Microsoft.SPOT;
using System.Runtime.CompilerServices;

namespace Samraksh
{
    public class Memory
    {
        // Record Id unique to a pointer
        private int recordId;

        // Stores the current record id
        private static int currentRecordId = 0;

        private byte[] buffer;

        private static int getRecordId()
        {
            return currentRecordId++;
        }

        public Memory(UInt32 bytes)
        {
            recordId = getRecordId();

            buffer = new byte[bytes];

            constructNativeMemoryPointer(recordId, bytes);

        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool write(int recordId, byte[] data);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool constructNativeMemoryPointer(int recordId, UInt32 bytes);

        // native call that destroys record created on the flash
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool disposeNativeMemoryPointer(int recordId);


        // write data to flash device
        public bool write(int data)
        {
            byte[] buffer = new byte[sizeof(int)];
            return write(recordId, buffer);
        }

        public byte[] read()
        {
            read(recordId, buffer);
            return buffer;
        }

        // read data from flash device
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern private bool read(int recordId, byte[] buffer);

        // destroys the record on the flash
        ~Memory()
        {
            disposeNativeMemoryPointer(recordId);
        }

    }
}
